#include "rtepch.h"
#include "AI.h"
#include "../../Scene/GameObject.h"
using namespace RTE;

AIComponent::AIComponent(std::string&& scriptPath) : ts(this), scriptPath(scriptPath) {}

TreeState& AIComponent::getTreeState() {
    return ts;
}

void AIComponent::init() {
    auto& script = GetGameObject().AddComponent<ScriptComponent>();
    script.attachScript(scriptPath);
}
