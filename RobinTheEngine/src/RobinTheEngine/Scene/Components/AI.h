#pragma once

#include "DetourCrowd.h"
#include "Scripting.h"
#include "../../AI/Navigation.h"
#include "../../AI/BehaviourTree.h"
#include "../../AI/EventExecutor.h"

namespace RTE {

    class AIComponent final: public Component, public EventExecutor {
    public:
        AIComponent() = default;
        AIComponent(std::string&& scriptPath);
        ~AIComponent();
        void onConstructInit(std::string&& scriptPath);
        void registerAgent(CrowdManager* cm);
        void setPerceptionManager(PerceptionManager* pm);
        void requestMove(DirectX::XMFLOAT3 pos);
        void onConsume(std::shared_ptr<Stimulus> stimulus) override;
        void onProduce(std::shared_ptr<Stimulus> stimulus);
        void onUpdate();
        int getAgentId();
        TreeState& getTreeState();
        void init(CrowdManager* cm, PerceptionManager* pm);
    private:
        CrowdManager* cm = nullptr;
        PerceptionManager* pm = nullptr;
        TreeState ts{};
        int agentId{};
        dtCrowdAgentParams agentParams{};
        std::string scriptPath{};

        dtNavMeshQuery* m_navQuery;
    };

}

