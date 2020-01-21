#include "Application.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"
#include "GUI/GUILayer.h"
#include "Engine/Objects/Model.h"
#include "Engine/Objects/Light.h"
#include "Engine/Objects/Actor.h"

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
	Light light(glm::vec3(0.0f, 1.0f, 2.0f));

	SceneNode scene;
	scene.AddChild(&light);
	scene.AddChild(&cubeActor);

	while (!m_Window->ShouldClose())
	{
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
		transform = glm::rotate(transform, (float) log(glfwGetTime()) * 0.005f, glm::vec3(0.0f, 1.0f, 0.0f)); 
		transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));

		light.SetLocation(transform * glm::vec4(light.GetLocation(), 1.0f));

		m_ViewMat = m_Camera->GetView();

		layer.Begin();
		layer.SetStyle(GUIStyle::Photoshop);

		m_Window->Update();

		m_Shader->Use();

		m_Shader->SetMat4("view", m_ViewMat);

		m_ProjMat = glm::perspective(glm::radians(45.0f), (float)m_Window->GetWidth() / (float)m_Window->GetHeight(), 0.1f, 100.0f);

		m_Shader->SetMat4("projection", m_ProjMat);

		m_Shader->SetMat4("model", m_ModelMat);

		m_Shader->SetFloat3("inColor", glm::vec3(layer.GetColor()[0], layer.GetColor()[1], layer.GetColor()[2]));

		m_Shader->SetFloat3("lightColor", glm::vec3(1.0f));
		m_Shader->SetFloat3("lightPos", light.GetLocation());
		m_Shader->SetFloat3("viewPos", m_Camera->GetPosition());

		m_Renderer->Draw();
		cube.Draw(m_Shader);

		light.GetShader()->Use();
		light.GetShader()->SetMat4("projection", m_ProjMat);
		light.GetShader()->SetMat4("view", m_ViewMat);
		light.GetShader()->SetMat4("model", light.GetModelMatrix());
		scene.Update();


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
