#pragma once

#include <string>
#include <sol/sol.hpp>
#include "../Component.h"

namespace RTE {

    class ScriptComponent {
    public:
        virtual ~ScriptComponent() = default;
        virtual bool attachScript(const std::string& script, bool isFile = true);
        virtual bool attachScriptNoExec(const std::string& script, bool isFile = true);
        virtual bool reloadScript();
        virtual sol::state& getState();

        template<typename ...Params>
        void call(Params ...args) {
            if (!checkScript()) {
                return;
            }
            executable(std::forward<Params>(args)...);
        }

        template<typename ...Params>
        void callf(std::string func, Params ...args) {
            if (!checkScript()) {
                return;
            }
            lua[func](std::forward<Params>(args)...);
        }

    protected:
        ScriptComponent();
        virtual bool checkScript();
        virtual void onAttachEnd(const std::string& script, bool isFile = true);
        virtual void onAttachStart(const std::string& script, bool isFile = true);

    private:
        sol::state lua;
        std::string script;
        sol::load_result executable;
        bool isFile = true;
        bool scriptAttached = false;
    };

}