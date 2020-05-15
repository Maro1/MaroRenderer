#include "GUILayer.h"

#include "Application/File/FileHandler.h"

GUILayer::GUILayer(Window* window, Application* app)
{
	m_Window = window;
	m_App = app;
	m_WindowHeight = m_App->GetWindow()->GetHeight();
	m_WindowWidth = m_App->GetWindow()->GetWidth();
}

void GUILayer::Attach()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(m_Window->GetGLFWwindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void GUILayer::Detach()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void GUILayer::Begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	CreateDockspace();

	DisplayMainMenu();

	DisplayConsole();


	ImGui::Begin("Hierarchy");
	DisplayHierarchy(&m_App->GetScene()->GetRoot()->GetChildren());
	ImGui::End();

	DisplayTabs();

	DisplayViewport();

}

void GUILayer::End()
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float) m_Window->GetWidth(), (float) m_Window->GetHeight());

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUILayer::SetStyle(GUIStyle style)
{
	if (style == GUIStyle::Darcula)
	{
		DarculaStyle();
	}
	else if (style == GUIStyle::Photoshop)
	{
		PhotoshopStyle();
	}
}

void GUILayer::DisplayTabs()
{
	ImGui::Begin("Properties");
	ImGui::SetWindowSize(ImVec2(300, m_WindowHeight), ImGuiCond_Once);
	ImGui::SetWindowPos(ImVec2(m_WindowWidth-600, 20), ImGuiCond_Once);
	if (ImGui::BeginTabBar("Properties"))
	{
		if (ImGui::BeginTabItem("Transform"))
		{
			static float vec4a[4] = { m_ActiveNode->GetLocation().x, m_ActiveNode->GetLocation().y, m_ActiveNode->GetLocation().z, 0.44f };
			
			ImGui::Text("Location: ");
			ImGui::SameLine();
			ImGui::InputFloat3("", vec4a, "%.3f");
			char buffer[20] = "";
			ImGui::InputText("Test: ", buffer, IM_ARRAYSIZE(buffer));
			ImGui::Text("Rotation");
			ImGui::Text("Scale");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Material"))
		{
			ImGui::Text("Albedo");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Lighting"))
		{
			ImGui::Text("Directional light");
			ImGui::SliderFloat("Strength", m_App->GetScene()->GetDirectionalLightStrength(), 0, 1);
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
}

void GUILayer::DisplayViewport()
{
	ImGui::SetWindowSize(ImVec2(m_Window->GetWidth() / 2 + 10, m_Window->GetHeight() / 2 + 10), ImGuiCond_Once);
	ImGui::SetWindowPos(ImVec2(m_Window->GetWidth() / 4, m_Window->GetHeight() / 4),  ImGuiCond_Once);
	ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
	{
		// Get FBO texture
		unsigned int tex = m_App->GetRenderer()->GetFBOTexture();

		// Draw texture to viewport
		ImGui::Image((void*)(intptr_t)tex, ImVec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y), ImVec2(0, 1), ImVec2(1, 0));

		if (ImGui::IsWindowFocused()) {
			m_App->SetViewPortFocused(true);
		}
		else
		{
			m_App->SetViewPortFocused(false);
		}
		m_App->GetRenderer()->UpdateFrameBuffer(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
	}
	ImGui::End();
}

void GUILayer::DisplayConsole()
{
	ImGui::Begin("Console");
	// fill at later time
	ImGui::End();
}

void GUILayer::CreateDockspace()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->GetWorkPos());
	ImGui::SetNextWindowSize(viewport->GetWorkSize());
	ImGui::SetNextWindowViewport(viewport->ID);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	ImGui::Begin("MyDockSpace", NULL, window_flags);

	ImGuiID dockspaceID = ImGui::GetID("MyDockSpace");
	if (!ImGui::DockBuilderGetNode(dockspaceID)) {
		ImGui::DockBuilderRemoveNode(dockspaceID);
		ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_None);

		ImGuiID dock_main_id = dockspaceID;
		ImGuiID dock_up_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.5f, nullptr, &dock_main_id);
		ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.2f, nullptr, &dock_main_id);
		ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.2f, nullptr, &dock_main_id);
		ImGuiID dock_down_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.1f, nullptr, &dock_main_id);
		ImGuiID dock_middle_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_None, 0.2f, nullptr, &dock_main_id);

		ImGui::DockBuilderDockWindow("Properties", dock_right_id);
		ImGui::DockBuilderDockWindow("Hierarchy", dock_left_id);
		ImGui::DockBuilderDockWindow("Console", dock_down_id);
		ImGui::DockBuilderDockWindow("Viewport", dock_up_id);

		// Disable tab bar for custom toolbar
		ImGuiDockNode* node = ImGui::DockBuilderGetNode(dock_up_id);
		node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
		node->LocalFlags |= ImGuiDockNodeFlags_NoWindowMenuButton;
		node->LocalFlags |= ImGuiDockNodeFlags_NoCloseButton;

		ImGui::DockBuilderFinish(dock_main_id);
	}

	ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f));

	ImGui::End();
}

void GUILayer::DisplayHierarchy(std::vector<SceneNode*>* children)
{
	ImGui::SetWindowSize(ImVec2(m_WindowWidth / 10, m_WindowHeight), ImGuiCond_Once);
	ImGui::SetWindowPos(ImVec2(0, 20), ImGuiCond_Once);

	for (std::vector<SceneNode*>::const_iterator object = (*children).begin(); object != (*children).end(); ++object)
	{
		unsigned int flags = ImGuiTreeNodeFlags_OpenOnArrow;

		if ((*object)->GetChildren().size() == 0)
			flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

		ImGui::PushID((*object)->GetUUID());
		bool open = ImGui::TreeNodeEx((*object)->GetLabel().c_str(), flags);
		if (ImGui::IsItemClicked()) {
			m_ActiveNode = *object;
			m_App->GetCamera()->Target((*object)->GetLocation());
		}

		bool hasChildren = (*object)->GetChildren().size() > 0;
		ImGui::PopID();

		if (hasChildren && open)
		{
			DisplayHierarchy(&(*object)->GetChildren());
			ImGui::TreePop();
		}
	}
}

void GUILayer::ImportModel()
{
	std::string filePath = FileHandler::ShowOpenFileDialog(FileHandler::FileType::MESH_FILE);
	// Remember to delete these
	Model* model = new Model(filePath);
	Actor* actor = new Actor(model);
	m_App->GetScene()->AddActor(actor);
	m_ActiveNode = actor;
}

void GUILayer::ImportTexture()
{
}

void GUILayer::DisplayMainMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Import model")) {}
			if (ImGui::IsItemClicked())
			{
				ImportModel();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}


void GUILayer::DarculaStyle()
{
	ImGuiStyle* style = &ImGui::GetStyle();
	style->WindowRounding = 5.3f;
	style->GrabRounding = style->FrameRounding = 2.3f;
	style->ScrollbarRounding = 5.0f;
	style->FrameBorderSize = 1.0f;
	style->ItemSpacing.y = 6.5f;

	style->Colors[ImGuiCol_Text] = { 0.73333335f, 0.73333335f, 0.73333335f, 1.00f };
	style->Colors[ImGuiCol_TextDisabled] = { 0.34509805f, 0.34509805f, 0.34509805f, 1.00f };
	style->Colors[ImGuiCol_WindowBg] = { 0.23529413f, 0.24705884f, 0.25490198f, 0.94f };
	style->Colors[ImGuiCol_ChildBg] = { 0.23529413f, 0.24705884f, 0.25490198f, 0.00f };
	style->Colors[ImGuiCol_PopupBg] = { 0.23529413f, 0.24705884f, 0.25490198f, 0.94f };
	style->Colors[ImGuiCol_Border] = { 0.33333334f, 0.33333334f, 0.33333334f, 0.50f };
	style->Colors[ImGuiCol_BorderShadow] = { 0.15686275f, 0.15686275f, 0.15686275f, 0.00f };
	style->Colors[ImGuiCol_FrameBg] = { 0.16862746f, 0.16862746f, 0.16862746f, 0.54f };
	style->Colors[ImGuiCol_FrameBgHovered] = { 0.453125f, 0.67578125f, 0.99609375f, 0.67f };
	style->Colors[ImGuiCol_FrameBgActive] = { 0.47058827f, 0.47058827f, 0.47058827f, 0.67f };
	style->Colors[ImGuiCol_TitleBg] = { 0.04f, 0.04f, 0.04f, 1.00f };
	style->Colors[ImGuiCol_TitleBgCollapsed] = { 0.16f, 0.29f, 0.48f, 1.00f };
	style->Colors[ImGuiCol_TitleBgActive] = { 0.00f, 0.00f, 0.00f, 0.51f };
	style->Colors[ImGuiCol_MenuBarBg] = { 0.27058825f, 0.28627452f, 0.2901961f, 0.80f };
	style->Colors[ImGuiCol_ScrollbarBg] = { 0.27058825f, 0.28627452f, 0.2901961f, 0.60f };
	style->Colors[ImGuiCol_ScrollbarGrab] = { 0.21960786f, 0.30980393f, 0.41960788f, 0.51f };
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = { 0.21960786f, 0.30980393f, 0.41960788f, 1.00f };
	style->Colors[ImGuiCol_ScrollbarGrabActive] = { 0.13725491f, 0.19215688f, 0.2627451f, 0.91f };
	style->Colors[ImGuiCol_CheckMark] = { 0.90f, 0.90f, 0.90f, 0.83f };
	style->Colors[ImGuiCol_SliderGrab] = { 0.70f, 0.70f, 0.70f, 0.62f };
	style->Colors[ImGuiCol_SliderGrabActive] = { 0.30f, 0.30f, 0.30f, 0.84f };
	style->Colors[ImGuiCol_Button] = { 0.33333334f, 0.3529412f, 0.36078432f, 0.49f };
	style->Colors[ImGuiCol_ButtonHovered] = { 0.21960786f, 0.30980393f, 0.41960788f, 1.00f };
	style->Colors[ImGuiCol_ButtonActive] = { 0.13725491f, 0.19215688f, 0.2627451f, 1.00f };
	style->Colors[ImGuiCol_Header] = { 0.33333334f, 0.3529412f, 0.36078432f, 0.53f };
	style->Colors[ImGuiCol_HeaderHovered] = { 0.453125f, 0.67578125f, 0.99609375f, 0.67f };
	style->Colors[ImGuiCol_HeaderActive] = { 0.47058827f, 0.47058827f, 0.47058827f, 0.67f };
	style->Colors[ImGuiCol_Separator] = { 0.31640625f, 0.31640625f, 0.31640625f, 1.00f };
	style->Colors[ImGuiCol_SeparatorHovered] = { 0.31640625f, 0.31640625f, 0.31640625f, 1.00f };
	style->Colors[ImGuiCol_SeparatorActive] = { 0.31640625f, 0.31640625f, 0.31640625f, 1.00f };
	style->Colors[ImGuiCol_ResizeGrip] = { 1.00f, 1.00f, 1.00f, 0.85f };
	style->Colors[ImGuiCol_ResizeGripHovered] = { 1.00f, 1.00f, 1.00f, 0.60f };
	style->Colors[ImGuiCol_ResizeGripActive] = { 1.00f, 1.00f, 1.00f, 0.90f };
	style->Colors[ImGuiCol_PlotLines] = { 0.61f, 0.61f, 0.61f, 1.00f };
	style->Colors[ImGuiCol_PlotLinesHovered] = { 1.00f, 0.43f, 0.35f, 1.00f };
	style->Colors[ImGuiCol_PlotHistogram] = { 0.90f, 0.70f, 0.00f, 1.00f };
	style->Colors[ImGuiCol_PlotHistogramHovered] = { 1.00f, 0.60f, 0.00f, 1.00f };
	style->Colors[ImGuiCol_TextSelectedBg] = { 0.18431373f, 0.39607847f, 0.79215693f, 0.90f };
}

void GUILayer::PhotoshopStyle()
{
	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	colors[ImGuiCol_Text] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.500f, 0.500f, 0.500f, 1.000f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.280f, 0.280f, 0.280f, 0.000f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
	colors[ImGuiCol_Border] = ImVec4(0.266f, 0.266f, 0.266f, 1.000f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.200f, 0.200f, 0.200f, 1.000f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.280f, 0.280f, 0.280f, 1.000f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.277f, 0.277f, 0.277f, 1.000f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.300f, 0.300f, 0.300f, 1.000f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_Button] = ImVec4(1.000f, 1.000f, 1.000f, 0.000f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
	colors[ImGuiCol_ButtonActive] = ImVec4(1.000f, 1.000f, 1.000f, 0.391f);
	colors[ImGuiCol_Header] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(1.000f, 1.000f, 1.000f, 0.250f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.670f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_Tab] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.352f, 0.352f, 0.352f, 1.000f);
	colors[ImGuiCol_TabActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);

	style->ChildRounding = 4.0f;
	style->FrameBorderSize = 1.0f;
	style->FrameRounding = 2.0f;
	style->GrabMinSize = 7.0f;
	style->PopupRounding = 2.0f;
	style->ScrollbarRounding = 12.0f;
	style->ScrollbarSize = 13.0f;
	style->TabBorderSize = 1.0f;
	style->TabRounding = 0.0f;
	style->WindowRounding = 4.0f;
}
