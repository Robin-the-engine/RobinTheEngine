#pragma once
#include "Platform/DirectX11/Camera.h"

namespace RTE {


	class RenderSystem
	{
	public:
		virtual void Init() = 0;
		virtual void OnResize(int width, int height) = 0;
		virtual void OnRenderBegin() = 0;
		virtual void OnRenderEnd() = 0;
		virtual ~RenderSystem() {};

		virtual void SetCamera(Camera* camera) = 0;


	};

}

