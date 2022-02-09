#pragma once
#include <string>
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

    struct EventListener final {
        using notifyer = std::function<void(std::shared_ptr<Stimulus>)>;
        EventListener();
        void setNotifyer(notifyer&& nf);
        ~EventListener();
        void onNotify(std::shared_ptr<Stimulus> stimulus) const;
    private:
        notifyer notifyFunc = [](std::shared_ptr<Stimulus>){};
    };


    struct Sound final : Stimulus {
        Sound(DirectX::XMFLOAT3 p) : Stimulus("Sound"), pos(p) {}
    private:
        DirectX::XMFLOAT3 pos;
    };


}
