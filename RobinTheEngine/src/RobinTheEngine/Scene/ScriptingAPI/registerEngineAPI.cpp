#include "rtepch.h"
#include "ScriptingAPI.h"
#include "ScriptingAPIImpl.h"
#include "../../../Platform/DirectX11/GameObject.h"

// TODO: remove
template<>
void RTE::registerUserType<RTE::Deprecated::GameObject>(sol::state& lua) {
    using RTE::Deprecated::GameObject;
    sol::usertype<GameObject> ut = lua.new_usertype<GameObject>("GameObject");
    ut["SetPosition"] = sol::overload(
        [](GameObject& go, const XMVECTOR& pos) {go.SetPosition(pos); },
        [](GameObject& go, const XMFLOAT3& pos) {go.SetPosition(pos); },
        [](GameObject& go, float x, float y, float z) {go.SetPosition(x, y, z); }
    );
    ut["AdjustRotation"] = sol::overload(
        [](GameObject& go, const XMVECTOR& rot) {go.AdjustRotation(rot); },
        [](GameObject& go, const XMFLOAT3& rot) {go.AdjustRotation(rot); },
        [](GameObject& go, float x, float y, float z) {go.AdjustRotation(x, y, z); }
    );
    ut["SetRotation"] = sol::overload(
        [](GameObject& go, const XMVECTOR& rot) {go.SetRotation(rot); },
        [](GameObject& go, const XMFLOAT3& rot) {go.SetRotation(rot); },
        [](GameObject& go, float x, float y, float z) {go.SetRotation(x, y, z); }
    );
    ut["SetLookAtPos"] = &GameObject::SetLookAtPos;
    ut["GetPositionFloat3"] = &GameObject::GetPositionFloat3;
}

void RTE::registerEngineAPI(sol::state& lua) {
    // we don't need this probably
    //registerUserType<DirectX::XMMATRIX>(lua);

    registerUserType<DirectX::XMFLOAT3>(lua);
    registerUserType<Scene>(lua);
    registerUserType<GameObject>(lua);
    registerUserType<Transform>(lua);
    registerUserType<MeshRenderer>(lua);
    registerUserType<Serializer>(lua);
    registerUserType<GameTimer>(lua);
    registerUserType<Camera>(lua);
    registerUserType<Log>(lua);

    // TODO: remove
    registerUserType<RTE::Deprecated::GameObject>(lua);
}