#pragma once
#include "ScriptingAPI.h"

#include <sol/sol.hpp>

// API provided for the following classes
#include "../Scene.h"
#include "../GameObject.h"
#include "../Serializer.h"
#include "../Components.h"
#include "../../JobSystem/JobSystem.h"
#include "../../JobSystem/JobPriority.h"
#include "RobinTheEngine/GameTimer.h"
#include "Platform/DirectX11/Camera.h"
#include "../../Log.h"
#include "../../ResourceFactory.h"
#include "../../AI/TickResult.h"
#include "../../AI/PerceptionManager.h"
#include "../../AI/Steering.h"

namespace RTE {

    ///
    /// ---------- Main engine classes
    ///

    template<>
    void registerUserType<Camera>(sol::state& lua) {
        sol::usertype<Camera> ut = lua.new_usertype<Camera>("Camera", sol::constructors<Camera()>());
        ut["SetProjectionProperties"] = &Camera::SetProjectionProperties;
        ut["GetViewMatrix"] = &Camera::GetViewMatrix;
        ut["GetProjectionMatrix"] = &Camera::GetProjectionMatrix;
        ut["GetPositionVec"] = &Camera::GetPositionVec;
        ut["GetRotationVec"] = &Camera::GetRotationVec;
        ut["GetPositionFloat3"] = &Camera::GetPositionFloat3;
        ut["GetRotationFloat3"] = &Camera::GetRotationFloat3;
        ut["SetPosition"] = sol::overload(
            [](Camera& camera, const XMVECTOR& vec) {camera.SetPosition(vec); },
            [](Camera& camera, const XMFLOAT3& vec) {camera.SetPosition(vec); }
        );
        ut["AdjustPosition"] = sol::overload(
            [](Camera& camera, const XMVECTOR& vec) {camera.AdjustPosition(vec); },
            [](Camera& camera, const XMFLOAT3& vec) {camera.AdjustPosition(vec); }
        );
        ut["SetRotation"] = sol::overload(
            [](Camera& camera, const XMVECTOR& vec) {camera.SetRotation(vec); },
            [](Camera& camera, const XMFLOAT3& vec) {camera.SetRotation(vec); }
        );
        ut["AdjustRotation"] = sol::overload(
            [](Camera& camera, const XMVECTOR& vec) {camera.AdjustRotation(vec); },
            [](Camera& camera, const XMFLOAT3& vec) {camera.AdjustRotation(vec); }
        );
        ut["GetForwardVector"] = &Camera::GetForwardVector;
        ut["GetBackwardVector"] = &Camera::GetBackwardVector;
        ut["GetRightVector"] = &Camera::GetRightVector;
        ut["GetLeftVector"] = &Camera::GetLeftVector;
    }

    template<>
    void registerUserType<GameTimer>(sol::state& lua) {
        sol::usertype<GameTimer> ut = lua.new_usertype<GameTimer>("GameTimer", sol::constructors<GameTimer()>());
        ut["TotalTime"] = &GameTimer::TotalTime;
        ut["DeltaTime"] = &GameTimer::DeltaTime;
        ut["Reset"] = &GameTimer::Reset;
        ut["Start"] = &GameTimer::Start;
        ut["Stop"] = &GameTimer::Stop;
        ut["Tick"] = &GameTimer::Tick;
    }

    template<>
    void registerUserType<Serializer>(sol::state& lua) {
        sol::usertype<Serializer> ut = lua.new_usertype<Serializer>("Serializer", sol::constructors<Serializer(Scene&)>());
        ut["Serialize"] = &Serializer::Serialize;
        ut["Deserialize"] = &Serializer::Deserialize;
    }

    template<>
    void registerUserType<Log>(sol::state& lua) {
        sol::usertype<Log> ut = lua.new_usertype<Log>("Log");
        auto logger = Log::GetLogger("lua");
        ut["trace"] = [logger = logger](std::string msg) {logger->trace(msg); };
        ut["info"]  = [logger = logger](std::string msg) {logger->info(msg); };
        ut["warn"]  = [logger = logger](std::string msg) {logger->warn(msg); };
        ut["error"] = [logger = logger](std::string msg) {logger->error(msg); };
        ut["fatal"] = [logger = logger](std::string msg) {logger->critical(msg); };
    }

    template<>
    void registerUserType<DirectX::XMFLOAT3>(sol::state& lua) {
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

    ///
    /// ---------- Main engine classes finished
    ///

    ///
    /// ---------- JobSystem related stuff
    ///

    template<>
    void registerUserType<JobPriority>(sol::state& lua) {
        lua.new_enum("JobPriority",
            "LOWEST", JobPriority::LOWEST,
            "LOW", JobPriority::LOW,
            "MEDIUM", JobPriority::MEDIUM,
            "HIGH", JobPriority::HIGH,
            "HIGHEST", JobPriority::HIGHEST
        );
    }

    template<>
    void registerUserType<JobSystem>(sol::state& lua) {
        sol::usertype<JobSystem> ut = lua.new_usertype<JobSystem>("JobSystem");
        ut["GetJobSystem"] = &JobSystem::GetJobSystem;
        ut["getThreadCount"] = &JobSystem::getThreadCount;
        ut["isJobDone"] = &JobSystem::isJobDone;
        ut["waitForJob"] = &JobSystem::waitForJob;
        ut["kickJob"] = sol::overload(
            [](JobSystem& js, sol::function function)
            { return js.kickJob(function, JobPriority::HIGH, -1); },
            [](JobSystem& js, sol::function function, JobPriority priority)
            { return js.kickJob(function, priority, -1); },
            [](JobSystem& js, sol::function function, JobPriority priority, size_t threadNumber)
            { return js.kickJob(function, priority, threadNumber); }
        );
    }

    ///
    /// ---------- JobSystem related stuff finished
    ///

    ///
    /// ---------- Component related stuff
    ///

    template<>
    void registerUserType<Scene>(sol::state& lua) {
        sol::usertype<Scene> ut = lua.new_usertype<Scene>("Scene", sol::constructors<Scene()>());
        ut["GetGameObject"] = &Scene::GetGameObject;
        ut["CreateGameObject"] = sol::overload(
            [](Scene& scene) {scene.CreateGameObject(); },
            [](Scene& scene, entt::entity id) {scene.CreateGameObject(id); }
        );
        ut["name"] = &Scene::name;
    }

    template<>
    void registerUserType<GameObject>(sol::state& lua) {
        sol::usertype<GameObject> ut = lua.new_usertype<GameObject>("GameObject",
            sol::constructors<
            GameObject(),
            GameObject(const GameObject&),
            GameObject(entt::entity, Scene*)
            >()
            );
        ut["GetID"] = &GameObject::GetID;
        ut["GetTransform"] = &GameObject::GetTransform;

        registerUserComponent<Transform>(lua, "Transform");
        registerUserComponent<MeshRenderer>(lua, "MeshRenderer");
        registerUserComponent<ScriptComponent>(lua, "ScriptComponent");
    }

    template<>
    void registerUserType<Component>(sol::state& lua) {
        sol::usertype<Component> ut = lua.new_usertype<Component>("Component", sol::constructors<
            Component(),
            Component(const Component&)
        >());
        ut["GetGameObject"] = &Component::GetGameObject;
    }

    template<>
    void registerUserType<Transform>(sol::state& lua) {
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
    void registerUserType<MeshRenderer>(sol::state& lua) {
        sol::usertype<MeshRenderer> ut = lua.new_usertype<MeshRenderer>("MeshRenderer",
            sol::constructors<
                MeshRenderer(const MeshRenderer&),
                MeshRenderer(const Model&, const Material&)
            >(),
            sol::base_classes, sol::bases<Component>()
            );
        ut["GetMesh"] = &MeshRenderer::GetMesh;
        ut["GetMaterial"] = &MeshRenderer::GetMaterial;
        ut["SetMesh"] = &MeshRenderer::SetMesh;
        ut["SetMaterial"] = &MeshRenderer::SetMaterial;
    }

    ///
    /// ---------- Component related stuff finished
    ///

    ///
    /// ---------- Resources related stuff
    ///

    template<>
    void registerUserType<BaseResource>(sol::state& lua) {
        using ResourceID = BaseResource::ResourceID;
        sol::usertype<BaseResource> ut = lua.new_usertype<BaseResource>("BaseResource",
            sol::constructors<
                BaseResource(const BaseResource&),
                BaseResource(ResourceID id)
            >()
        );
        ut["GetResourceID"] = &BaseResource::GetResourceID;
    }

    template<>
    void registerUserType<Model>(sol::state& lua) {
        sol::usertype<Model> ut = lua.new_usertype<Model>("Model",
            sol::constructors<Model()>(),
            sol::base_classes, sol::bases<BaseResource>()
        );
        ut["Initialize"] = &Model::Initialize;
        ut["meshes"] = &Model::meshes;
    }

    template<>
    void registerUserType<Material>(sol::state& lua) {
        sol::usertype<Material> ut = lua.new_usertype<Material>("Material",
            sol::constructors<
                Material(),
                Material(std::string id),
                Material(MaterialDescriptor desc)
            >(),
            sol::base_classes, sol::bases<BaseResource>()
            );
        ut["matPtr"] = &Material::matPtr;
    }

    template<>
    void registerUserType<MaterialType>(sol::state& lua) {
        lua.new_enum("MaterialType",
            "None", MaterialType::None,
            "ColoredMaterial", MaterialType::ColoredMaterial,
            "TexturedMaterial", MaterialType::TexturedMaterial
        );
    }

    template<>
    void registerUserType<MaterialDescriptor>(sol::state& lua) {
        sol::usertype<MaterialDescriptor> ut = lua.new_usertype<MaterialDescriptor>("MaterialDescriptor");
        ut["key"] = &MaterialDescriptor::key;
        ut["MaterialType"] = &MaterialDescriptor::MaterialType;
        ut["color"] = &MaterialDescriptor::color;
        ut["vsKey"] = &MaterialDescriptor::vsKey;
        ut["psKey"] = &MaterialDescriptor::psKey;
        ut["vsPath"] = &MaterialDescriptor::vsPath;
        ut["psPath"] = &MaterialDescriptor::psPath;
        ut["textureKey"] = &MaterialDescriptor::textureKey;
    }

    template<>
    void registerUserType<MaterialBase>(sol::state& lua) {
        sol::usertype<MaterialBase> ut = lua.new_usertype<MaterialBase>("MaterialBase");
        ut["ApplyMaterial"] = &MaterialBase::ApplyMaterial;
        ut["GetMaterialType"] = &MaterialBase::GetMaterialType;
    }

    template<>
    void registerUserType<ColoredMaterialBase>(sol::state& lua) {
        sol::usertype<ColoredMaterialBase> ut = lua.new_usertype<ColoredMaterialBase>("ColoredMaterialBase",
            sol::constructors<ColoredMaterialBase(MaterialDescriptor)>(),
            sol::base_classes, sol::bases<MaterialBase>()
        );
        ut["SetColor"] = &ColoredMaterialBase::SetColor;
    }

    template<>
    void registerUserType<TexturedMaterialBase>(sol::state& lua) {
        sol::usertype<TexturedMaterialBase> ut = lua.new_usertype<TexturedMaterialBase>("TexturedMaterialBase",
            sol::constructors<TexturedMaterialBase(MaterialDescriptor)>(),
            sol::base_classes, sol::bases<MaterialBase>()
        );
        ut["SetTexture"] = &TexturedMaterialBase::SetTexture;
    }

    template<>
    void registerUserType<MeshDesc>(sol::state& lua) {
        sol::usertype<MeshDesc> ut = lua.new_usertype<MeshDesc>("MeshDesc",
            sol::constructors<MeshDesc()>()
        );
        ut["key"] = &MeshDesc::key;
        ut["layout"] = &MeshDesc::layout;
        ut["path"] = &MeshDesc::path;
    }

    template<>
    void registerUserType<ResourceFactory>(sol::state& lua) {
        sol::usertype<ResourceFactory> ut = lua.new_usertype<ResourceFactory>("ResourceFactory");
        ut["Get"] = &ResourceFactory::Get;
        ut["GetHashValue"] = &ResourceFactory::GetHashValue;
        registerUserResource<Material>(lua, "Material");
        registerUserResource<Texture>(lua, "Texture");
        registerUserResource<pixelShader>(lua, "pixelShader");
        registerUserResource<vertexShader>(lua, "vertexShader");
        registerUserResource<Model>(lua, "Model");
    }

    ///
    /// ---------- Resources related stuff finished
    ///


    ///
    /// ---------- AI related stuff
    ///

    template<>
    void registerUserType<TickResult>(sol::state& lua) {
        lua.new_enum("TickResult",
            "FAILURE", TickResult::FAILURE,
            "SUCCESS", TickResult::SUCCESS,
            "RUNNING", TickResult::RUNNING
        );
    }

    // fully implemented inside lua tables,
    // only need for registering these tables;
    struct UGLY_BLACK_BOARD_STUB___{};
    template<>
    void registerUserType<UGLY_BLACK_BOARD_STUB___>(sol::state& lua) {
        lua.script(R"SCRIPT(

        BlackBoard = {storage = {}}

        function BlackBoard:get(key)
            return self.storage[key]
        end

        function BlackBoard:set(key, value)
            self.storage[key] = value
        end

        )SCRIPT");
    }

    template<>
    void registerUserType<Position>(sol::state& lua) {
        sol::usertype<Model> ut = lua.new_usertype<Position>("Position",
            sol::constructors<Position()>()
            );
        ut["Initialize"] = &Model::Initialize;
        ut["meshes"] = &Model::meshes;
    }

    ///
    /// ---------- AI related stuff finished
    ///


}
