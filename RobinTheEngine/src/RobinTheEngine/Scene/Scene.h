#pragma once
#include "entt.hpp"
#include <RobinTheEngine/RenderSystem.h>

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

		void RenderScene(RenderSystem& rs);

		std::string name = "Untitled";

	private:
		entt::registry registry;

		friend class GameObject;
		friend class Serializer;
	};
}



