#pragma once

namespace RTE {


	class RenderSystem
	{
	public:
		virtual void Init() = 0;
		virtual void OnResize(int width, int height) = 0;
		virtual void OnRenderBegin() = 0;
		virtual void OnRenderEnd() = 0;
		virtual ~RenderSystem() {};


	};

}

