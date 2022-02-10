#include "rtepch.h"
#include "EventExecutor.h"
#include "PerceptionManager.h"
using namespace RTE;

Stimulus::Stimulus(const std::string& type) {
    id = std::hash<std::string>{}(type);
}

Stimulus::~Stimulus() = default;

bool Stimulus::operator==(const Stimulus & other) const {
    return id == other.id;
}

Stimulus::StimulusId Stimulus::getType() {
    return id;
}

DirectX::XMFLOAT3 Sound::getPos() {
    return pos;
}

EventExecutor::~EventExecutor() = default;

void EventExecutor::onConsume(std::shared_ptr<Stimulus> stimulus) {}

void EventExecutor::onProduce(PerceptionManager* pm, std::shared_ptr<Stimulus> stimulus) {
    pm->addStimulus(stimulus);
}
