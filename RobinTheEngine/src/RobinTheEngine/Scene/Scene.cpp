#include "rtepch.h"
#include "Scene.h"
#include "GameObject.h"
#include <RobinTheEngine/RenderSystem.h>
#include "RobinTheEngine/Scene/Components/ColliderComponent.h"

namespace RTE
{
	GameObject Scene::CreateGameObject()
	{
		return CreateGameObject(entt::null);
	}

	GameObject Scene::CreateGameObject(entt::entity id)
	{
		auto entity = registry.create(id);
		Transform& transform = registry.emplace<Transform>(entity);
		transform.scene = this;
		transform.owner = entity;
		return GameObject(entity, this);
	}

	GameObject Scene::GetGameObject(entt::entity id)
	{
		return GameObject(id, this);
	}

	void Scene::DeleteGameObject(entt::entity entity)
	{
		registry.destroy(entity);
	}

	void Scene::UpdateScene()
	{
		UpdateCameras();
		UpdateColliders();
		UpdateLight(*cameraptr);
		
	}

	void Scene::UpdateLight(RTE::Camera cam)
	{
		auto lights = registry.view<RTE::Transform, RTE::LightComponent>();

		for (auto l : lights)
		{

			auto comps = lights.get<RTE::Transform, RTE::LightComponent>(l);

			auto& transform = std::get<0>(comps);
			auto& lightComp = std::get<1>(comps);
			
			auto lightPos = XMVectorSet(0, 0, 0, 1);
			auto lightRot = XMVectorSet(0, 0, 1, 0);

			lightPos = XMVector4Transform(lightPos, transform.GetMatrix());
			lightRot = XMVector4Transform(lightRot, transform.GetMatrix());

			cam.GetViewMatrix();
			XMStoreFloat4(&lightComp.lightComponent.PositionVS, XMVector4Transform(lightPos, cam.GetViewMatrix()));
			XMStoreFloat4(&lightComp.lightComponent.DirectionVS, XMVector4Normalize(XMVector4Transform(lightRot, cam.GetViewMatrix())));

		}
	}

	void Scene::UpdateCameras()
	{
		auto cameras = registry.view<RTE::Transform, RTE::Camera>();

		for (auto l : cameras)
		{

			auto comps = cameras.get<RTE::Transform, RTE::Camera>(l);

			auto& transform = std::get<0>(comps);
			auto& cam = std::get<1>(comps);

			if (cam.Activate) {
				cameraptr = &cam;
			}

			if (cam.GetDirtyFlag()) {
				transform.SetPosition(cam.GetPositionFloat3());
				transform.SetRotation(cam.GetRotationFloat3());
				cam.SetDirtyFlag(false);
				continue;
			}

			cam.SetPosition(transform.GetPosition());
			cam.SetRotation(transform.GetRotation());

			
		}
		
	}

	void Scene::UpdateColliders()
	{
		auto colliders = registry.view<RTE::Transform, RTE::Collider>();

		for (auto l : colliders)
		{
			auto comps = colliders.get<RTE::Transform, RTE::Collider>(l);

			auto& transform = std::get<0>(comps);
			auto& coll = std::get<1>(comps);

			coll.origin = transform.GetPosition();
			auto tmp = transform.GetRotation();
			coll.direction = XMFLOAT4(tmp.x, tmp.y, tmp.z, 0);
			XMFLOAT4X4 tm; XMStoreFloat4x4(&tm, transform.GetMatrix());
			coll.Update(tm);
		}

	}

	/*void Scene::RenderScene(RenderSystem& rs)
	{
		auto MeshesToRender = registry.view<RTE::Transform, RTE::MeshRenderer>();
		for (auto go : MeshesToRender)
		{
			auto toRen = MeshesToRender.get<RTE::Transform, RTE::MeshRenderer>(go);
			rs.DoRender(toRen);
		}
	}*/
}