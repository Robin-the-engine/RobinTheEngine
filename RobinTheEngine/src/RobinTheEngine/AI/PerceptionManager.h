#pragma once
#include <unordered_set>
#include <string>
#include <utility>
#include <vector>
#include <queue>
#include <memory>
#include "Platform/DirectX11/Camera.h"

namespace RTE {

    class GameObject;

    struct Stimulus {

        using StimulusId = size_t;

        virtual ~Stimulus();
        bool operator==(const Stimulus& other) const;

        template <typename T>
        T* getAs() {
            return dynamic_cast<T*>(this);
        }

        virtual StimulusId getType();

    protected:
        Stimulus(const std::string& type);
        StimulusId id;
    };

    struct EventListener {
        EventListener(RTE::GameObject* owner);
        virtual ~EventListener();
        virtual void onNotify(std::shared_ptr<Stimulus> stimulus) = 0;
    private:
        RTE::GameObject* owner;
    };

    struct PerceptionManager final {
        PerceptionManager();
        void registerListener(EventListener* listener, std::vector<Stimulus::StimulusId> events);
        void addStimulus(std::shared_ptr<Stimulus> stimulus);
        void notify();
        void notify(std::shared_ptr<Stimulus> stimulus);
    private:
        std::vector<std::pair<EventListener*, std::unordered_set<Stimulus::StimulusId>>> toNotify;
        std::queue<std::shared_ptr<Stimulus>> stimulusQueue;
    };

    struct Sound final : Stimulus {
        Sound(DirectX::XMFLOAT3 p): Stimulus("Sound"), pos(p) {}
    private:
        DirectX::XMFLOAT3 pos;
    };

}
