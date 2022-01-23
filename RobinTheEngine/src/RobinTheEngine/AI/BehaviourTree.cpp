#include "rtepch.h"
#include "BehaviourTree.h"

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

Parallel::Parallel(Policy p): policy(p) {}

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
            state.currentBehaviour->abort(treePath); // running leaves (actions / conditions)
        }
        treePath.path.pop_back();
        state = treePath.path.back();
    }
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

BehaviourTree::BehaviourTree(const std::string& path) {
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
    
}
