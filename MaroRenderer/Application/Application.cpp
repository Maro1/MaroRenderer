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
	m_Shader = new Shader(vShader, fShader);
	m_Camera = new PerspectiveCamera(glm::vec3(0.0f, -0.3f, 3.0f));

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

		m_ViewMat = glm::lookAt(m_Camera->GetLocation(), m_Camera->GetLocation() + m_Camera->GetFront(), m_Camera->GetUp());

		m_Shader->SetMat4("view", m_ViewMat);

		m_ProjMat = glm::perspective(glm::radians(45.0f), (float)m_Window->GetWidth() / (float)m_Window->GetHeight(), 0.1f, 100.0f);

		m_Shader->SetMat4("projection", m_ProjMat);

		m_Shader->SetMat4("model", m_ModelMat);

		m_Shader->SetFloat4("inColor", glm::vec4(layer.GetColor()[0], layer.GetColor()[1], layer.GetColor()[2], layer.GetColor()[3]));

		m_Renderer->Draw();
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
			bLeftMousePressed = true;
		}
	}
	else if (e.GetType() == EventType::MouseButtonRelease)
	{
		MouseReleasedEvent* mousereleased = (MouseReleasedEvent*)&e;
		int button = mousereleased->GetButton();
		if (button == 0)
		{
			bLeftMousePressed = false;
		}
	}
	else if (e.GetType() == EventType::MouseMove)
	{
		MouseMovedEvent* mousemoved = (MouseMovedEvent*)&e;
		float x = mousemoved->GetX();
		float y = mousemoved->GetY();
		if (bLeftMousePressed)
		{
			m_ModelMat = glm::rotate(m_ModelMat, (x - m_CurrMouseX) * glm::radians(0.5f), glm::vec3(0.0, 1.0, 0.0));
			m_ModelMat = glm::rotate(m_ModelMat, (y - m_CurrMouseY) * glm::radians(0.5f), glm::vec3(1.0, 0.0, 0.0));
		}
		m_CurrMouseX = x;
		m_CurrMouseY = y;
	}
	else if (e.GetType() == EventType::WindowResize)
	{
		WindowResizeEvent* windowrezize = (WindowResizeEvent*)&e;
		glViewport(0, 0, windowrezize->GetWidth(), windowrezize->GetHeight());
	}
}
