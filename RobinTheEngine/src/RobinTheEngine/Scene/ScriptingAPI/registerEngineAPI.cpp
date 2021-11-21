#include "rtepch.h"
#include "ScriptingAPI.h"
#include "ScriptingAPIImpl.h"
#include "../../../Platform/DirectX11/GameObject.h"

void RTE::registerEngineAPI(sol::state& lua) {
    registerUserType<DirectX::XMFLOAT3>(lua);
    registerUserType<Scene>(lua);
    registerUserType<GameObject>(lua);
    registerUserType<Transform>(lua);
    registerUserType<MeshRenderer>(lua);
    registerUserType<Serializer>(lua);
    registerUserType<GameTimer>(lua);
    registerUserType<Camera>(lua);
    registerUserType<Log>(lua);
    registerUserType<JobPriority>(lua);
    registerUserType<JobSystem>(lua);
}
