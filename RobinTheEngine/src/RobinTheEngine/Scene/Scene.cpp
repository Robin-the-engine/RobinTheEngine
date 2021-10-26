#include "rtepch.h"
#include "Scene.h"
#include "GameObject.h"

namespace RTE
{
	GameObject Scene::CreateGameObject()
	{
		return CreateGameObject(entt::null);
	}

	GameObject Scene::CreateGameObject(entt::entity id)
	{
		auto entity = registry.create(id);
		Transform transform = registry.emplace<Transform>(entity);
		transform.scene = this;
		transform.owner = entity;
		return GameObject(entity, this);
	}

	GameObject Scene::GetGameObject(entt::entity id)
	{
		return GameObject(id, this);
	}
}