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
		GameObject CreateGameObject(entt::entity id);
		GameObject GetGameObject(entt::entity entity);

		std::string name = "Untitled";

	private:
		entt::registry registry;

		friend class GameObject;
		friend class Serializer;
	};
}
