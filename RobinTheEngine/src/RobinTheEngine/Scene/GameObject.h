#pragma once
#include "Scene.h"
#include "Components.h"

namespace RTE
{
	class GameObject final
	{
	public:
		GameObject() = default;
		GameObject(const GameObject& other) = default;
		GameObject(entt::entity entity, Scene* scene);

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

		Transform& GetTransform();
		uint32_t GetID();

		operator bool() const;
		operator entt::entity() const;

		bool operator==(const GameObject& other) const;
		bool operator!=(const GameObject& other) const;

	private:
		Scene* scene = nullptr;
		entt::entity entity = entt::null;
	};
}