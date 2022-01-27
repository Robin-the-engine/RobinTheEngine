#include "EditorLayer.h"
#include <iostream>
#include <algorithm>

#include "RobinTheEngine/UI/ContentBrowser.h"


using namespace DirectX;

namespace RTE {

	XMFLOAT4 origin;
	XMFLOAT4 dir;
	bool isDraw = false;


	const char saveloadFolder[] = "assets/scene/";
	const int nameSize = 30;
	char saveFileName[nameSize] = { "Untitled" };
	char loadFileName[nameSize] = { "Untitled" };

	EditorLayer::EditorLayer()
		: Layer("Example")
	{
		scenePTR = &RTE::Application::Get().scene;


		for (auto mesh = ResourceFactory::Get().GetMeshDescriptorMap().begin(); mesh != ResourceFactory::Get().GetMeshDescriptorMap().end(); mesh++) {
			meshesIDs.push_back(mesh->first);
		}

		for (auto material = ResourceFactory::Get().GetMaterialDescriptorMap().begin(); material != ResourceFactory::Get().GetMaterialDescriptorMap().end(); material++) {
			materialsIDs.push_back(material->first);
		}

	}

	void EditorLayer::OnAttach() {
		scenePTR->name = "Test Scene";
		//scenePTR->cameraptr = &camera;
		auto cam = scenePTR->CreateGameObject();
		camera = &cam.AddComponent<RTE::Camera>();
		camera->SetPosition(XMFLOAT3(5, 4, -15));
		camera->SetProjectionProperties(45, static_cast<float>(RTE::Application::Get().GetWindow().GetWidth()) / static_cast<float>(RTE::Application::Get().GetWindow().GetHeight()), 1, 1000);

		for (int i = 0; i < 1; i++) {
			for (int j = 0; j < 5; j++) {

				auto go = scenePTR->CreateGameObject();
				auto& mr = go.AddComponent<RTE::MeshRenderer>();
				auto& transform = go.AddComponent<RTE::Transform>();
				mr.SetMaterial(RTE::ResourceFactory::Get().GetResource<RTE::Material>("newMaterialTesting"));
				mr.SetMesh(RTE::ResourceFactory::Get().GetResource<RTE::Model>("ogre"));
				int baseX = -10;
				int basey = -10;
				transform.SetPosition(baseX + (i * 2), basey + (j * 2), 0);
				go.AddComponent<RTE::LightComponent>();

				auto& cam = go.AddComponent<RTE::Camera>();
				//cam.SetPosition(XMFLOAT3(5, 4, -15));
				cam.SetProjectionProperties(45, static_cast<float>(RTE::Application::Get().GetWindow().GetWidth()) / static_cast<float>(RTE::Application::Get().GetWindow().GetHeight()), 1, 1000);
			}

		}

		window = &RTE::Application::Get().GetWindow();
		cameraSensitivity = 5000;
		cameraSpeed = 15000;

	}

	void EditorLayer::OnUpdate() {
		timer.Reset();
		timer.Tick();
		//RTE_INFO("ExampleLayer::Delta time {0}",timer.DeltaTime());
		if (IsViewPortPressed)
			UpdateCamera();
		//RTE_INFO("POS: {0}, {1}", posX, posY);
		rs->SetCustomFrameBuffer();
		//if (RTE::Input::IsKeyPressed(RTE_KEY_TAB))
		//	RTE_TRACE("Tab key is pressed (poll)!");
	    //	if (isDraw) {
		//	rs->DrawRay(origin, dir);
		//}
		camera = scenePTR->cameraptr;
	}

	void EditorLayer::attachContentBrowserWindow() {
		const static std::string defaultContentDir = "Content";
		static RTE::ContentBrowser cb("Utils\\Resources.yaml");
		static std::string currentDir = defaultContentDir;
		const static std::vector<std::pair<const std::string, RTE::ContentBrowser::ContentType>> menuMap = {
			{"file", RTE::ContentBrowser::FILE},
			{"mesh", RTE::ContentBrowser::MESH},
			{"texture", RTE::ContentBrowser::TEXTURE},
			{"shader", RTE::ContentBrowser::SHADER},
		};
		const auto listFiles = cb.listDirectory(currentDir);

		ImGuiWindowFlags cbflags = ImGuiWindowFlags_MenuBar;
		ImGui::Begin("Content browser", nullptr, cbflags);
		if (ImGui::BeginMenuBar()) {
			if (!std::filesystem::equivalent(currentDir, defaultContentDir)) {
				if (ImGui::Button("back")) {
					currentDir = cb.getNextDir(currentDir, "..");
				}
			}

			if (ImGui::BeginMenu("add")) {
				for (const auto& menuItem : menuMap) {
					if (ImGui::MenuItem(menuItem.first.c_str())) {
						std::string path = RTE::ContentBrowserGUI::openFileDialog();
						cb.addFile(path, currentDir, menuItem.second);
						break;
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		for (const auto& file : listFiles) {

			const std::string& btnName = file.second;
			const std::string popupname = btnName + "popup";
			if (ImGui::Button(btnName.c_str(), { 100, 100 })) {
				// Can't figure out how to check which mouse button clicked
				if (file.first == RTE::ContentBrowser::DIRECTORY) {
					currentDir = cb.getNextDir(currentDir, file.second);
				}
				else {
					ImGui::OpenPopup(popupname.c_str());
				}
			}
			if (ImGui::BeginPopup(popupname.c_str())) {
				if (ImGui::Selectable("delete")) {
					cb.removeFile(currentDir, file.second);
				}
				ImGui::EndPopup();
			}
			ImGui::SameLine();
		}
		ImGui::End();
	}

	void EditorLayer::OnImGuiRender()
	{
		// Note: Switch this to true to enable dockspace
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("RTE", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{/*
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				//if (ImGui::MenuItem("Exit")) Hazel::Application::Get().Close();
				ImGui::EndMenu();
			}*/

			ImGui::EndMenuBar();
		}


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("ViewPort");
		IsViewPortHowered = ImGui::IsWindowHovered();
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };
		if ((m_ViewportSize.x != viewportPanelSize.x) || (m_ViewportSize.y != viewportPanelSize.y))
		{
			rs->GetFrameBufferPtr()->Resize(rs->GetDevice(), (int)viewportPanelSize.x, (int)viewportPanelSize.y);
			//m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			camera->ResizeCamera(viewportPanelSize.x, viewportPanelSize.y);
		}


		ImGui::Image((ImTextureID)rs->GetFrameBufferPtr()->GetShaderResourceView().Get(), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 0 }, ImVec2{ 1, 1 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();



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
			this->lightPos = camera->GetPositionFloat3();
		}
		ImGui::DragFloat3("Clear color", &rs->GetClearColor().x, 0.001, 0, 1);
		ImGui::Text("Entities were drawn:%d", rs->GetFrameStats().ObjectsWasDrawed);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		//ImGui::ShowDemoWindow();

		ImGui::End();

		ImGuiWindowFlags cbflags = ImGuiWindowFlags_MenuBar;
		ImGui::Begin("Property panel", 0, cbflags);

		if (selectedEN.isValid) {

			bool hasMeshRenderer = scenePTR->GetRegistryPtr()->any_of<RTE::MeshRenderer>(selectedEN.ent);
			bool hasLightComponent = scenePTR->GetRegistryPtr()->any_of<RTE::LightComponent>(selectedEN.ent);
			bool hasScriptComponent = scenePTR->GetRegistryPtr()->any_of<RTE::ScriptComponent>(selectedEN.ent);
			bool hasCamera = scenePTR->GetRegistryPtr()->any_of<RTE::Camera>(selectedEN.ent);
			bool hasCollider = scenePTR->GetRegistryPtr()->any_of<RTE::Collider>(selectedEN.ent);

			if (ImGui::BeginMenuBar())
			{


				if (ImGui::BeginMenu("Add component"))
				{
					if (!hasMeshRenderer) {
						if (ImGui::MenuItem("Add mesh renderer component")) {
							scenePTR->GetGameObject(selectedEN.ent).AddComponent<MeshRenderer>();
						}
					}

					if (!hasLightComponent) {
						if (ImGui::MenuItem("Add light component")) {
							scenePTR->GetGameObject(selectedEN.ent).AddComponent<LightComponent>();
						}
					}

					if (!hasScriptComponent) {
						if (ImGui::MenuItem("Add script component")) {
							scenePTR->GetGameObject(selectedEN.ent).AddComponent<ScriptComponent>();

						}
					}

					if (!hasCamera) {
						if (ImGui::MenuItem("Add camera component")) {
							scenePTR->GetGameObject(selectedEN.ent).AddComponent<Camera>();
						}
					}
					if (!hasCollider) {
						if (ImGui::MenuItem("Add collider component")) {
							scenePTR->GetGameObject(selectedEN.ent).AddComponent<Collider>();
						}
					}


					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Delete component"))
				{

					if (hasMeshRenderer) {
						if (ImGui::MenuItem("Delete mesh renderer component")) {
							scenePTR->GetRegistryPtr()->remove<MeshRenderer>(selectedEN.ent);
						}
					}

					if (hasLightComponent) {
						if (ImGui::MenuItem("Delete light component")) {
							scenePTR->GetRegistryPtr()->remove<LightComponent>(selectedEN.ent);
						}
					}

					if (hasScriptComponent) {
						if (ImGui::MenuItem("Delete script component")) {
							scenePTR->GetRegistryPtr()->remove<ScriptComponent>(selectedEN.ent);
						}
					}

					if (hasCamera) {
						if (ImGui::MenuItem("Delete camera component")) {
							scenePTR->GetRegistryPtr()->remove<Camera>(selectedEN.ent);
						}
					}

					if (hasCollider) {
						if (ImGui::MenuItem("Delete collider component")) {
							scenePTR->GetRegistryPtr()->remove<Collider>(selectedEN.ent);
						}
					}

					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}



			if (scenePTR->GetRegistryPtr()->any_of<RTE::Transform>(selectedEN.ent)) {
				if (ImGui::CollapsingHeader("Transform")) {
					auto& tr = scenePTR->GetRegistryPtr()->get<Transform>(selectedEN.ent);
					auto pos = tr.GetPosition();
					ImGui::DragFloat3("Position", &pos.x, 0.1);
					tr.SetPosition(pos);

					auto rot = tr.GetRotation();
					ImGui::DragFloat3("Rotation", &rot.x, 0.1);
					tr.SetRotation(rot);

					auto scale = tr.GetScale();
					ImGui::DragFloat3("Scale", &scale.x, 0.1);
					tr.SetScale(scale);

				}
			}
			if (scenePTR->GetRegistryPtr()->any_of<RTE::MeshRenderer>(selectedEN.ent)) {

				if (ImGui::CollapsingHeader("Mesh renderer")) {
					auto& ar = Application::Get().scene.GetRegistryPtr()->get<MeshRenderer>(selectedEN.ent);

					int selectedMesh;
					if (ar.GetMesh().GetResourceID() == "invalid") selectedMesh = -1;

					auto iter = std::find(meshesIDs.begin(), meshesIDs.end(), ar.GetMesh().GetResourceID());
					if (iter != meshesIDs.end()) selectedMesh = std::distance(meshesIDs.begin(), iter);

					if (ImGui::Combo("Meshes", &selectedMesh,
						[](void* vec, int idx, const char** out_text) {
							std::vector<std::string>* vector = reinterpret_cast<std::vector<std::string>*>(vec);
							if (idx < 0 || idx >= vector->size())return false;
							*out_text = vector->at(idx).c_str();
							return true;
						}, reinterpret_cast<void*>(&meshesIDs), meshesIDs.size()))
					{
						ar.SetMesh(ResourceFactory::Get().GetResource<RTE::Model>(meshesIDs[selectedMesh]));
					}

						ImGui::Separator();
						int selectedMaterial;
						if (ar.GetMaterial().GetResourceID() == "invalid") selectedMaterial = -1;

						auto iter2 = std::find(materialsIDs.begin(), materialsIDs.end(), ar.GetMaterial().GetResourceID());
						if (iter2 != materialsIDs.end()) selectedMaterial = std::distance(materialsIDs.begin(), iter2);

						if (ImGui::Combo("Materials", &selectedMaterial,
							[](void* vec, int idx, const char** out_text) {
								std::vector<std::string>* vector = reinterpret_cast<std::vector<std::string>*>(vec);
								if (idx < 0 || idx >= vector->size())return false;
								*out_text = vector->at(idx).c_str();
								return true;
							}, reinterpret_cast<void*>(&materialsIDs), materialsIDs.size()))
						{
							ar.SetMaterial(ResourceFactory::Get().GetResource<RTE::Material>(materialsIDs[selectedMaterial]));
						}



				}
			}
			if (scenePTR->GetRegistryPtr()->any_of<RTE::MeshRenderer>(selectedEN.ent)) {

				if (ImGui::CollapsingHeader("Material")) {
					auto& ar = Application::Get().scene.GetRegistryPtr()->get<MeshRenderer>(selectedEN.ent);
					if (ar.GetMaterial().matPtr.use_count() != 0)
						ar.GetMaterial().matPtr->DrawImGui();

				}
			}
			if (scenePTR->GetRegistryPtr()->any_of<RTE::LightComponent>(selectedEN.ent)) {
				if (ImGui::CollapsingHeader("Light component")) {

					auto& ar = scenePTR->GetRegistryPtr()->get<LightComponent>(selectedEN.ent);

					ImGui::DragFloat4("Color", &ar.lightComponent.Color.x, 0.1, 0, 1);

					ImGui::DragFloat("SpotlightAngle", &ar.lightComponent.SpotlightAngle, 0.1);
					ImGui::DragFloat("Range", &ar.lightComponent.Range, 0.1);

					ImGui::DragFloat("Intensity", &ar.lightComponent.Intensity, 0.1);
					ImGui::Checkbox("Enabled", (bool*)&ar.lightComponent.Enabled);

					static const char* items[]{ "POINT_LIGHT",	"SPOT_LIGHT",	"DIRECTIONAL_LIGHT" };
					static int selectedItem; selectedItem = ar.lightComponent.Type;
					if (ImGui::Combo("Light type", &selectedItem, items, IM_ARRAYSIZE(items))) {
						ar.lightComponent.Type = (Light::LightType)selectedItem;
					}
					

				}
			}
			if (scenePTR->GetRegistryPtr()->any_of<RTE::ScriptComponent>(selectedEN.ent)) {
				if (ImGui::CollapsingHeader("Script component")) {

					auto& sc = scenePTR->GetRegistryPtr()->get<ScriptComponent>(selectedEN.ent);
					char path[50] = { "none" };
					ImGui::InputText("Script path", path, 50);
					if (ImGui::Button("Reload")) {
						sc.attachScript(path);
					}
					
				}
			}
			if (scenePTR->GetRegistryPtr()->any_of<RTE::Camera>(selectedEN.ent)) {
				if (ImGui::CollapsingHeader("Camera")) {

					auto& sc = scenePTR->GetRegistryPtr()->get<Camera>(selectedEN.ent);
					ImGui::Checkbox("IsActive", sc.GetActivateFlagPtr());

				}
			}
			if (scenePTR->GetRegistryPtr()->any_of<RTE::Collider>(selectedEN.ent)) {
				if (ImGui::CollapsingHeader("Collider")) {

					auto& sc = scenePTR->GetRegistryPtr()->get<Collider>(selectedEN.ent);
					ImGui::DragFloat3("Box extends", &sc.boxExtends.x, 0.01);
					ImGui::DragFloat("Sphere radius", &sc.sphereRadius, 0.01);


					static const char* items[]{ "Sphere",	"Box" };
					static int selectedItem; selectedItem = sc.type;
					if (ImGui::Combo("Collider type", &selectedItem, items, IM_ARRAYSIZE(items))) {
						sc.type = (Collider::ColliderType)selectedItem;
					}

					ImGui::ColorPicker4("Collider color", &sc.color.x);

				}
			}



		}
		ImGui::End();

		cbflags = ImGuiWindowFlags_MenuBar;
		ImGui::Begin("World outliner", 0, cbflags);

		//ImGui::PushID("outliner");
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Add"))
			{
				if (ImGui::MenuItem("Add new entity"))
					scenePTR->CreateGameObject();

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Delete"))
			{
				if (ImGui::MenuItem("Delete selected entity")) {
					if (selectedEN.isValid == true) {
						scenePTR->GetRegistryPtr()->destroy(selectedEN.ent);
						selectedEN.isValid = false;
					}
				}

				ImGui::EndMenu();
			}
			/*if (ImGui::Button("Add new entity"))
			{
				scenePTR->CreateGameObject();
			}*/

			ImGui::EndMenuBar();
		}

		//ImGui::Separator();
		scenePTR->GetRegistryPtr()->each([&](auto entity) {
			bool isSelected = false;

			if (selectedEN.isValid && selectedEN.ent == entity)
				isSelected = true;

			unsigned int tmpInt = (unsigned int)entity;
			std::string tmp = std::to_string(tmpInt);




			if (ImGui::MenuItem(tmp.c_str(), "", &isSelected)) {
				selectedEN.ent = entity;
				selectedEN.isValid = true;
			}
			if (ImGui::BeginPopupContextItem()) // <-- use last item id as popup id
			{
				if (ImGui::Button("Delete")) {
					ImGui::CloseCurrentPopup();
					scenePTR->GetRegistryPtr()->destroy(entity);
					if (selectedEN.ent == entity) selectedEN.isValid = false;
				}
				ImGui::EndPopup();

				return;
			}

			//ImGui::Text("%u", entity);

			});

		//ImGui::PopID();

		ImGui::End();
		attachContentBrowserWindow();

	}

	void EditorLayer::OnEvent(RTE::Event& event)
	{
		//RTE_TRACE("{0}", event);
		RTE::EventDispatcher dispatcher(event);
		if (IsViewPortHowered) {
			dispatcher.Dispatch<RTE::MouseButtonPressedEvent>(std::bind(&EditorLayer::MousePressed, this, std::placeholders::_1));
		}
		if (IsViewPortPressed)
			dispatcher.Dispatch<RTE::MouseButtonReleasedEvent>(std::bind(&EditorLayer::ShowCursor, this, std::placeholders::_1));

	}


	void EditorLayer::UpdateCamera() {

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

	void EditorLayer::pickObject() {

		auto [mx, my] = ImGui::GetMousePos();
		mx = Input::GetMouseX();
		my = Input::GetMouseY();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		ImVec2 viewportSize(m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);
		//my = viewportSize.y - my;
		int sx = (int)mx;
		int sy = (int)my;

		//picking code
		XMFLOAT4X4 P; XMStoreFloat4x4(&P, camera->GetProjectionMatrix());
		// Compute picking ray in view space.
		float vx = (+2.0f * sx / viewportSize.x - 1.0f) / P(0, 0);
		float vy = (-2.0f * sy / viewportSize.y + 1.0f) / P(1, 1);
		// Ray definition in view space.
		XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMVECTOR rayDir = XMVectorSet(vx, vy, 1.0f, 0.0f);

		XMMATRIX V = camera->GetViewMatrix();
		auto d = XMMatrixDeterminant(V);
		XMMATRIX invView = XMMatrixInverse(nullptr, V);

		// Assume nothing is picked to start, so the picked render-item is invisible.
		selectedEN.isValid = false;

		auto view = Application::Get().scene.GetRegistryPtr()->view<RTE::Transform, RTE::MeshRenderer>();
		float tmin; DirectX::XMStoreFloat(&tmin, DirectX::g_XMInfinity);
		for (auto go : view)
		{
			auto obj = view.get<RTE::Transform, RTE::MeshRenderer>(go);

			auto& transform = std::get<0>(obj);
			auto& mr = std::get<1>(obj);
			XMMATRIX W = transform.GetMatrix();
			auto dd = XMMatrixDeterminant(W);
			XMMATRIX invWorld = XMMatrixInverse(nullptr, W);

			// Tranform ray to vi space of Mesh.
			XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);

			auto tmpOrigin = rayOrigin;
			auto tmpDir = rayDir;

			tmpOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
			tmpDir = XMVector3TransformNormal(rayDir, toLocal);


			// Make the ray direction unit length for the intersection tests.
			tmpDir = XMVector3Normalize(tmpDir);

			XMStoreFloat4(&origin, tmpOrigin);
			XMStoreFloat4(&dir, tmpDir);
			isDraw = true;

			float t = 0;

			if (mr.GetMesh().box.Intersects(tmpOrigin, tmpDir, t))
			{
				if (t < tmin) {
					selectedEN.isValid = true;
					selectedEN.ent = go;
				}

			}

		}

	}

	bool EditorLayer::MousePressed(RTE::MouseButtonPressedEvent ev) {
		if (ev.GetMouseButton() == RTE_MOUSE_BUTTON_2) {
			window->HideCursor();
			posX = RTE::Input::GetMouseX();
			posY = RTE::Input::GetMouseY();
			IsViewPortPressed = true;
		}
		else if (ev.GetMouseButton() == RTE_MOUSE_BUTTON_1) {
			if (IsViewPortHowered) pickObject();

		}
		return true;

	}

	bool EditorLayer::ShowCursor(RTE::MouseButtonReleasedEvent ev) {
		if (ev.GetMouseButton() == RTE_MOUSE_BUTTON_2) {
			window->ShowCursor();
			IsViewPortPressed = false;
		}
		return true;

	}

}