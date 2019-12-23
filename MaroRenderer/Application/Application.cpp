#include "Application.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"
#include "GUI/GUILayer.h"

#define BIND_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

Application::Application()
{

	m_Window = new Window();
	m_Renderer = new Renderer();
	m_Shader = Shader::CreateShaderFromPath("Renderer/Shaders/basic2d.vs", "Renderer/Shaders/basic2d.fs");
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

	while (!m_Window->ShouldClose())
	{
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
		m_Shader->SetFloat3("lightPos", glm::vec3(1.2f, 1.0f, 2.0f));
		m_Shader->SetFloat3("viewPos", m_Camera->GetPosition());

		m_Renderer->Draw();

		m_Renderer->lightShader->Use();
		m_Renderer->lightShader->SetMat4("projection", m_ProjMat);
		m_Renderer->lightShader->SetMat4("view", m_ViewMat);
		m_Renderer->DrawLight();

		layer.End();
	}
}

void Application::OnEvent(const Event& e)
{
	if (e.GetType() == EventType::KeyPress)
	{
		KeyPressedEvent* keypressed = (KeyPressedEvent*)&e;
		std::cout << keypressed->GetKeyCode() << std::endl;
		LOG_INFO("Key Pressed.");
	}
	else if (e.GetType() == EventType::MouseButtonPress)
	{
		MousePressedEvent* mousepressed = (MousePressedEvent*)&e;
		int button = mousepressed->GetButton();
		if (button == 0)
		{
			m_Camera->LeftMousePressed(true);
		}
	}
	else if (e.GetType() == EventType::MouseButtonRelease)
	{
		MouseReleasedEvent* mousereleased = (MouseReleasedEvent*)&e;
		int button = mousereleased->GetButton();
		if (button == 0)
		{
			m_Camera->LeftMousePressed(false);
		}
	}
	else if (e.GetType() == EventType::MouseMove)
	{
		MouseMovedEvent* mousemoved = (MouseMovedEvent*)&e;
		float x = mousemoved->GetX();
		float y = mousemoved->GetY();
		m_Camera->MouseMoved(x, y);
		m_ViewMat = m_Camera->GetView();
	}
	else if (e.GetType() == EventType::WindowResize)
	{
		WindowResizeEvent* windowrezize = (WindowResizeEvent*)&e;
		glViewport(0, 0, windowrezize->GetWidth(), windowrezize->GetHeight());
	}
}
