#include "rtepch.h"
#include "GameObject.h"
#include "RobinTheEngine/Log.h"

namespace RTE
{
	GameObject::GameObject(entt::entity entity, Scene* scene)
		: entity(entity), scene(scene)
	{ 	}

	Transform& GameObject::GetTransform() { return scene->registry.get<Transform>(entity); }

	template<typename T>
	bool GameObject::HasComponent()
	{
		return !scene->registry.orphan<T>(entity);
	}

	template<typename T>
	inline T& GameObject::GetComponent() const
	{
		return scene->registry.get<T>(entity);
	}

	template<typename T, typename... Args>
	T& GameObject::AddComponent(Args&&... args)
	{
		T& comp = scene->registry.emplace<T>(entity, std::forward<Args>(args)...);
		comp.scene = scene;
		comp.owner = entity;
		return comp;
	}

	template<typename T>
	void GameObject::RemoveComponent()
	{
		scene->registry.remove<T>(entity);
	}
	

	// Template specialization for Transform, because it's default
	template<>
	Transform& GameObject::AddComponent<Transform>()
	{
		return GetTransform();
	}
	template<>
	void GameObject::RemoveComponent<Transform>()
	{
		RTE_CORE_ERROR("You can't remove Transform component");
	}
}

