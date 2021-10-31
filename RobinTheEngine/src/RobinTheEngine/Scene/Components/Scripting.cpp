#include "rtepch.h"
#include "Scripting.h"

using namespace RTE;

ScriptComponent::ScriptComponent() : 
    lua(), script(""), executable(), isFile(false), scriptAttached(false) 
{
    lua.open_libraries(sol::lib::base);
}

bool ScriptComponent::checkScript() {
    return scriptAttached;
}

void ScriptComponent::onAttachEnd(const std::string& script, bool isFile) {
    this->isFile = isFile;
    this->script = script;
    scriptAttached = true;
}

void ScriptComponent::onAttachStart(const std::string& script, bool isFile) {
    if (isFile) {
        executable = lua.load_file(script);
    }
    else {
        executable = lua.load(script);
    }
}

bool ScriptComponent::attachScript(const std::string& script, bool isFile) {
    // Note: script will be executed once on the load
    // to fill function table inside the sol2 library
    // if you want to execute the script manually use
    onAttachStart(script, isFile);
    if (executable.valid()) {
        onAttachEnd(script, isFile);
        call();
        return true;
    }
    return false;
}

bool RTE::ScriptComponent::attachScriptNoExec(const std::string& script, bool isFile) {
    // Note: script will be only loaded, but not executed, and,
    // therefore function table inside the sol2 library will not be filled
    // so, you have to execute the script manually before use
    onAttachStart(script, isFile);
    if (executable.valid()) {
        onAttachEnd(script, isFile);
        return true;
    }
    return false;
}

bool ScriptComponent::reloadScript() {
    if (scriptAttached) {
        return attachScript(script, isFile);
    }
}

sol::state& ScriptComponent::getState() {
    return lua;
}
