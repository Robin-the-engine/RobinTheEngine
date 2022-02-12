#pragma once

#include "../Scene/Components/NavMeshComponent.h"
#include "DetourCrowd.h"

namespace RTE {

    class Steerings {
        dtCrowdAgent dtca;
    };


    class CrowdManager {

    public:

        CrowdManager() = default;
        CrowdManager(const std::string& navMeshKey, int agents = 1, float agentRadius = 5);
        ~CrowdManager();

        void init(const std::string& navMeshKey, int agents, float agentRadius);
        int  addAgent(const float* pos, const dtCrowdAgentParams* params);
        const dtCrowdAgent* getAgent(int idx);
        const dtQueryFilter* getFilter(int i);
        dtNavMesh* getNavMesh();
        const float* getQueryExtents();
        bool move(int agentIdx, dtPolyRef ref, const float* pos);
        void update(float dt);

    private:
        dtCrowd* crowd = nullptr;
        NavMeshComponent navMesh;
    };

}
