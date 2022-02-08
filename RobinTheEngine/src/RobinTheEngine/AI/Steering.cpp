#include "rtepch.h"
#include "Steering.h"

using namespace RTE;

CrowdManager::CrowdManager(const std::string& navMeshKey, int agents, float agentRadius) {
    init(navMeshKey, agents, agentRadius);
}

CrowdManager::~CrowdManager() {
    if (crowd) {
        dtFreeCrowd(crowd);
    }
}

void CrowdManager::init(const std::string& navMeshKey, int agents, float agentRadius) {
    crowd = dtAllocCrowd();
    navMesh.init(navMeshKey);
    crowd->init(agents, agentRadius, navMesh.get());
    crowd->setObstacleAvoidanceParams(1, &avoidanceParams);
}

void CrowdManager::addAgent(const float* pos, const dtCrowdAgentParams* params) {
    crowd->addAgent(pos, params);
}

int CrowdManager::getActiveAgents(dtCrowdAgent** agents, const int maxAgents) {
    return crowd->getActiveAgents(agents, maxAgents);
}

bool CrowdManager::move(int agentIdx, dtPolyRef ref, const float* pos) {
    return crowd->requestMoveTarget(agentIdx, ref, pos);
}

void CrowdManager::update(float dt) {
    crowd->update(dt, nullptr);
}
