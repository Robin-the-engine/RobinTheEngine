#pragma once
#include <sol/sol.hpp>
#include <string>
#include "../GameObject.h"
#include "../../ResourceFactory.h"

namespace RTE {

    template<typename UserType>
    void registerUserType(sol::state& lua) {
        static_assert("this function must be overriden for use with desired type" && false);
    }

    template<typename Component>
    void registerUserComponent(sol::state& lua, const char* const ComponentName) {
        auto GameObjectTable = lua["GameObject"];
        assert(GameObjectTable.valid() && "GameObject doesn't registered in current lua state, can't attach Component to it");
        lua["GetComponent_" + std::string(ComponentName)] = &GameObject::template GetComponent<Component>;
        lua["HasComponent_" + std::string(ComponentName)] = &GameObject::template HasComponent<Component>;
        lua["AddComponent_" + std::string(ComponentName)] = &GameObject::template AddComponent<Component>;
        lua["RemoveComponent_" + std::string(ComponentName)] = &GameObject::template RemoveComponent<Component>;
    }

    template<typename Resource>
    void registerUserResource(sol::state& lua, const char* const ResourceName) {
        auto GameObjectTable = lua["GameObject"];
        assert(GameObjectTable.valid() && "GameObject doesn't registered in current lua state, can't attach Component to it");
        lua["GetResource" + std::string(ResourceName)] = &ResourceFactory::template GetResource<Resource>;
    }

    void registerEngineAPI(sol::state& lua);
}
