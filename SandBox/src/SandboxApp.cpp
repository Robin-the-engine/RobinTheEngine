#include <iostream>
#include <RTE.h>
#include "imgui/imgui.h"
#include "RobinTheEngine/Scene/Serializer.h"
#include "RobinTheEngine/Scene/GameObject.h"

using namespace DirectX;

const char saveloadFolder[] = "assets/scene/";
const int nameSize = 30;
char saveFileName[nameSize] = { "Untitled" };
char loadFileName[nameSize] = { "Untitled" };

class ExampleLayer : public RTE::Layer
{
public:

	RTE::Camera camera;
	using JobHandle = RTE::JobHandle;

	RTE::Window* window;
	GameTimer timer;
	float cameraSensitivity;
	float posX, posY;
	float cameraSpeed;
	//RTE::GameObject spot;
	RTE::Deprecated::GameObject ogre;
	RTE::Deprecated::GameObject ogre1;
	//RTE::GameObject blub;
	RTE::Deprecated::GameObject ball;
	RTE::Deprecated::GameObject ball1;
	RTE::Deprecated::GameObject ball2;
	RTE::Deprecated::GameObject ball3;
	RTE::Deprecated::GameObject amogus;
	RTE::Deprecated::GameObject amogus1;
	RTE::Deprecated::GameObject amogus2;
	RTE::JobSystem jobSystem;
  
	RTE::DirectX11RenderSystem* rs = static_cast<RTE::DirectX11RenderSystem*>(RTE::Application::Get().GetRenderSystem());

	RTE::Scene scene;

	float ambientStrength = 1;
	DirectX::XMFLOAT3 ambientColor = DirectX::XMFLOAT3(1, 1, 1);

	float diffuseStrength = 1;
	DirectX::XMFLOAT3 diffuseCollor = DirectX::XMFLOAT3(1, 1, 1);

	DirectX::XMFLOAT3 lightPos = DirectX::XMFLOAT3(0, 0, 0);

	float specularStrength = 1;

	float simulationSpeed = 1;

		RTE::ConstantBuffer<RTE::CB_VS_MATRIX4x4> cbuffer;
		RTE::ConstantBuffer<RTE::CB_PS_LIGHT> lightCbuffer;
	ExampleLayer()
		: Layer("Example"), cbuffer("MVPMatrix"), lightCbuffer("LightProps"),jobSystem()
	{

	}

	void OnAttach() {
		scene.name = "Test Scene";

		rs->GetContext()->PSSetConstantBuffers(0, 1, lightCbuffer.GetAddressOf());
		camera.SetPosition(XMFLOAT3(0, 0, -10));
		camera.SetProjectionProperties(90, static_cast<float>(RTE::Application::Get().GetWindow().GetWidth()) / static_cast<float>(RTE::Application::Get().GetWindow().GetWidth()), 0.05, 1000);
		window = &RTE::Application::Get().GetWindow();
		cameraSensitivity = 5000;
		cameraSpeed = 15000;

		//spot.Initialize("objects\\spot\\spot.obj", *RTE::Application::Get().cbuffer);
		//spot.SetTexturePath(0, 0, "objects\\spot\\spot_texture.png");
		//spot.SetPosition(-6, 0.5, 0);
		//spot.SetLookAtPos(XMFLOAT3(0, 0, 1));

		std::vector<JobHandle> handles;

		//RTE::JobDescription j(std::move([&]() {ogre.Initialize("objects\\ogre\\bs_rest.obj", *RTE::Application::Get().cbuffer); }));

		ogre.Initialize("objects\\ogre\\bs_rest.obj", cbuffer);
		ogre1.Initialize("objects\\ogre\\bs_rest.obj", cbuffer);
		ball.Initialize("objects\\PokemonBall.obj", cbuffer);
		ball1.Initialize("objects\\PokemonBall.obj", cbuffer);
		ball2.Initialize("objects\\PokemonBall.obj", cbuffer);
		ball3.Initialize("objects\\PokemonBall.obj", cbuffer);
		amogus.Initialize("objects\\amogus\\amogus.obj", cbuffer);
		amogus1.Initialize("objects\\amogus\\amogus.obj", cbuffer);
		amogus2.Initialize("objects\\amogus\\amogus.obj", cbuffer);


		ogre.SetTexturePath(0, 0, "objects\\ogre\\diffuse.png");
		ogre1.SetTexturePath(0, 0, "objects\\ogre\\diffuse.png");
		
			ball.SetTexturePath(0, 0, "objects\\green.png"); 
			ball.SetTexturePath(1, 0, "objects\\green.png"); 
			ball.SetTexturePath(2, 0, "objects\\green.png"); 
		
				ball1.SetTexturePath(0, 0, "objects\\spot\\spot_texture.png"); 
			ball1.SetTexturePath(1, 0, "objects\\spot\\spot_texture.png"); 
			ball1.SetTexturePath(2, 0, "objects\\spot\\spot_texture.png"); 
	
			ball2.SetTexturePath(0, 0, "objects\\blub\\blub_texture.png"); 
			ball2.SetTexturePath(1, 0, "objects\\blub\\blub_texture.png"); 
			ball2.SetTexturePath(2, 0, "objects\\blub\\blub_texture.png"); 
		
			ball3.SetTexturePath(0, 0, "objects\\black.png");
			ball3.SetTexturePath(1, 0, "objects\\black.png"); 
			ball3.SetTexturePath(2, 0, "objects\\black.png"); 
		 amogus.SetTexturePath(0, 0, "objects\\amogus\\amogusDiffuse.jpg"); 
			amogus1.SetTexturePath(0, 0, "objects\\amogus\\amogusNormal.jpg"); 
			amogus2.SetTexturePath(0, 0, "objects\\amogus\\amogusDiffuse.jpg"); 
		/*	blub.Initialize("objects\\blub\\blub_triangulated.obj", *RTE::Application::Get().cbuffer);
		blub.SetTexturePath(0, 0, "objects\\blub\\blub_texture.png");
		blub.AdjustPosition(5, 0, 0);*/
		ball.SetScale(0.01f, 0.01f, 0.01f); ball.AdjustPosition(2, 0, 0);
		ball1.SetScale(0.01f, 0.01f, 0.01f);
		ball2.SetScale(0.01f, 0.01f, 0.01f); 
		ball3.SetScale(0.01f, 0.01f, 0.01f); ball3.AdjustPosition(-2, 0, 0); 
		amogus.SetScale(0.01, 0.01, 0.01); amogus.AdjustPosition(-2, 0, 0); 
		amogus1.SetScale(0.005, 0.005, 0.005); amogus1.AdjustPosition(0, 1, 0); 
		amogus2.SetScale(0.01, 0.01, 0.01); 
		
	}

	float angle = 0;
	void OnUpdate() override
	{
		timer.Reset();
		timer.Tick();
		//RTE_INFO("ExampleLayer::Delta time {0}",timer.DeltaTime());

		UpdateCamera();
		UpdateLight();

		angle += timer.DeltaTime() * 200 * simulationSpeed;
		ogre1.SetPosition(5 *  cos( angle),
						5 * sin( angle) , 0);
		ogre1.SetLookAtPos(XMFLOAT3(0, 0, 0));
		ogre1.AdjustRotation(0, 2 * timer.DeltaTime(), 0);

		ball1.SetPosition(-5 *   cos(angle),
						0, 5 *  sin( angle));
		this->lightPos = ball1.GetPositionFloat3();

		ball2.SetPosition(0, -5 *   cos( angle),
						 -5 *  sin( angle));


		amogus.SetPosition(6 *   sin( angle),
						6 *  cos( angle) , 0);
		amogus.SetLookAtPos(XMFLOAT3(0, 0, 0));

		amogus2.SetPosition(5.5 *  sin( angle),0,
						5.5 *  cos( angle) );  
		amogus2.SetLookAtPos(XMFLOAT3(0, 0, 0));

		amogus1.SetRotation(XMFLOAT3(0,angle/3,0));
		ball.SetRotation(XMFLOAT3(angle/3, angle/5, 0));
		
		ogre.SetLookAtPos(XMFLOAT3(camera.GetPositionFloat3().x * -1, camera.GetPositionFloat3().y * -1, camera.GetPositionFloat3().z *-1));
		//ogre.SetLookAtPos(camera->GetPositionFloat3());
		//if (RTE::Input::IsKeyPressed(RTE_KEY_TAB))
		//	RTE_TRACE("Tab key is pressed (poll)!");
	}

	virtual void OnImGuiRender() override
	{
		static bool attachLightToCamera = false;
		ImGui::Begin("Test");
		if (ImGui::CollapsingHeader("Scene Save/Load"))
		{
			ImGui::Text("Current directory: ");
			ImGui::SameLine();
			ImGui::Text(saveloadFolder);

			ImGui::InputText("Save file name", saveFileName, nameSize);
			if (ImGui::Button("Save Scene"))
			{
				RTE::Serializer sr(scene);
				sr.Serialize(std::string(saveloadFolder) + saveFileName + ".scene");
			}
			ImGui::Separator();
			ImGui::InputText("Load file name", (char*)&loadFileName, nameSize);
			if (ImGui::Button("Load Scene"))
			{
				
				RTE::Serializer sr(scene);
				sr.Deserialize(std::string(saveloadFolder) + loadFileName + ".scene");
			}
		}
		if (ImGui::CollapsingHeader("Camera settings")) {

			ImGui::SliderFloat("Camera sensitivity", &cameraSensitivity, 0, 10000);
			ImGui::SliderFloat("Camera speed", &cameraSpeed, 1500, 30000);
		}
		if (ImGui::CollapsingHeader("Light settings"))
		{

			if (ImGui::CollapsingHeader("Ambient"))
			{
				ImGui::DragFloat("Ambient strength", &ambientStrength, 0.01f, 0, 1);
				ImGui::Separator();
				ImGui::ColorPicker3("Ambient color", &ambientColor.x);
			}
			ImGui::Separator();
			if (ImGui::CollapsingHeader("Diffuse"))
			{
				ImGui::DragFloat("Diffuse strength", &diffuseStrength, 0.01f, 0, 1);
				ImGui::Separator();
				ImGui::DragFloat("Specular strength", &specularStrength, 0.01f, 0, 1);
				ImGui::Separator();
				ImGui::ColorPicker3("Diffuse color", &diffuseCollor.x);
				ImGui::Separator();
				ImGui::DragFloat3("Light position", &lightPos.x, 1, -100, 100);
				ImGui::Separator();
				ImGui::Text("Set light position on camera");
				ImGui::SameLine();
				ImGui::Checkbox("", &attachLightToCamera);

			}
			ImGui::Separator();
		}

		if (attachLightToCamera) {
			this->lightPos = camera.GetPositionFloat3();
		}
		ImGui::DragFloat("Simulation speed", &this->simulationSpeed, 0.2f, 0, 100);
		ImGui::DragFloat3("Clear color", &rs->GetClearColor().x, 0.001, 0, 1);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		//ImGui::ShowDemoWindow();
		ImGui::End();
	}

	void OnEvent(RTE::Event& event) override
	{
		RTE_TRACE("{0}", event);
		RTE::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<RTE::MouseButtonPressedEvent>(std::bind(&ExampleLayer::SetMousePosition, this, std::placeholders::_1));
		dispatcher.Dispatch<RTE::MouseButtonReleasedEvent>(std::bind(&ExampleLayer::ShowCursor, this, std::placeholders::_1));
		//RTE_ASSERT(false, "Its false!");

	}


	void OnRender() override
	{
		auto vp = camera.GetViewMatrix()* camera.GetProjectionMatrix();
		//rs->GetContext()->PSSetConstantBuffers(0, 0, lightCbuffer.GetAddressOf());
		//spot.Draw(vp);
		ogre.Draw(vp);
		ogre1.Draw(vp);
		ball.Draw(vp);
		ball1.Draw(vp);
		ball2.Draw(vp);
		ball3.Draw(vp);
		amogus.Draw(vp);
		amogus1.Draw(vp);
		amogus2.Draw(vp);
		//blub.Draw(vp);
	}

	void UpdateCamera() {
		//window->SetEventCallback()

		if (RTE::Input::IsMouseButtonPressed(RTE_MOUSE_BUTTON_2)) {

			float signX, signY;
			float offsetX = posX - RTE::Input::GetMouseX();
			float offsetY = posY - RTE::Input::GetMouseY();

			signX = offsetX > 0 ? -1 : 1;
			signY = offsetY > 0 ? -1 : 1;

			if (offsetX) {
				camera.AdjustRotation(XMFLOAT3(0, cameraSensitivity* -offsetX * timer.DeltaTime(), 0));
				posX = RTE::Input::GetMouseX();
			}
			if (offsetY) {
				camera.AdjustRotation(XMFLOAT3(cameraSensitivity * -offsetY * timer.DeltaTime(), 0, 0));
				posY = RTE::Input::GetMouseY();
			}

		}

		if (RTE::Input::IsKeyPressed(RTE_KEY_W)) {
			camera.AdjustPosition(camera.GetForwardVector() * cameraSpeed* timer.DeltaTime());
		}
		if (RTE::Input::IsKeyPressed(RTE_KEY_S)) {
			camera.AdjustPosition(camera.GetBackwardVector() * cameraSpeed* timer.DeltaTime());
		}
		if (RTE::Input::IsKeyPressed(RTE_KEY_A)) {
			camera.AdjustPosition(camera.GetLeftVector() * cameraSpeed* timer.DeltaTime());
		}
		if (RTE::Input::IsKeyPressed(RTE_KEY_D)) {
			camera.AdjustPosition(camera.GetRightVector() * cameraSpeed* timer.DeltaTime());
		}
		if (RTE::Input::IsKeyPressed(RTE_KEY_SPACE)) {
			camera.AdjustPosition(XMFLOAT3(0.f, cameraSpeed* timer.DeltaTime(), 0.f));
		}
		if (RTE::Input::IsKeyPressed(RTE_KEY_LEFT_CONTROL)) {
			camera.AdjustPosition(XMFLOAT3(0.f, -cameraSpeed * timer.DeltaTime(), 0.f));
		}
	}
	void UpdateLight() {
		lightCbuffer.data.ambientStrength = this->ambientStrength;
		lightCbuffer.data.ambientLightColor = this->ambientColor;

		lightCbuffer.data.diffuseStrenght = this->diffuseStrength;
		lightCbuffer.data.diffuseCollor = this->diffuseCollor;

		lightCbuffer.data.lightPosition = this->lightPos;

		lightCbuffer.data.specularStrength = this->specularStrength;
		lightCbuffer.data.viewPosition = this->camera.GetPositionFloat3();

		lightCbuffer.WriteBuffer();
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
