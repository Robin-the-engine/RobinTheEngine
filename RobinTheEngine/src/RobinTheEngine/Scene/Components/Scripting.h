#pragma once

#include <string>
#include <sol/sol.hpp>
#include "../Component.h"

namespace RTE {

    class ScriptComponent {
    public:
        virtual ~ScriptComponent() = default;
        virtual void attachScript(const std::string& script, bool isFile = true);
        virtual void reloadScript();
        virtual sol::state& getState();

        template<typename ...Params>
        void call(Params ...) {
            if (scriptAttached) {
                executable(Params ...);
            }
        }

        template<>
        void call(float x, float y, float z) {
            if (scriptAttached) {
                executable(x, y, z);
            }
        }

    protected:
        ScriptComponent();

    private:
        sol::state lua;
        const std::string& script;
        sol::load_result executable;
        bool isFile = true;
        bool scriptAttached = false;
    };

    template<typename UserType>
    void registerUserType() {
        assert("you must create specializetion of this template for the type you need" && false);
    }

}