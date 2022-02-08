#include "rtepch.h"
#include <functional>
#include "PerceptionManager.h"

using namespace RTE;

Stimulus::Stimulus(const std::string& type) {
    id = std::hash<std::string>{}(type);
}

Stimulus::~Stimulus() = default;

bool Stimulus::operator==(const Stimulus& other) const {
    return id == other.id;
}

Stimulus::StimulusId Stimulus::getType() {
    return id;
}

EventListener::EventListener(RTE::GameObject* owner): owner(owner) {}
EventListener::~EventListener() = default;

PerceptionManager::PerceptionManager() = default;

void PerceptionManager::registerListener(EventListener* listener, std::vector<Stimulus::StimulusId> events) {
    toNotify.push_back({listener, std::unordered_set(events.begin(), events.end()) });
}

void PerceptionManager::addStimulus(std::shared_ptr<Stimulus> stimulus) {
    stimulusQueue.push(stimulus);
}

void PerceptionManager::notify() {
    while(!stimulusQueue.empty()) {
        auto stimulus = stimulusQueue.front();
        stimulusQueue.pop();
        for(auto & [listener, usefulStimuls] : toNotify) {
            if(usefulStimuls.contains(stimulus->getType())) {
                listener->onNotify(stimulus);
            }
        }
    }
}

void PerceptionManager::notify(std::shared_ptr<Stimulus> stimulus) {
    stimulusQueue.push(stimulus);
    notify();
}
