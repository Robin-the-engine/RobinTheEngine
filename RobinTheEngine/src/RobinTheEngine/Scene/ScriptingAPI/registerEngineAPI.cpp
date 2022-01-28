#include "rtepch.h"
#include "ScriptingAPI.h"
#include "ScriptingAPIImpl.h"
#include "../../../Platform/DirectX11/GameObject.h"

void RTE::registerEngineAPI(sol::state& lua) {

    registerUserType<Camera>(lua);
    registerUserType<GameTimer>(lua);
    registerUserType<Serializer>(lua);
    registerUserType<Log>(lua);
    registerUserType<DirectX::XMFLOAT3>(lua);

    registerUserType<JobPriority>(lua);
    registerUserType<JobSystem>(lua);

    registerUserType<Scene>(lua);
    registerUserType<GameObject>(lua);
    registerUserType<Component>(lua);
    registerUserType<Transform>(lua);
    registerUserType<MeshRenderer>(lua);

    registerUserType<BaseResource>(lua);
    registerUserType<Model>(lua);
    registerUserType<Material>(lua);
    registerUserType<MaterialType>(lua);
    registerUserType<MaterialDescriptor>(lua);
    registerUserType<MaterialBase>(lua);
    registerUserType<ColoredMaterialBase>(lua);
    registerUserType<TexturedMaterialBase>(lua);
    registerUserType<MeshDesc>(lua);
    registerUserType<ResourceFactory>(lua);

    registerUserType<TickResult>(lua);
    registerUserType<UGLY_BLACK_BOARD_STUB___>(lua);

}
