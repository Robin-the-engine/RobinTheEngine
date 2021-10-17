#pragma once
#include "entt.hpp"

namespace RTE
{
	class GameObject;

	class Scene
	{
	public:
		Scene() = default;

		GameObject CreateGameObject();
		GameObject GetGameObject(entt::entity entity);

	private:
		entt::registry registry;

		friend class GameObject;
	};
}



