#include "rtepch.h"
#include "AI.h"
#include "../../Scene/GameObject.h"
#include "RobinTheEngine/AI/PerceptionManager.h"
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

void AIComponent::registerAgent(CrowdManager* cm) {
    const auto pos = GetGameObject().GetComponent<Transform>().GetPosition();
    agentId = cm->addAgent(&pos.x, &agentParams);
    this->cm = cm;
}

void AIComponent::setPerceptionManager(PerceptionManager* pm) {
    this->pm = pm;
}

void AIComponent::requestMove(DirectX::XMFLOAT3 pos) {
    cm->move(agentId, 0, &pos.x);
}

void AIComponent::onConsume(std::shared_ptr<Stimulus> stimulus) {
    auto& script = GetGameObject().AddComponent<ScriptComponent>();
    script.callf("onEvent", stimulus.get());
}

void AIComponent::onProduce(std::shared_ptr<Stimulus> stimulus) {
    EventExecutor::onProduce(pm, stimulus);
}

int AIComponent::getAgentId() {
    return agentId;
}

TreeState& AIComponent::getTreeState() {
    return ts;
}

void AIComponent::init() {
    auto& script = GetGameObject().AddComponent<ScriptComponent>();
    script.attachScript(scriptPath);
}
