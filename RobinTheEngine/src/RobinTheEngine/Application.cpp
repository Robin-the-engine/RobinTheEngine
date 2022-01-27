#include "rtepch.h"
#include "Application.h"
#include "Display.h"
#include "Platform/DirectX12/DirectX12RenderSystem.h"
#include "Platform/DirectX11/DirectX11RenderSystem.h"
#include "Platform/Windows/WindowsWindow.h"
#include "GLFW/glfw3.h"
#include "RobinTheEngine/Input.h"

#include "Platform/DirectX11/Buffer.h"
#include "Platform/DirectX11/Shaders.h"
#include "Platform/DirectX11/ConstantBuffer.h"
#include "Platform/DirectX11/Vertex.h"
#include "Platform/DirectX11/Camera.h"
#include "Platform/DirectX11/Model.h"
#include "Platform/DirectX11/GameObject.h"
#include "ResourceFactory.h"
#include "Timestep.h"


namespace RTE {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;
	RTE::Application::Application()
	{
		RTE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		auto hwnd = ((WindowsWindow*)(m_Window.get()))->GetHwnd();
		m_RenderSystem = std::make_unique<DirectX11RenderSystem>(hwnd);
		m_RenderSystem->Init();
		m_RenderSystem->OnResize(m_Window->GetWidth(), m_Window->GetHeight());

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);


	}


	Application::~Application()
	{

	}

	void Application::Run()
	{


		const int TICKS_PER_SECOND = 50;
		const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
		const int MAX_FRAMESKIP = 10;
		DWORD next_game_tick = 0; //GetTickCount64();
		int loops;

		while (m_Running) {

			loops = 0;
			m_Window->OnUpdate();
			scene.UpdateScene();
			while (GetTickCount64() > next_game_tick && loops < MAX_FRAMESKIP) {
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate();

				next_game_tick += SKIP_TICKS;
				loops++;
			}


			m_RenderSystem->OnRenderBegin();
			//scene.RenderScene(*m_RenderSystem);
			m_RenderSystem->DoRender(&scene);
			//for (Layer* layer : m_LayerStack)
				//layer->OnRender();

			m_ImGuiLayer->Begin();
			{
				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_RenderSystem->OnRenderEnd();

		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

    HWND Application::GetNativeWindowHandle() const {
        return dynamic_cast<WindowsWindow*>(m_Window.get())->GetHwnd();;
    }

    void Application::OnEvent(Event& e)
	{

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));


		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}

	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e) {
		m_RenderSystem->OnResize(e.GetWidth(), e.GetHeight());
		return true;
	}
}




Microsoft::WRL::ComPtr<ID3D11Device> RTE::GetDevice() {
	auto rs = (RTE::DirectX11RenderSystem*)RTE::Application::Get().GetRenderSystem();
	return rs->GetDevice();

}

Microsoft::WRL::ComPtr<ID3D11DeviceContext> RTE::GetContext() {
	auto rs = (RTE::DirectX11RenderSystem*)RTE::Application::Get().GetRenderSystem();
	return rs->GetContext();

}
