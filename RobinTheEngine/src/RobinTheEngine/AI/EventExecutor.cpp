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

std::shared_ptr<Sound> getStimulusAsSound(std::shared_ptr<Stimulus> pStimulus) {
    assert(pStimulus->getType() == Sound().getType() && "Bad pointer to Sound");
    auto casted = dynamic_pointer_cast<Sound>(pStimulus);
    return casted;
}

std::ostream& operator<<(std::ostream& out, DirectX::XMFLOAT3 f3) {
    out << '{' << f3.x << ", " << f3.y << ", " << f3.z << '}';
    return out;
}

