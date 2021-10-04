#include "rtepch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_dx12.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_dx11.h"
#include "GLFW/glfw3.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Platform/DirectX11/DirectX11RenderSystem.h"

#include "RobinTheEngine/Application.h"

namespace RTE {

	RTE::WindowsWindow* win;
	RTE::DirectX12RenderSystem* ren;
	ImGuiIO* io;

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
		win = (WindowsWindow*) &(Application::Get().GetWindow());
		ren = (DirectX12RenderSystem*)(Application::Get().GetRenderSystem());

	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		io = &ImGui::GetIO();
		io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
   //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		//io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		ImGui::StyleColorsDark();


		ImGuiStyle& style = ImGui::GetStyle();
		if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		RTE::WindowsWindow* win = static_cast<WindowsWindow*>(&Application::Get().GetWindow());
		RTE::DirectX11RenderSystem* ren = static_cast<DirectX11RenderSystem*> (Application::Get().GetRenderSystem());

		//ImGui_ImplWin32_Init((GLFWwindow*)win->GetNativeWindow());
		ImGui_ImplGlfw_InitForVulkan((GLFWwindow*)win->GetNativeWindow(), true);
		//ImGui_ImplWin32_Init((GLFWwindow*)win->GetNativeWindow());
		ImGui_ImplDX11_Init(ren->GetDevice().Get(), ren->GetContext().Get());
		/*	ImGui_ImplDX12_Init(ren->GetDevice().Get(), 3,
				DXGI_FORMAT_R8G8B8A8_UNORM, ren->mCbvHeap.Get(),
				ren->mCbvHeap->GetCPUDescriptorHandleForHeapStart(),
				ren->mCbvHeap->GetGPUDescriptorHandleForHeapStart());*/

	}

	void ImGuiLayer::OnDetach()
	{
		//ImGui_ImplDX12_Shutdown();
		ImGui_ImplDX11_Shutdown();
		//ImGui_ImplWin32_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

	}

	void ImGuiLayer::Begin()
	{
		// Start the Dear ImGui frame
		//ImGui_ImplDX12_NewFrame();
		ImGui_ImplDX11_NewFrame();
		//ImGui_ImplWin32_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

	}


	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();

		ImGui::Render();
		//ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), ren->GetCmdList());
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		// Update and Render additional Platform Windows
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
			//ImGui::RenderPlatformWindowsDefault(NULL, (void*)ren->GetCmdList());
		}
	}

	void ImGuiLayer::OnImGuiRender()
	{
		/*static bool show = true;
		ImGui::ShowDemoWindow(&show);*/
	}


}