#pragma once

#include "DetourCrowd.h"
#include "Scripting.h"
#include "../../AI/Steering.h"
#include "../../AI/BehaviourTree.h"
#include "../../AI/EventListener.h"

namespace RTE {

    class AIComponent: public Component {
    public:
        AIComponent() = default;
        AIComponent(std::string&& scriptPath);
        void onConstructInit(std::string&& scriptPath);
        void setEventReaction(EventListener::notifyer&& nf);
        void registerAgent(CrowdManager* cm);
        TreeState& getTreeState();
        void init();
    private:
        EventListener ev;
        TreeState ts;
        int agentID;
        dtCrowdAgentParams agentParams;
        std::string scriptPath;
    };

}

