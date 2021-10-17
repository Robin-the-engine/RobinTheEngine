#pragma once
#include "entt.hpp"


namespace RTE
{
	class Scene;
	class GameObject;

	struct Component
	{
	public:
		Component() = default;
		Component(const Component&) = default;

		GameObject GetGameObject();

	private:
		Scene* scene = nullptr;
		entt::entity owner = entt::null;

		friend class GameObject;
		friend class Scene;
	};
}
