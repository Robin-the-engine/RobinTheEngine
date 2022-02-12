#include "rtepch.h"
#include "Navigation.h"

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
    crowd->init(agents, agentRadius, getNavMesh());
	dtObstacleAvoidanceParams avoidanceParams;

    // Low (11)
	avoidanceParams.velBias = 0.5f;
	avoidanceParams.adaptiveDivs = 5;
	avoidanceParams.adaptiveRings = 2;
	avoidanceParams.adaptiveDepth = 1;
	crowd->setObstacleAvoidanceParams(0, &avoidanceParams);

	// Medium (22)
	avoidanceParams.velBias = 0.5f;
	avoidanceParams.adaptiveDivs = 5;
	avoidanceParams.adaptiveRings = 2;
	avoidanceParams.adaptiveDepth = 2;
	crowd->setObstacleAvoidanceParams(1, &avoidanceParams);

	// Good (45)
	avoidanceParams.velBias = 0.5f;
	avoidanceParams.adaptiveDivs = 7;
	avoidanceParams.adaptiveRings = 2;
	avoidanceParams.adaptiveDepth = 3;
	crowd->setObstacleAvoidanceParams(2, &avoidanceParams);

	// High (66)
	avoidanceParams.velBias = 0.5f;
	avoidanceParams.adaptiveDivs = 7;
	avoidanceParams.adaptiveRings = 3;
	avoidanceParams.adaptiveDepth = 3;

	crowd->setObstacleAvoidanceParams(3, &avoidanceParams);
}

int CrowdManager::addAgent(const float* pos, const dtCrowdAgentParams* params) {
    return crowd->addAgent(pos, params);
}

const dtCrowdAgent* CrowdManager::getAgent(int idx) {
    return crowd->getAgent(idx);
}

const dtQueryFilter* CrowdManager::getFilter(int i) {
    return crowd->getFilter(i);
}

dtNavMesh* CrowdManager::getNavMesh() {
    return navMesh.get();
}

const float* CrowdManager::getQueryExtents() {
    return crowd->getQueryExtents();
}

bool CrowdManager::move(int agentIdx, dtPolyRef ref, const float* pos) {
    return crowd->requestMoveTarget(agentIdx, ref, pos);
}

void CrowdManager::update(float dt) {
    crowd->update(dt, nullptr);
}
