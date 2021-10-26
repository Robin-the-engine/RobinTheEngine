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

		vertexShader vs(L"shaders\\VS.hlsl");
		pixelShader ps(L"shaders\\PS.hlsl");
		//ResourceFactory::Get().GetResource<Model>("ogre");
		//auto id = ResourceFactory::Get().GetHashValue("ogre");
		CB_VS_MATRIX4x4 rotation;

		//Create sampler description for sampler state
		CD3D11_SAMPLER_DESC sampDesc(D3D11_DEFAULT);
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		auto rs = ((DirectX11RenderSystem*)m_RenderSystem.get());
		ComPtr<ID3D11SamplerState> samplerState;
		ThrowIfFailed(rs->GetDevice()->CreateSamplerState(&sampDesc, samplerState.GetAddressOf())); //Create sampler state

		float i = 5;
		while (m_Running) {
			m_Window->OnUpdate();
			m_RenderSystem->OnRenderBegin();

			auto context = static_cast<DirectX11RenderSystem*>(m_RenderSystem.get())->GetContext();
			context->IASetInputLayout(vs.GetInputLayout());
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			context->VSSetShader(vs.GetShader(), 0, 0);
			context->PSSetShader(ps.GetShader(), 0, 0);
			context->CSSetSamplers(0, 1, samplerState.GetAddressOf());

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			for (Layer* layer : m_LayerStack)
				layer->OnRender();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
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
		m_RenderSystem->OnResize(800, 800);

		return true;
	}
}
