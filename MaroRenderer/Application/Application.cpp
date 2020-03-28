#include "Application.h"

#define BIND_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

Application::Application()
{

	m_Window = new Window();
	m_Renderer = new Renderer();
	m_Shader = Shader::CreateShaderFromPath("Engine/Shaders/basic2d.vs", "Engine/Shaders/basic2d.fs");
	m_Camera = new ArcballCamera(m_Window->GetWidth(), m_Window->GetHeight());

	m_Window->SetEventCallback(BIND_FUNC(OnEvent));
}

Application::~Application()
{
}

void Application::Run()
{
	GUILayer layer(m_Window);
	layer.Attach();

	std::string path = "cube.obj";
	std::string filename = "Box_Texture.png";

	Model cube(path, filename);
	Actor cubeActor(&cube);
	Actor cubeActor2(&cube);
	cubeActor2.SetLocation(glm::vec3(2.0f, 0.0f, 0.0f));

	PointLight light(m_Shader, glm::vec3(0.0f, 1.0f, 2.0f));
	PointLight light2(m_Shader, glm::vec3(0.0f, -1.0f, -2.0f));


	m_Scene = new Scene(m_Camera);
	m_Scene->AddPointLight(&light);
	m_Scene->AddPointLight(&light2);

	m_Scene->AddActor(&cubeActor);
	m_Scene->AddActor(&cubeActor2);

	while (!m_Window->ShouldClose())
	{

		m_Scene->RotateLight(glfwGetTime());

		layer.Begin();
		layer.SetStyle(GUIStyle::Photoshop);

		m_Window->Update();

		cubeActor.SetColor(glm::vec3(layer.GetColor()[0], layer.GetColor()[1], layer.GetColor()[2]));

		m_Renderer->Clear();
		m_Scene->UpdateShaders();
		m_Scene->Render();

		layer.End();
	}
}

void Application::OnEvent(const Event& e)
{
	if (e.GetType() == EventType::KeyPress)
	{
		KeyPressedEvent* keypressed = (KeyPressedEvent*)&e;
		if (keypressed->GetKeyCode() == 76)
		{
			m_Scene->ToggleDirectionalLight();
			LOG_INFO("Toggled!");
		}
	}
	else if (e.GetType() == EventType::MouseButtonPress)
	{
		MousePressedEvent* mousepressed = (MousePressedEvent*)&e;
		int button = mousepressed->GetButton();
		int mods = mousepressed->GetMods();

		// Hard coded for now, update later ( 4 = ALT )
		if (button == 0 && mods == 4)
		{
			m_Camera->LeftMousePressed(true, mousepressed->GetX(), mousepressed->GetY());
		}
	}
	else if (e.GetType() == EventType::MouseButtonRelease)
	{
		MouseReleasedEvent* mousereleased = (MouseReleasedEvent*)&e;
		int button = mousereleased->GetButton();
		if (button == 0)
		{
			m_Camera->LeftMousePressed(false, mousereleased->GetX(), mousereleased->GetY());
		}
	}
	else if (e.GetType() == EventType::MouseMove)
	{
		MouseMovedEvent* mousemoved = (MouseMovedEvent*)&e;
		float x = mousemoved->GetX();
		float y = mousemoved->GetY();
		m_Camera->MouseMoved(x, y);
	}
	else if (e.GetType() == EventType::WindowResize)
	{
		WindowResizeEvent* windowrezize = (WindowResizeEvent*)&e;
		glViewport(0, 0, windowrezize->GetWidth(), windowrezize->GetHeight());
	}
	else if (e.GetType() == EventType::MouseScroll)
	{
		MouseScrolledEvent* mousescroll = (MouseScrolledEvent*)&e;
		int yOffset = mousescroll->GetOffsetY();
		if (yOffset != 0)
		{
			m_Camera->MouseScrolled(yOffset);
		}
	}
}
