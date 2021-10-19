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
		bool HasComponent();

		template<typename T>
		T& GetComponent() const;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args);

		template<typename T>
		void RemoveComponent();

		Transform& GetTransform();

		operator bool() const;
		operator entt::entity() const;

		bool operator==(const GameObject& other) const;
		bool operator!=(const GameObject& other) const;

	private:
		Scene* scene = nullptr;
		entt::entity entity = entt::null;
	};
}