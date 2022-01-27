#pragma once
#include <iostream>
#include <RTE.h>
#include "imgui/imgui.h"
#include "RobinTheEngine/Scene/Serializer.h"
#include "RobinTheEngine/Scene/GameObject.h"
#include "RobinTheEngine/UI/ContentBrowser.h"


namespace RTE {

	struct selectedEnt
	{	
		entt::entity ent;
		bool isValid = false;
	};


	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;
		virtual void OnAttach() override;
		void OnUpdate() override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;
		void UpdateCamera();
		bool MousePressed(RTE::MouseButtonPressedEvent ev);
		bool ShowCursor(RTE::MouseButtonReleasedEvent ev);
	private:

	    void attachContentBrowserWindow();
		void pickObject();
		RTE::Camera* camera;

		RTE::Window* window;
		GameTimer timer;
		float cameraSensitivity;
		float posX, posY;
		float cameraSpeed;
		RTE::DirectX11RenderSystem* rs = static_cast<RTE::DirectX11RenderSystem*>(RTE::Application::Get().GetRenderSystem());
		RTE::Scene* scenePTR;
		float ambientStrength = 1;
		DirectX::XMFLOAT3 ambientColor = DirectX::XMFLOAT3(1, 1, 1);
		float diffuseStrength = 1;
		DirectX::XMFLOAT3 diffuseCollor = DirectX::XMFLOAT3(1, 1, 1);
		DirectX::XMFLOAT3 lightPos = DirectX::XMFLOAT3(0, 0, 0);
		float specularStrength = 1;
		float simulationSpeed = 1;
		float angle = 0;
		ImVec2 m_ViewportSize = { 0,0 };
		ImVec2 m_ViewportBounds[2];
		bool IsViewPortHowered = false;
		bool IsViewPortPressed = false;
		selectedEnt selectedEN;
		std::vector<std::string> meshesIDs;
		std::vector<std::string> materialsIDs;

	};

}


