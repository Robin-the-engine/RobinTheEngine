#pragma once
#include "Core.h"
#include "Window.h"

#include "Platform/DirectX12/DirectX12RenderSystem.h"

#include "RobinTheEngine/LayerStack.h"
#include "RobinTheEngine/Events/Event.h"
#include "RobinTheEngine/Events/ApplicationEvent.h"
#include "RobinTheEngine/ImGui/ImGuiLayer.h"
#include "Platform/DirectX11/Camera.h"
#include "Platform/DirectX11/ConstantBuffer.h"
#include <memory>


namespace RTE {

	class RTE_API Application
	{
	public:
		//TEMP camera for testing
		Camera camera;
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline RenderSystem* GetRenderSystem() { return m_RenderSystem.get(); }
		inline static Application& Get() { return *s_Instance; }

		//Temp cbuffer. TODO: remove this to the materials
		std::unique_ptr<ConstantBuffer<CB_VS_MATRIX4x4>> cbuffer;
		std::unique_ptr<ConstantBuffer<CB_PS_LIGHT>> lightCbuffer;


	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		std::unique_ptr<Window> m_Window;
		std::unique_ptr<RenderSystem> m_RenderSystem;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		static Application* s_Instance;

		
	};

	Application* CreateApplication();
}

