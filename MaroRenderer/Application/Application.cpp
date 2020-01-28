#include "Application.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"
#include "GUI/GUILayer.h"
#include "Engine/Objects/Model.h"
#include "Engine/Objects/Light.h"
#include "Engine/Objects/Actor.h"
#include "Engine/Objects/Scene.h"

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
	Model cube(path);
	Actor cubeActor(&cube);
	Actor cubeActor2(&cube);
	cubeActor.SetLocation(glm::vec3(1.0f, 0.0f, 0.0f));
	cubeActor2.SetLocation(glm::vec3(-1.0f, 0.0f, 0.0f));
	Light light(glm::vec3(0.0f, 1.0f, 2.0f));

	Scene scene(m_Camera);
	scene.AddLight(&light);
	for (int i = -5; i <= 5; i++)
	{
		Actor* cubeAc = new Actor(&cube);
		cubeAc->SetLocation(glm::vec3(4 * cos(i * 1.0f), 4 * sin(i * 1.0f), 0.0f));
		scene.AddActor(cubeAc);
	}
	//scene.AddActor(&cubeActor);
	//scene.AddActor(&cubeActor2);

	while (!m_Window->ShouldClose())
	{
		cubeActor2.SetColor(glm::vec3(0.5f, 0.0f, 0.0f));
		scene.RotateLight(glfwGetTime());

		layer.Begin();
		layer.SetStyle(GUIStyle::Photoshop);

		m_Window->Update();

		cubeActor.SetColor(glm::vec3(layer.GetColor()[0], layer.GetColor()[1], layer.GetColor()[2]));

		m_Renderer->Clear();
		scene.UpdateShaders();
		scene.Render();

		layer.End();
	}
}

void Application::OnEvent(const Event& e)
{
	if (e.GetType() == EventType::KeyPress)
	{
		KeyPressedEvent* keypressed = (KeyPressedEvent*)&e;
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
