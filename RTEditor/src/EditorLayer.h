#pragma once
#include <iostream>
#include <RTE.h>
#include "imgui/imgui.h"
#include "RobinTheEngine/Scene/Serializer.h"
#include "RobinTheEngine/Scene/GameObject.h"


namespace RTE {
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
		void UpdateLight();
		bool SetMousePosition(RTE::MouseButtonPressedEvent ev);
		bool ShowCursor(RTE::MouseButtonReleasedEvent ev);
	private:


		RTE::Camera camera;
		//using JobHandle = RTE::JobHandle;

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
		RTE::ConstantBuffer<RTE::CB_VS_MATRIX4x4> cbuffer;
		RTE::ConstantBuffer<RTE::CB_PS_LIGHT> lightCbuffer;
		RTE::GameObject go2;
		float angle = 0;


	};

}


