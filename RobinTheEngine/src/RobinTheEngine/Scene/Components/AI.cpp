#include "rtepch.h"
#include "AI.h"
#include "../../Scene/GameObject.h"
#include "RobinTheEngine/AI/PerceptionManager.h"
#include "RobinTheEngine/Log.h"
#include <format>
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
    //auto agentId = cm->getAgent(agentId; >
    //navquery->findNearestPoly(p, halfExtents, filter, &m_targetRef, m_targetPos);
    cm->move(agentId, 0, &pos.x);
}

void AIComponent::onConsume(std::shared_ptr<Stimulus> stimulus) {
    auto& script = GetGameObject().GetComponent<ScriptComponent>();
    script.callf("onEvent", stimulus);
}

void AIComponent::onProduce(std::shared_ptr<Stimulus> stimulus) {
    EventExecutor::onProduce(pm, stimulus);
}

void AIComponent::onUpdate() {
    auto pos = cm->getAgent(agentId)->npos;
    Log::GetLogger("ai")->info(std::format("Current pos: {} {} {}", pos[0], pos[1], pos[2]));
    GetGameObject().GetTransform().SetPosition(pos[0], pos[1], pos[2]);
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
    auto& lua = script.getStateRef();
    // register agent movement function
    lua["selfAI"] = this;
}
