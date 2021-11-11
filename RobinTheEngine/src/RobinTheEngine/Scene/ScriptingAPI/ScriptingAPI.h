#pragma once
#include <sol/sol.hpp>

// API provided for the following classes
#include "../Scene.h"
#include "../GameObject.h"
#include "../Serializer.h"
#include "../Components/Transform.h"
#include "../Components/MeshRenderer.h"

namespace RTE {

    template<typename UserType>
    inline void registerUserType(sol::state& lua) {
        static_assert("this function must be overriden for use with desired type" && false);
    }

    template<>
    inline void registerUserType<Scene>(sol::state& lua) {
        sol::usertype<Scene> ut = lua.new_usertype<Scene>("Scene", sol::constructors<Scene()>());
        ut["GetGameObject"] = &Scene::GetGameObject;
        ut["CreateGameObject"] = sol::overload(
            [](Scene& scene) {scene.CreateGameObject(); },
            [](Scene& scene, entt::entity id) {scene.CreateGameObject(id); }
        );
        ut["name"] = &Scene::name;
    }

    //TODO: finish
    template<>
    inline void registerUserType<GameObject>(sol::state& lua) {
        sol::usertype<GameObject> ut = lua.new_usertype<GameObject>("GameObject",
            sol::constructors<
            GameObject(),
            GameObject(const GameObject&),
            GameObject(entt::entity, Scene*)
            >()
            );
        ut["GetID"] = &GameObject::GetID;
        ut["GetTransform"] = &GameObject::GetTransform;
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
        */
    }

    template<>
    inline void registerUserType<DirectX::XMFLOAT3>(sol::state& lua) {
        using XMFLOAT3 = DirectX::XMFLOAT3;
        sol::usertype<XMFLOAT3> ut = lua.new_usertype<XMFLOAT3>("XMFLOAT3",
            sol::constructors<
            XMFLOAT3(),
            XMFLOAT3(const XMFLOAT3&),
            XMFLOAT3(XMFLOAT3&&),
            XMFLOAT3(float, float, float),
            XMFLOAT3(_In_reads_(3) const float*)
            >()
            );
        ut["x"] = &XMFLOAT3::x;
        ut["y"] = &XMFLOAT3::y;
        ut["z"] = &XMFLOAT3::z;
    }

    //TODO: finish (NO need probably)
    //template<>
    //void registerUserType<DirectX::XMMATRIX>(sol::state& lua) {
    //    using XMMATRIX = DirectX::XMMATRIX;
    //    sol::usertype<XMMATRIX> t = lua.new_usertype<XMMATRIX>("XMMATRIX");
    //}

    template<>
    inline void registerUserType<Component>(sol::state& lua) {
        sol::usertype<Component> ut = lua.new_usertype<Component>("Component", sol::constructors<
            Component(),
            Component(const Component&)
        >());
        ut["GetGameObject"] = &Component::GetGameObject;
    }

    template<>
    inline void registerUserType<Transform>(sol::state& lua) {
        using XMFLOAT3 = DirectX::XMFLOAT3;
        using XMMATRIX = DirectX::XMMATRIX;

        sol::usertype<Transform> ut = lua.new_usertype<Transform>("Transform",
            sol::constructors<
                Transform(),
                Transform(const Transform&),
                Transform(const XMFLOAT3, const XMFLOAT3, const XMFLOAT3),
                Transform(const XMFLOAT3, const XMFLOAT3, const XMFLOAT3, Transform&)
            >(),
            sol::base_classes, sol::bases<Component>()
            );
        ut["GetPosition"] = &Transform::GetPosition;
        ut["GetRotation"] = &Transform::GetRotation;
        ut["GetScale"] = &Transform::GetScale;
        ut["SetPosition"] = sol::overload(
            [](Transform& transform, float x, float y, float z) {transform.SetPosition(x, y, z); },
            [](Transform& transform, const XMFLOAT3& pos) {transform.SetPosition(pos); }
        );
        ut["SetRotation"] = sol::overload(
            [](Transform& transform, float x, float y, float z) {transform.SetRotation(x, y, z); },
            [](Transform& transform, const XMFLOAT3& rotation) {transform.SetRotation(rotation); }
        );
        ut["SetScale"] = sol::overload(
            [](Transform& transform, float x, float y, float z) {transform.SetScale(x, y, z); },
            [](Transform& transform, const XMFLOAT3& scale) {transform.SetScale(scale); }
        );
        ut["HasParent"] = &Transform::HasParent;
        ut["GetParent"] = &Transform::GetParent;
        ut["SetParent"] = &Transform::SetParent;
        ut["GetMatrix"] = &Transform::GetMatrix;
        ut["to_string"] = &Transform::to_string;
    }

    template<>
    inline void registerUserType<MeshRenderer>(sol::state& lua) {
        sol::usertype<MeshRenderer> ut = lua.new_usertype<MeshRenderer>("MeshRenderer",
            sol::constructors<
                MeshRenderer(const MeshRenderer&),
                MeshRenderer(const IMesh& mesh, const IMaterial& material)
            >(),
            sol::base_classes, sol::bases<Component>()
        );
        ut["GetMesh"] = &MeshRenderer::GetMesh;
        ut["GetMaterial"] = &MeshRenderer::GetMaterial;
        ut["SetMesh"] = &MeshRenderer::SetMesh;
        ut["SetMaterial"] = &MeshRenderer::SetMaterial;
    }

    template<>
    inline void registerUserType<Serializer>(sol::state& lua) {
        sol::usertype<Serializer> ut = lua.new_usertype<Serializer>("Serializer", sol::constructors<Serializer(Scene&)>());
        ut["Serialize"] = &Serializer::Serialize;
        ut["Deserialize"] = &Serializer::Deserialize;
    }

    inline void registerEngineAPI(sol::state& lua) {
        // we don't need these two probably
        //registerUserType<DirectX::XMFLOAT3>(lua);
        //registerUserType<DirectX::XMMATRIX>(lua);

        registerUserType<Scene>(lua);
        registerUserType<GameObject>(lua);
        registerUserType<Transform>(lua);
        registerUserType<MeshRenderer>(lua);
        registerUserType<Serializer>(lua);
    }

}

// Need to add JobSystem and ResourceManager API
// Fix inlining - users only need templated registerUserType function without any implemetation