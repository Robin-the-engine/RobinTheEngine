#pragma once
#include "Platform/DirectX11/Camera.h"
#include "RobinTheEngine/Scene/Components.h"
#include "RobinTheEngine/Scene/Scene.h"

namespace RTE {


	class RenderSystem
	{
	public:
		virtual void Init() = 0;
		virtual void OnResize(int width, int height) = 0;
		virtual void OnRenderBegin() = 0;
		virtual void OnRenderEnd() = 0;
		virtual ~RenderSystem() {};

		virtual void DoRender(std::tuple<RTE::Transform, RTE::MeshRenderer> meshes) = 0;
		virtual void DoRender(std::tuple<RTE::Transform, RTE::MeshRenderer> meshes, void* lightComps) = 0;
		virtual void DoRender(RTE::Scene* scene) = 0;

		virtual void SetCamera(Camera* camera) = 0;
	};

}
