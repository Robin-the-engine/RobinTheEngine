#include "rtepch.h"
#include "Scene.h"
#include "GameObject.h"

namespace RTE
{
	GameObject Scene::CreateGameObject()
	{
		auto entity = registry.create();
		Transform transform = registry.emplace<Transform>(entity);
		transform.scene = this;
		transform.owner = entity;
		return GameObject(entity, this);
	}

	GameObject Scene::GetGameObject(entt::entity entity)
	{
		return GameObject(entity, this);
	}
}