#include "rtepch.h"
#include "AI.h"
#include "../../Scene/GameObject.h"
using namespace RTE;

AIComponent::AIComponent(std::string&& scriptPath) {
    onConstructInit(std::move(scriptPath));
}

void AIComponent::onConstructInit(std::string&& scriptPath) {
    this->scriptPath = std::move(scriptPath);
    ts.owner = this;
    agentParams.radius = 5.0;
    agentParams.collisionQueryRange = 0.5;
    agentParams.height = 5.0;
    agentParams.maxAcceleration = 100;
}

void AIComponent::setEventReaction(EventListener::notifyer&& nf) {
    ev.setNotifyer(std::move(nf));
}

void AIComponent::registerAgent(CrowdManager* cm) {
    const auto pos = GetGameObject().GetComponent<Transform>().GetPosition();
    agentID = cm->addAgent(&pos.x, &agentParams);
}

TreeState& AIComponent::getTreeState() {
    return ts;
}

void AIComponent::init() {
    auto& script = GetGameObject().AddComponent<ScriptComponent>();
    script.attachScript(scriptPath);
}
