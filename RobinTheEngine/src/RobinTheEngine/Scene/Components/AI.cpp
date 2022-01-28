#include "rtepch.h"
#include "AI.h"
#include "../../Scene/GameObject.h"
using namespace RTE;

AIComponent::AIComponent(std::string&& scriptPath) {
    onConstructInit(std::move(scriptPath));
}

void AIComponent::onConstructInit(std::string&& scriptPath) {
    this->scriptPath = scriptPath;
    ts.owner = this;
}

TreeState& AIComponent::getTreeState() {
    return ts;
}

void AIComponent::init() {
    auto& script = GetGameObject().AddComponent<ScriptComponent>();
    script.attachScript(scriptPath);
}
