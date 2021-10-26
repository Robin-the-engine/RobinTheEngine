#include "rtepch.h"
#include "GameObject.h"
#include "RobinTheEngine/Log.h"

namespace RTE
{
	GameObject::GameObject(entt::entity entity, Scene* scene)
		: entity(entity), scene(scene)
	{ 	}

	Transform& GameObject::GetTransform() { return scene->registry.get<Transform>(entity); }

	uint32_t GameObject::GetID() { return (uint32_t)entity; }


	GameObject::operator bool() const { return scene && entity != entt::null; }
	GameObject::operator entt::entity() const { return entity; }

	bool GameObject::operator==(const GameObject& other) const
	{
		return entity == other.entity && scene == other.scene;
	}

	bool GameObject::operator!=(const GameObject& other) const
	{
		return !(*this == other);
	}
}

