#pragma once
#include <unordered_set>
#include <utility>
#include <vector>
#include <queue>
#include <memory>
#include "EventListener.h"

namespace RTE {

    struct PerceptionManager final {
        PerceptionManager();
        void registerListener(EventListener* listener, std::vector<Stimulus::StimulusId> events);
        void addStimulus(std::shared_ptr<Stimulus> stimulus);
        void notify();
    private:
        std::vector<std::pair<EventListener*, std::unordered_set<Stimulus::StimulusId>>> toNotify;
        std::queue<std::shared_ptr<Stimulus>> stimulusQueue;
    };

}
