#include <iostream>
#include <RTE.h>
#include "RobinTheEngine/EntryPoint.h"
#include "imgui/imgui.h"
#include "RobinTheEngine/Scene/Serializer.h"
#include "RobinTheEngine/Scene/GameObject.h"
#include "RobinTheEngine/ResourceFactory.h"
#include "RobinTheEngine/UI/ContentBrowser.h"
#include <fstream>

using namespace DirectX;

const char saveloadFolder[] = "assets/scene/";
const int nameSize = 30;
char saveFileName[nameSize] = { "Untitled" };
char loadFileName[nameSize] = { "Untitled" };

class ExampleLayer : public RTE::Layer
{
public:
		
	RTE::Camera* camera;
	using JobHandle = RTE::JobHandle;
	using BehaviourTree = RTE::BehaviourTree;

	RTE::Window* window;
	GameTimer timer;
	float cameraSensitivity;
	float posX, posY;
	float cameraSpeed;
		
	//RTE::JobSystem &jobSystem = RTE::JobSystem::GetJobSystem();

	RTE::DirectX11RenderSystem* rs = static_cast<RTE::DirectX11RenderSystem*>(RTE::Application::Get().GetRenderSystem());
	RTE::Scene* scenePTR;


	ExampleLayer()
		: Layer("Example")
	{
		scenePTR = &RTE::Application::Get().scene;
	}
	
	RTE::GameObject testgo;

	void OnAttach() {
		scenePTR->name = "Test Scene";

		testgo = scenePTR->CreateGameObject();
		testgo.AddComponent<RTE::BehaviourTree>("example");
		auto& ai = testgo.AddComponent<RTE::AIComponent>(R"(D:\Projects\c++\RobinTheEngine\SandBox\Content\Scripts\ai.lua)");
		ai.init();

		auto cam = scenePTR->CreateGameObject();
		camera = &cam.AddComponent<RTE::Camera>();
		camera->SetPosition(XMFLOAT3(5, 4, -15));
		camera->SetProjectionProperties(45, static_cast<float>(RTE::Application::Get().GetWindow().GetWidth()) / static_cast<float>(RTE::Application::Get().GetWindow().GetHeight()), 1, 1000);

		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {

				auto go = scenePTR->CreateGameObject();
				auto& mr = go.AddComponent<RTE::MeshRenderer>();
				auto& transform = go.AddComponent<RTE::Transform>();
				mr.SetMaterial(RTE::ResourceFactory::Get().GetResource<RTE::Material>("texturedMaterial"));
				mr.SetMesh(RTE::ResourceFactory::Get().GetResource<RTE::Model>("ogre"));
				int baseX = -10;
				int basey = -10;
				transform.SetPosition(baseX + (i * 2), basey + (j * 2), 0);
			}

		}


		window = &RTE::Application::Get().GetWindow();
		cameraSensitivity = 5000;
		cameraSpeed = 15000;
	}

	float angle = 0;
	void OnUpdate() override
	{
		testgo.GetComponent<BehaviourTree>().tick();
		//testgo.GetComponent<RTE::ScriptComponent>().OnUpdate();
		timer.Reset();
		timer.Tick();
		//RTE_INFO("ExampleLayer::Delta time {0}",timer.DeltaTime());

		UpdateCamera();

		//if (RTE::Input::IsKeyPressed(RTE_KEY_TAB))
		//	RTE_TRACE("Tab key is pressed (poll)!");
		camera = scenePTR->cameraptr;
	}

	virtual void OnImGuiRender() override
	{
		//Uncomment for settings window
		/*
		static bool attachLightToCamera = false;
		ImGui::Begin("Settings");
		if (ImGui::CollapsingHeader("Scene Save/Load"))
		{
			ImGui::Text("Current directory: ");
			ImGui::SameLine();
			ImGui::Text(saveloadFolder);

			ImGui::InputText("Save file name", saveFileName, nameSize);
			if (ImGui::Button("Save Scene"))
			{
				RTE::Serializer sr(*scenePTR);
				sr.Serialize(std::string(saveloadFolder) + saveFileName + ".scene");
			}
			ImGui::Separator();
			ImGui::InputText("Load file name", (char*)&loadFileName, nameSize);
			if (ImGui::Button("Load Scene"))
			{

				RTE::Serializer sr(*scenePTR);
				sr.Deserialize(std::string(saveloadFolder) + loadFileName + ".scene");
			}
		}
		if (ImGui::CollapsingHeader("Camera settings")) {

			ImGui::SliderFloat("Camera sensitivity", &cameraSensitivity, 0, 10000);
			ImGui::SliderFloat("Camera speed", &cameraSpeed, 1500, 30000);
		}
		ImGui::DragFloat3("Clear color", &rs->GetClearColor().x, 0.001, 0, 1);
		ImGui::Text("Entities were drawn:%d", rs->GetFrameStats().ObjectsWasDrawed);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		//ImGui::ShowDemoWindow();
		//ImVec2 vec = ImVec2(800, 600);
		//ImGui::Image((ImTextureID)rs->GetFrameBufferPtr()->GetShaderResourceView().Get(), vec);


		ImGui::End();*/
	}

	void OnEvent(RTE::Event& event) override
	{
		RTE_TRACE("{0}", event);
		RTE::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<RTE::MouseButtonPressedEvent>(std::bind(&ExampleLayer::SetMousePosition, this, std::placeholders::_1));
		dispatcher.Dispatch<RTE::MouseButtonReleasedEvent>(std::bind(&ExampleLayer::ShowCursor, this, std::placeholders::_1));

	}


	void OnRender() override
	{
		//that method will removed. We not render things in layers.
	}

	void UpdateCamera() {

		if (RTE::Input::IsMouseButtonPressed(RTE_MOUSE_BUTTON_2)) {

			float signX, signY;
			float offsetX = posX - RTE::Input::GetMouseX();
			float offsetY = posY - RTE::Input::GetMouseY();

			signX = offsetX > 0 ? -1 : 1;
			signY = offsetY > 0 ? -1 : 1;

			if (offsetX) {
				camera->AdjustRotation(XMFLOAT3(0, cameraSensitivity * -offsetX * timer.DeltaTime(), 0));
				posX = RTE::Input::GetMouseX();
			}
			if (offsetY) {
				camera->AdjustRotation(XMFLOAT3(cameraSensitivity * -offsetY * timer.DeltaTime(), 0, 0));
				posY = RTE::Input::GetMouseY();
			}

		}

		if (RTE::Input::IsKeyPressed(RTE_KEY_W)) {
			camera->AdjustPosition(camera->GetForwardVector() * cameraSpeed * timer.DeltaTime());
		}
		if (RTE::Input::IsKeyPressed(RTE_KEY_S)) {
			camera->AdjustPosition(camera->GetBackwardVector() * cameraSpeed * timer.DeltaTime());
		}
		if (RTE::Input::IsKeyPressed(RTE_KEY_A)) {
			camera->AdjustPosition(camera->GetLeftVector() * cameraSpeed * timer.DeltaTime());
		}
		if (RTE::Input::IsKeyPressed(RTE_KEY_D)) {
			camera->AdjustPosition(camera->GetRightVector() * cameraSpeed * timer.DeltaTime());
		}
		if (RTE::Input::IsKeyPressed(RTE_KEY_SPACE)) {
			camera->AdjustPosition(XMFLOAT3(0.f, cameraSpeed * timer.DeltaTime(), 0.f));
		}
		if (RTE::Input::IsKeyPressed(RTE_KEY_LEFT_CONTROL)) {
			camera->AdjustPosition(XMFLOAT3(0.f, -cameraSpeed * timer.DeltaTime(), 0.f));
		}
	}

	bool SetMousePosition(RTE::MouseButtonPressedEvent ev) {
		if (ev.GetMouseButton() == RTE_MOUSE_BUTTON_2) {
			window->HideCursor();
			posX = RTE::Input::GetMouseX();
			posY = RTE::Input::GetMouseY();
		}
		return true;

	}

	bool ShowCursor(RTE::MouseButtonReleasedEvent ev) {
		if (ev.GetMouseButton() == RTE_MOUSE_BUTTON_2) {
			window->ShowCursor();
		}
		return true;

	}
};

class Sandbox : public RTE::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};


RTE::Application* RTE::CreateApplication()
{
	return new Sandbox();
}
