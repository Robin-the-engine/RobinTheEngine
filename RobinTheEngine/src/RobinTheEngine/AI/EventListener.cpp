#include "rtepch.h"
#include "EventListener.h"

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

EventListener::EventListener() = default;

void EventListener::onNotify(std::shared_ptr<Stimulus> stimulus) const {
    notifyFunc(stimulus);
}

void EventListener::setNotifyer(notifyer&& nf) {
    notifyFunc = std::move(nf);
}
EventListener::~EventListener() = default;
