#pragma once

#include <string>
#include <sol/sol.hpp>
#include "../Component.h"
#include "../../Events/Event.h"

namespace RTE {

    template<typename T>
    concept NONPRIMITIVE = !std::is_fundamental_v<std::remove_pointer_t<std::decay_t<T>>>;

    class ScriptComponent: public Component {
    public:
        ScriptComponent();
        ScriptComponent(const ScriptComponent&) = delete;
        ScriptComponent(ScriptComponent&&) noexcept = default;
        ScriptComponent& operator=(const ScriptComponent& other) = delete;
        ScriptComponent& operator=(ScriptComponent&& other) noexcept = default;
        virtual ~ScriptComponent() = default;

        virtual bool attachScript(const std::string& script, bool isFile = true);
        virtual bool attachScriptNoExec(const std::string& script, bool isFile = true);
        virtual bool reloadScript();
        virtual sol::state& getState();

        // sol can't use references to primitives
        template<NONPRIMITIVE T>
        void setref(std::string name, T& value) {
            lua[name] = std::ref(value);
        }

        template<typename T>
        void setcopy(std::string name, T& value) {
            static_assert(!std::is_pointer_v<T> && "Can't use pointer in this function");
            lua[name] = value;
        }

        template<typename ...Params>
        void callf(std::string func, Params&& ...args) {
            if (!checkScript()) {
                return;
            }
            lua[func](std::forward<Params>(args)...);
        }

        // API for RTE::Layer
        virtual void OnAttach();
        virtual void OnDetach();
        virtual void OnUpdate();
        virtual void OnEvent(Event& event);
        virtual void OnImGuiRender();
        virtual void OnRender();
        std::string GetScript() { return script; }
    protected:
        virtual bool checkScript();
        virtual void onScriptAttachStart(const std::string& script, bool isFile = true);
        virtual void onScriptAttachEnd(const std::string& script, bool isFile = true);

    private:
        sol::state lua;
        std::string script;
        sol::load_result executable;
        bool isFile = true;
        bool scriptAttached = false;
    };

}