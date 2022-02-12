#include "rtepch.h"
#include <functional>
#include "PerceptionManager.h"

using namespace RTE;

PerceptionManager::PerceptionManager() = default;

void PerceptionManager::registerListener(EventExecutor* listener, std::vector<Stimulus::StimulusId> events) {
    toNotify.push_back({listener, std::unordered_set(events.begin(), events.end()) });
}

void PerceptionManager::addStimulus(std::shared_ptr<Stimulus> stimulus) {
    stimulusQueue.push(stimulus);
}

void PerceptionManager::notify() {
    while(!stimulusQueue.empty()) {
        auto stimulus = stimulusQueue.front();
        auto type = stimulus->getType();
        stimulusQueue.pop();
        for(auto & [listener, usefulStimuls] : toNotify) {
            if(usefulStimuls.contains(type)) {
                listener->onConsume(stimulus);
            }
        }
    }
}
