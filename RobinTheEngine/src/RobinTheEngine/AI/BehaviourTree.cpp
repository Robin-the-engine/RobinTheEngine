#include "rtepch.h"

#include <unordered_map>
#include <format>
#include <charconv>
#include "../Log.h"
#include "../Exceptions/BehaviourException.h"
#include "../ResourceFactory.h"
#include "yaml-cpp/yaml.h"
#include "RobinTheEngine/Scene/Components/AI.h"
#include "RobinTheEngine/Scene/GameObject.h"
#include "BehaviourTree.h"

namespace {
    auto logger = RTE::Log::GetLogger("behaviours");
    
    // YAML keywords
    const std::string PARALLEL = "parallel";
    const std::string POLICY = "policy";
    const std::string SEQUENCE = "sequence";
    const std::string SELECTOR = "selector";
    const std::string REPEAT = "repeat";
    const std::string TIMES = "times";
    const std::string CONDITION = "condition";
    const std::string ACTION = "action";
    const std::string INVERT = "invert";

    using RTE::Parallel;
    using RTE::Behaviour;

    const std::unordered_map<std::string, Parallel::Policy> policymap = {
        {"REQUIRE_ALL", Parallel::REQUIRE_ALL},
        {"FAILURE_ALL", Parallel::FAILURE_ALL},
        {"SUCCESS_ALL", Parallel::SUCCESS_ALL},
        {"REQUIRE_ANY", Parallel::REQUIRE_ANY},
        {"FAILURE_ANY", Parallel::FAILURE_ANY},
        {"SUCCESS_ANY", Parallel::SUCCESS_ANY},
    };


    class TreeBuilder final {
    public:
        TreeBuilder() = default;
        void build(
            YAML::Node currentNode,
            std::shared_ptr<Behaviour>& parent,
            const std::string& parentName
        );
        void build( // first node
            YAML::Node node,
            std::shared_ptr<Behaviour>& firstNode
        );
        void processNode(
            YAML::Node node,
            std::shared_ptr<Behaviour>& parent,
            const std::string& parentName
        );
    private:
        std::shared_ptr<Behaviour> generateNode(const std::string& nodeName);
    };
}

using namespace RTE;

TreeState::TreeState(AIComponent* ai) {
    setOwner(ai);
}
void TreeState::setOwner(AIComponent* ai) {
    owner = ai;
}

void Behaviour::addChild(std::shared_ptr<Behaviour>) {}
void Behaviour::abort(TreeState& treeState) {}

TickResult Sequence::tick(TreeState& treeState) {
    TickResult tickResult = TickResult::SUCCESS;
    auto& pathInfo = treeState.path.back();
    while (tickResult == TickResult::SUCCESS && pathInfo.currentChild < childs.size()) {
        auto child = childs[pathInfo.currentChild];
        BehaviourState newState;
        newState.currentBehaviour = child;
        treeState.path.push_back(newState);
        tickResult = child->tick(treeState);
        if (tickResult == TickResult::RUNNING) {
            pathInfo.running = true;
            break;
        }
        treeState.path.pop_back();
        pathInfo.currentChild++;
        pathInfo.running = false;
    }
    return tickResult;
}

TickResult Selector::tick(TreeState& treeState) {
    TickResult tickResult = TickResult::SUCCESS;
    auto& pathInfo = treeState.path.back();
    while (tickResult == TickResult::FAILURE && pathInfo.currentChild < childs.size()) {
        auto child = childs[pathInfo.currentChild];
        BehaviourState newState;
        newState.currentBehaviour = child;
        treeState.path.push_back(newState);
        tickResult = child->tick(treeState);
        if (tickResult == TickResult::RUNNING) {
            break;
        }
        treeState.path.pop_back();
        pathInfo.currentChild++;
    }
    return tickResult;
}

TickResult Parallel::tick(TreeState& treeState) {
    TickResult tickResult = TickResult::SUCCESS;
    auto& pathInfo = treeState.path.back();
    while (pathInfo.currentChild < childs.size()) {
        auto child = childs[pathInfo.currentChild];
        BehaviourState newState;
        newState.currentBehaviour = child;
        treeState.path.push_back(newState);
        tickResult = child->tick(treeState);
        if (tickResult != TickResult::RUNNING) {
            treeState.path.pop_back();
        }
        if (policy == SUCCESS_ANY && tickResult == TickResult::SUCCESS ||
            policy == FAILURE_ANY && tickResult == TickResult::FAILURE ||
            policy == REQUIRE_ANY && tickResult != TickResult::RUNNING) {
            break;
        }
        pathInfo.currentChild++;
        if (policy == SUCCESS_ALL && tickResult == TickResult::FAILURE ||
            policy == FAILURE_ALL && tickResult == TickResult::SUCCESS) {
            // *_ALL condition is not achieved, restart subtree
            abort(treeState);
            pathInfo.currentChild = 0;
            tickResult = TickResult::SUCCESS;
        }
    }
    abort(treeState);
    return tickResult;
}

void Parallel::abort(TreeState& treeState) {
    auto *state = &treeState.path.back();
    while (state->currentBehaviour.get() != this) {
        if (state->running) {
            state->currentBehaviour->abort(treeState); // running leaves (actions), then their parents (seq, sel, par, etc)
        }
        treeState.path.pop_back();
        state = &treeState.path.back();
    }
}

void Parallel::setPolicy(Policy p) {
    policy = p;
}

void Decorator::addChild(std::shared_ptr<Behaviour> behaviour) {
    child = behaviour;
}

TickResult Repeat::tick(TreeState& treeState) {
    auto& pathInfo = treeState.path.back();
    // here we have only one child, so
    // we can reuse pathInfo.currentChild as our cycle counter.
    // And also we use pathInfo.running as abort signal here,
    // because we always either running or succeeded.
    if (repeatCondition(pathInfo.currentChild)) {
        pathInfo.running = true;
        BehaviourState newState;
        newState.currentBehaviour = child;
        treeState.path.push_back(newState);
        TickResult tickResult = child->tick(treeState);
        if (tickResult != TickResult::RUNNING) {
            treeState.path.pop_back();
            pathInfo.currentChild++;
        }
        return TickResult::RUNNING;
    }
    pathInfo.running = false;
    return TickResult::SUCCESS;
}

void Repeat::setTimes(size_t times) {
    repeatCondition = [times = times](size_t repeats) {return repeats < times; };
}

TickResult Invert::tick(TreeState& treeState) {
    BehaviourState newState;
    newState.currentBehaviour = child;
    treeState.path.push_back(newState);
    TickResult tickResult = child->tick(treeState);
    if (tickResult == TickResult::RUNNING) {
        return tickResult;
    }
    treeState.path.pop_back();
    if (tickResult == TickResult::SUCCESS) {
        return TickResult::FAILURE;
    }
    return TickResult::SUCCESS;
}

ScriptComponent& checkAndGetScript(TreeState& treeState) {
    assert(treeState.owner->GetGameObject().HasComponent<ScriptComponent>(),
        "AI do not have script to execute");
    return treeState.owner->GetGameObject().GetComponent<ScriptComponent>();
}

TickResult luaExecExternal(ScriptComponent& script, const std::string code) {
    auto& lua = script.getStateRef();
    const auto uglyName = "_LUA_EXECUTE_EXTERNAL_123_";
    auto wrapped = std::format(
        R"SCRIPT(
            {} = load('return {}')()
            if( {} == true ) then
                {} = TickResult.SUCCESS
            elseif ( {} == false) then
                {} = TickResult.FAILURE
            end
        )SCRIPT",
        uglyName, code, uglyName, uglyName, uglyName, uglyName);
    try {
        lua.script(wrapped);
        auto res = lua[uglyName];
        return res.get<TickResult>();
    }
    catch (sol::error& e) {
        logger->error(std::format("Error in script {}\nmsg: {}", wrapped, e.what()));
        return TickResult::FAILURE;
    }
}

TickResult Action::tick(TreeState& treeState) {
    auto& script = checkAndGetScript(treeState);
    auto execLine = std::format("{}({})", fname, fparams);
    auto result = luaExecExternal(script, execLine);
    if (result == TickResult::RUNNING) {
        treeState.path.back().running = true;
    }
    return result;
}

void Action::abort(TreeState& treeState) {
    Behaviour::abort(treeState);
    auto& script = checkAndGetScript(treeState);
    script.callf("abort");
}

void Action::setAction(const std::string& description) {
    auto firstSpace = description.find(' ');
    fname = description.substr(0, firstSpace);

    if (firstSpace != std::string::npos) {
        fparams = description.substr(firstSpace + 1);
    }
}

TickResult Condition::tick(TreeState& treeState) {
    auto& script = checkAndGetScript(treeState);

    auto get_operand = [](const std::string& op) {
        double res;
        auto [ptr, ec] { std::from_chars(op.data(), op.data() + op.size(), res) };
        if (ec == std::errc()) {
            return op;
        }
        return std::format("BlackBoard:get(\\'{}\\')", op);
    };

    auto execLine = std::format("{} {} {}",
        get_operand(condition[0]), condition[1], get_operand(condition[2]));

    return luaExecExternal(script, execLine);
}

void Condition::updateCondition(const std::string& condPart) {
    if (cur_el == COND_SIZE) {
        throw RTE::BehaviourException(
            "Invalid condition. Condition should consist of three parts: "
            "left operand, binary operation, right operand"
        );
    }
    condition[cur_el++] = condPart;
}

void SeqBehaviour::addChild(std::shared_ptr<Behaviour> behaviour) {
    childs.push_back(behaviour);
}

void BehaviourTree::setResourceId(const std::string& ResourceId) {
    bti = ResourceFactory::Get().GetResource<BehaviourTreeImpl>(ResourceId);
    inited = true;
}

BehaviourTree::BehaviourTree(const std::string& ResourceId) {
    setResourceId(ResourceId);
}

TickResult BehaviourTree::tick() {
    if (!inited) {
        return TickResult::FAILURE;
    }
    assert(GetGameObject().HasComponent<AIComponent>());
    auto& state = GetGameObject().GetComponent<AIComponent>().getTreeState();
    if (state.path.empty()) {
        state.path.emplace_back(bti.getHead(), 0, false);
    }
    auto tickResult = state.path.back().currentBehaviour->tick(state);
    if (tickResult != TickResult::RUNNING) {
        state.path.pop_back();
    }
    return tickResult;
}

BehaviourTreeImpl::BehaviourTreeImpl(const std::string& path) : BaseResource("BehaviourTree") {
    buildTree(path);
}

std::shared_ptr<Behaviour> BehaviourTreeImpl::getHead() {
    return head;
}

void BehaviourTreeImpl::buildTree(const std::string& path) {
    TreeBuilder tb;
    YAML::Node node = YAML::LoadFile(path);
    tb.build(node, head);
}

/*
 * TreeBuilder realization
 */

void TreeBuilder::build(YAML::Node node, std::shared_ptr<Behaviour>& firstNode) {
    auto nodeInfo = node.begin();
    auto nodeName = nodeInfo->first.Scalar();
    firstNode = generateNode(nodeName);
    build(nodeInfo->second, firstNode, nodeName);
}

void TreeBuilder::processNode(
    YAML::Node node,
    std::shared_ptr<Behaviour>& parent,
    const std::string& parentName
) {
    auto nodeName = node.begin()->first.Scalar();
    //logger->trace(std::format("node is: {}", nodeName));
    if (nodeName == TIMES) {
        if (parentName != REPEAT) {
            throw RTE::BehaviourException(
                "times node not directly under the repeat node"
            );
        }
        dynamic_cast<Repeat*>(parent.get())->setTimes(node[TIMES].as<int>());
        return;
    }
    if (nodeName == POLICY) {
        if (parentName != PARALLEL) {
            throw RTE::BehaviourException(
                "policy node not directly under the parallel node"
            );
        }
        const Parallel::Policy policy = policymap.at(node[POLICY].as<std::string>());
        dynamic_cast<Parallel*>(parent.get())->setPolicy(policy);
        return;
    }
    if (parentName == CONDITION) {
        dynamic_cast<Condition*>(parent.get())->updateCondition(node.Scalar());
        return;
    }
    if (parentName == ACTION) {
        dynamic_cast<Action*>(parent.get())->setAction(node.Scalar());
        return;
    }
    auto current = generateNode(nodeName);
    build(node.begin()->second, current, nodeName);
    parent->addChild(current);
}

void TreeBuilder::build(
    YAML::Node currentNode,
    std::shared_ptr<Behaviour>& parent,
    const std::string& parentName
) {
    if (currentNode.IsMap() || currentNode.IsScalar()) {
        processNode(currentNode, parent, parentName);
    }
    if (currentNode.IsSequence()) {
        for (auto node : currentNode) {
            processNode(node, parent, parentName);
        }
    }
}

std::shared_ptr<Behaviour> TreeBuilder::generateNode(const std::string& nodeName) {
    if (nodeName == PARALLEL) {
        return std::make_shared<Parallel>();
    }
    if (nodeName == SEQUENCE) {
        return std::make_shared<Sequence>();
    }
    if (nodeName == SELECTOR) {
        return std::make_shared<Selector>();
    }
    if (nodeName == ACTION) {
        return std::make_shared<Action>();
    }
    if (nodeName == CONDITION) {
        return std::make_shared<Condition>();
    }
    if (nodeName == REPEAT) {
        return std::make_shared<Repeat>();
    }
    if (nodeName == INVERT) {
        return std::make_shared<Invert>();
    }
    throw RTE::BehaviourException(std::format("Unknown node name: {}", nodeName).c_str());
}
