#include "rtepch.h"
#include "../Log.h"
#include "BehaviourTree.h"
#include "../Exceptions/BehaviourException.h"
#include "yaml-cpp/yaml.h"
#include <unordered_map>
#include <format>

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

    const std::unordered_map<std::string, Parallel::Policy> policymap = {
        {"REQUIRE_ALL", Parallel::REQUIRE_ALL},
        {"FAILURE_ALL", Parallel::FAILURE_ALL},
        {"SUCCESS_ALL", Parallel::SUCCESS_ALL},
        {"REQUIRE_ANY", Parallel::REQUIRE_ANY},
        {"FAILURE_ANY", Parallel::FAILURE_ANY},
        {"SUCCESS_ANY", Parallel::SUCCESS_ANY},
    };


    class TreeBuilder {
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

void Behaviour::addChild(std::shared_ptr<Behaviour>) {}
void Behaviour::abort(TreeState& treePath) {}

TickResult Sequence::tick(TreeState& treePath) {
    TickResult tickResult = TickResult::SUCCESS;
    auto& pathInfo = treePath.path.back();
    while (tickResult == TickResult::SUCCESS && pathInfo.currentChild < childs.size()) {
        auto child = childs[pathInfo.currentChild];
        BehaviourState newState;
        newState.currentBehaviour = child;
        treePath.path.push_back(newState);
        tickResult = child->tick(treePath);
        if (tickResult == TickResult::RUNNING) {
            pathInfo.running = true;
            break;
        }
        treePath.path.pop_back();
        pathInfo.currentChild++;
        pathInfo.running = false;
    }
    return tickResult;
}

TickResult Selector::tick(TreeState& treePath) {
    TickResult tickResult = TickResult::SUCCESS;
    auto& pathInfo = treePath.path.back();
    while (tickResult == TickResult::FAILURE && pathInfo.currentChild < childs.size()) {
        auto child = childs[pathInfo.currentChild];
        BehaviourState newState;
        newState.currentBehaviour = child;
        treePath.path.push_back(newState);
        tickResult = child->tick(treePath);
        if (tickResult == TickResult::RUNNING) {
            break;
        }
        treePath.path.pop_back();
        pathInfo.currentChild++;
    }
    return tickResult;
}

TickResult Parallel::tick(TreeState& treePath) {
    TickResult tickResult = TickResult::SUCCESS;
    auto& pathInfo = treePath.path.back();
    while (pathInfo.currentChild < childs.size()) {
        auto child = childs[pathInfo.currentChild];
        BehaviourState newState;
        newState.currentBehaviour = child;
        treePath.path.push_back(newState);
        tickResult = child->tick(treePath);
        if (tickResult != TickResult::RUNNING) {
            treePath.path.pop_back();
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
            abort(treePath);
            pathInfo.currentChild = 0;
            tickResult = TickResult::SUCCESS;
        }
    }
    abort(treePath);
    return tickResult;
}

void Parallel::abort(TreeState& treePath) {
    auto &state = treePath.path.back();
    while (state.currentBehaviour.get() != this) {
        if (state.running) {
            state.currentBehaviour->abort(treePath); // running leaves (actions), then their parents (seq, sel, par, etc)
        }
        treePath.path.pop_back();
        state = treePath.path.back();
    }
}

void Parallel::setPolicy(Policy p) {
    policy = p;
}

void Decorator::addChild(std::shared_ptr<Behaviour> behaviour) {
    child = behaviour;
}

TickResult Repeat::tick(TreeState& treePath) {
    auto& pathInfo = treePath.path.back();
    // here we have only one child, so
    // we can reuse pathInfo.currentChild as our cycle counter.
    // And also we use pathInfo.running as abort signal here,
    // because we always either running or succeeded.
    if (repeatCondition(pathInfo.currentChild)) {
        pathInfo.running = true;
        BehaviourState newState;
        newState.currentBehaviour = child;
        treePath.path.push_back(newState);
        TickResult tickResult = child->tick(treePath);
        if (tickResult != TickResult::RUNNING) {
            treePath.path.pop_back();
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

TickResult Invert::tick(TreeState& treePath) {
    BehaviourState newState;
    newState.currentBehaviour = child;
    treePath.path.push_back(newState);
    TickResult tickResult = child->tick(treePath);
    if (tickResult == TickResult::RUNNING) {
        return tickResult;
    }
    treePath.path.pop_back();
    if (tickResult == TickResult::SUCCESS) {
        return TickResult::FAILURE;
    }
    return TickResult::SUCCESS;
}

TickResult Action::tick(TreeState& treePath) {
    return {};
}

void Action::abort(TreeState& tree_path) {
    Behaviour::abort(tree_path);
}

TickResult Condition::tick(TreeState& treePath) {
    return {};
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

BehaviourTree::BehaviourTree(const std::string& path): BaseResource("BehaviourTree") {
    buildTree(path);
}

TickResult BehaviourTree::tick(TreeState& state) {
    if (state.path.empty()) {
        state.path.emplace_back(head, 0, false);
    }
    auto tickResult = state.path.back().currentBehaviour->tick(state);
    if (tickResult != TickResult::RUNNING) {
        state.path.pop_back();
    }
    return tickResult;
}

void BehaviourTree::buildTree(const std::string& path) {
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
    logger->trace(std::format("node is: {}", nodeName));
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
    auto current = generateNode(nodeName);
    build(node.begin()->second, current, nodeName);
    parent->addChild(current);
}

void TreeBuilder::build(
    YAML::Node currentNode,
    std::shared_ptr<Behaviour>& parent,
    const std::string& parentName
) {
    if (currentNode.IsMap()) {
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
