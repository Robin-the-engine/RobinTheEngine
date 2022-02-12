#pragma once
#include <string>
#include "Platform/DirectX11/Camera.h"
#include <memory>

namespace RTE {
    struct Sound;
    class GameObject;
    struct PerceptionManager;

    struct Stimulus {

        using StimulusId = size_t;

        virtual ~Stimulus();
        bool operator==(const Stimulus& other) const;

        virtual StimulusId getType();

    protected:
        Stimulus() = default;
        Stimulus(const std::string& type);
        StimulusId id;
    };

    struct EventExecutor {
        virtual ~EventExecutor();
        virtual void onConsume(std::shared_ptr<Stimulus> stimulus);
        virtual void onProduce(PerceptionManager* pm, std::shared_ptr<Stimulus> stimulus);
    protected:
        EventExecutor() = default;
    };

    struct Sound final : Stimulus {
        Sound() : Stimulus("Sound"), pos{} {};
        Sound(DirectX::XMFLOAT3 p) : Stimulus("Sound"), pos(p) {}
        DirectX::XMFLOAT3 getPos();
    private:
        DirectX::XMFLOAT3 pos;
    };

}

std::shared_ptr<RTE::Sound> getStimulusAsSound(std::shared_ptr<RTE::Stimulus> pStimulus);
std::ostream& operator<<(std::ostream& out, DirectX::XMFLOAT3 f3);
