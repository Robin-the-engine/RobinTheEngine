#include <sol/sol.hpp>

// API provided for the following classes
#include "Scene.h"
#include "GameObject.h"
#include "Components.h"

namespace RTE {

    template<typename UserType>
    void registerUserType(sol::state& lua);

    template<>
    void registerUserType<Scene>(sol::state& lua) {
        sol::usertype<Scene> scene = lua.new_usertype<Scene>("Scene", sol::constructors<Scene()>());
        scene["GetGameObject"] = &Scene::GetGameObject;
        scene["CreateGameObject"] = sol::overload(
            [](Scene& scene) {scene.CreateGameObject();},
            [](Scene& scene, entt::entity id) {scene.CreateGameObject(id);}
        );
        scene["name"] = &Scene::name;
    }

    //TODO: finish
    template<>
    void registerUserType<GameObject>(sol::state& lua) {
        sol::usertype<GameObject> go = lua.new_usertype<GameObject>("GameObject",
            sol::constructors<
                GameObject(),
                GameObject(const GameObject&),
                GameObject(entt::entity, Scene*)
            >()
        );
        go["GetID"] = &GameObject::GetID;
        go["GetTransform"] = &GameObject::GetTransform;
       /*
        template<typename T>
        bool HasComponent()
        {
            return !scene->registry.orphan<T>(entity);
        }

        template<typename T>
        T& GetComponent() const
        {
            return scene->registry.get<T>(entity);
        }

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            T& comp = scene->registry.emplace<T>(entity, std::forward<Args>(args)...);
            comp.scene = scene;
            comp.owner = entity;
            return comp;
        }

        template<typename T>
        void RemoveComponent()
        {
            scene->registry.remove<T>(entity);
        }

        // Template specialization for Transform, because it's default
        template<>
        Transform& AddComponent<Transform>()
        {
            return GetTransform();
        }
        template<>
        void RemoveComponent<Transform>()
        {
            RTE_CORE_ERROR("You can't remove Transform component");
        }

        operator bool() const;
        operator entt::entity() const;

        bool operator==(const GameObject& other) const;
        bool operator!=(const GameObject& other) const;

       */
    }

    //TODO: finish (NO need probably)
    template<>
    void registerUserType<DirectX::XMFLOAT3>(sol::state& lua) {
        using XMFLOAT3 = DirectX::XMFLOAT3;
        sol::usertype<XMFLOAT3> t = lua.new_usertype<XMFLOAT3>("XMFLOAT3",
            sol::constructors<
                XMFLOAT3(),
                XMFLOAT3(const XMFLOAT3&),
                XMFLOAT3(XMFLOAT3&&),
                XMFLOAT3(float, float, float),
                XMFLOAT3(_In_reads_(3) const float*)
            >()
        );
        t["x"] = &XMFLOAT3::x;
        t["y"] = &XMFLOAT3::y;
        t["z"] = &XMFLOAT3::z;

        /*
        XMFLOAT3& operator=(const XMFLOAT3&) = default;
        XMFLOAT3& operator=(XMFLOAT3&&) = default;
        */
    }

    //TODO: finish (NO need probably)
    //template<>
    //void registerUserType<DirectX::XMMATRIX>(sol::state& lua) {
    //    using XMMATRIX = DirectX::XMMATRIX;
    //    sol::usertype<XMMATRIX> t = lua.new_usertype<XMMATRIX>("XMMATRIX");
    //}

    //TODO: finish
    template<>
    void registerUserType<Transform>(sol::state& lua) {
        using XMFLOAT3 = DirectX::XMFLOAT3;
        using XMMATRIX = DirectX::XMMATRIX;

        sol::usertype<Transform> t = lua.new_usertype<Transform>("Transform",
            sol::constructors<
                Transform(),
                Transform(const Transform&),
                Transform(const XMFLOAT3, const XMFLOAT3, const XMFLOAT3),
                Transform(const XMFLOAT3, const XMFLOAT3, const XMFLOAT3, Transform&)
            >()
        );
        t["GetPosition"] = &Transform::GetPosition;
        t["GetRotation"] = &Transform::GetRotation;
        t["GetScale"] = &Transform::GetScale;
        t["SetPosition"] = sol::overload(
            [](Transform& transform, float x, float y, float z) {transform.SetPosition(x, y, z); },
            [](Transform& transform, const XMFLOAT3& pos) {transform.SetPosition(pos); }
        );
        t["SetRotation"] = sol::overload(
            [](Transform& transform, float x, float y, float z) {transform.SetRotation(x, y, z); },
            [](Transform& transform, const XMFLOAT3& rotation) {transform.SetRotation(rotation); }
        );
        t["SetScale"] = sol::overload(
            [](Transform& transform, float x, float y, float z) {transform.SetScale(x, y, z); },
            [](Transform& transform, const XMFLOAT3& scale) {transform.SetScale(scale); }
        );
        t["HasParent"] = &Transform::HasParent;
        t["GetParent"] = &Transform::GetParent;
        t["SetParent"] = &Transform::SetParent;
        t["GetMatrix"] = &Transform::GetMatrix;
        t["to_string"] = &Transform::to_string;

        /*
		std::ostream& operator << (std::ostream& outs)
		{
			return outs << to_string();
		}
        */
    }

    void registerEngineAPI(sol::state& lua) {
        registerUserType<Scene>(lua);
        registerUserType<GameObject>(lua);
        registerUserType<DirectX::XMFLOAT3>(lua);
        //registerUserType<DirectX::XMMATRIX>(lua);
        registerUserType<Transform>(lua);
    }

}