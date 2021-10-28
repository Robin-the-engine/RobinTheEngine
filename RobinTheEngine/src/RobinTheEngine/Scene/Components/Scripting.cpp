#include "rtepch.h"
#include "Scripting.h"

using namespace RTE;

ScriptComponent::ScriptComponent() : 
    lua(), script(""), executable(), isFile(false), scriptAttached(false) 
{
    lua.open_libraries(sol::lib::base);
}

void ScriptComponent::attachScript(const std::string& script, bool isFile) {
    if (isFile) {
        executable = lua.load_file(script);
    }
    else {
        executable = lua.load(script);
    }
    if (executable.valid()) {
        scriptAttached = true;
    }
}

void ScriptComponent::reloadScript() {
    attachScript(script, isFile);
}

sol::state& ScriptComponent::getState() {
    return lua;
}
