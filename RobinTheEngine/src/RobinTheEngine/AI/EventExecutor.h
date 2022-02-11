#pragma once
#include <string>
#include "Platform/DirectX11/Camera.h"

namespace RTE {
    class GameObject;
    struct PerceptionManager;

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
        Sound() = default;
        Sound(DirectX::XMFLOAT3 p) : Stimulus("Sound"), pos(p) {}
        DirectX::XMFLOAT3 getPos();
    private:
        DirectX::XMFLOAT3 pos;
    };


}
