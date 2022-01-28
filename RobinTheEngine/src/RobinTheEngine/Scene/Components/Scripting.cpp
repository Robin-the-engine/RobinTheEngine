#include "rtepch.h"
#include "Scripting.h"
#include "../ScriptingAPI/ScriptingAPI.h"
#include <format>
using namespace RTE;

ScriptComponent::ScriptComponent():
    lua(), script(""), executable(), isFile(false), scriptAttached(false) {
    lua.open_libraries(
        sol::lib::os,
        sol::lib::base,
        sol::lib::math,
        sol::lib::table,
        sol::lib::utf8
    );
    registerEngineAPI(lua);
}

bool ScriptComponent::checkScript() {
    return scriptAttached;
}

void ScriptComponent::onScriptAttachEnd(const std::string& script, bool isFile) {
    this->isFile = isFile;
    this->script = script;
    scriptAttached = true;
}

sol::protected_function_result ScriptComponent::execute(const std::string& code) {
    auto res = lua.script(code);
    if(!res.valid()) {
        logger->error(
            std::format("Error in script:\n{}\nStatus: {}", code, to_string(res.status()))
        );
    }
    return res;
}

void ScriptComponent::OnAttach() {
    callf("OnAttach");
}

void ScriptComponent::OnDetach() {
    callf("OnDetach");
}

void ScriptComponent::OnUpdate() {
    callf("OnUpdate");
}

void ScriptComponent::OnEvent(Event& event) {
    callf<Event&>("OnEvent", event);
}

void ScriptComponent::OnImGuiRender() {
    callf("OnImGuiRender");
}

void ScriptComponent::OnRender() {
    callf("OnRender");
}

void ScriptComponent::onScriptAttachStart(const std::string& script, bool isFile) {
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
    onScriptAttachStart(script, isFile);
    if (executable.valid()) {
        onScriptAttachEnd(script, isFile);
        executable();
        return true;
    }
    logger->error(std::format("script: {} is not attached. ERR_STATUS: {}", script, to_string(executable.status())));
    return false;
}

bool RTE::ScriptComponent::attachScriptNoExec(const std::string& script, bool isFile) {
    // Note: script will be only loaded, but not executed, and,
    // therefore function table inside the sol2 library will not be filled
    // so, you have to execute the script manually before use
    onScriptAttachStart(script, isFile);
    if (executable.valid()) {
        onScriptAttachEnd(script, isFile);
        return true;
    }
    logger->error(std::format("script: {} is not attached. ERR_STATUS: {}", script, to_string(executable.status())));
    return false;
}

bool ScriptComponent::reloadScript() {
    if (scriptAttached) {
        return attachScript(script, isFile);
    }
    return false;
}

sol::state& ScriptComponent::getState() {
    return lua;
}
