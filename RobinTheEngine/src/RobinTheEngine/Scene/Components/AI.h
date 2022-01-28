#pragma once

#include "Scripting.h"
#include "../../AI/Steering.h"
#include "../../AI/BehaviourTree.h"

namespace RTE {

    class AIComponent: public Component {
    public:
        AIComponent() = default;
        AIComponent(std::string&& scriptPath);
        void onConstructInit(std::string&& scriptPath);
        TreeState& getTreeState();
        void init();
    private:
        TreeState ts;
        std::string scriptPath;
    };

}

