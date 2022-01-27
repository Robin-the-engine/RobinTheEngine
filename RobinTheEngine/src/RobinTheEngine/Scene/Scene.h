#pragma once
#include "entt.hpp"
#include "Platform/DirectX11/Camera.h"


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

		void UpdateScene();
		//tmp cam ptr
		RTE::Camera* cameraptr = nullptr;

		//void RenderScene(RenderSystem& rs);

		std::string name = "Untitled";
		
		//temp method. we need to create some iterator for scene
		entt::registry* GetRegistryPtr() { return &registry; }
		
	private:
		entt::registry registry;
		void UpdateLight(RTE::Camera cam);
		void UpdateCameras();
		void UpdateColliders();

		friend class GameObject;
		friend class Serializer;
	};
}
