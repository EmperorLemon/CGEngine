#include "Core/Application.h"

#include "GUI/GUIContext.h"

#include "ECS/Component/Transform.h"
#include "ECS/Component/DrawObject.h"

namespace CGEngine
{
#define BIND_EVENT_FN(x) [this](auto&&... args) -> decltype(auto) { return this->x(std::forward<decltype(args)>(args)...); }

	Application::Application(const ApplicationCreateInfo& appInfo [[maybe_unused]]) : m_logger(Logger("APP"))
	{
		m_sceneManager.AddScene(Scene("Default Scene"));

		CreateWindow({ 800, 800,"Default Window" }, m_window);

		m_window.eventCallback = BIND_EVENT_FN(OnEvent);

		m_renderer = std::make_unique<Renderer>(Renderer({ GraphicsAPI::CG_OPENGL_API, m_window }));

		CreateGUIContext(m_window);
	}

	void Application::Run()
	{
		m_time.Start();

		auto& defaultScene = m_sceneManager.DefaultScene();
		defaultScene.SetupScene();

		auto& entities = defaultScene.GetEntities();

		auto& main_camera = defaultScene.GetMainCamera();
		m_renderer->PreRender(main_camera);

		while (!WindowClosed(m_window))
		{
			PollEvents();

			m_time.Update();

			BeginGUIFrame();

			CreateViewport(m_renderer->GetViewportID());
			CreateEditorWindow(defaultScene);

			EndGUIFrame();

			m_renderer->FirstPass();

			m_renderer->Update(main_camera, m_time);

			int32_t offset = 0;
			entities.Iterate<Component::Transform>([&](const Component::Transform& transform)
			{
				m_renderer->UpdateTransform(offset, transform);
				offset++;
			});

			entities.Iterate<Component::Transform, Component::DrawObject>([&](const Component::Transform& transform, const Component::DrawObject& object)
			{
				m_renderer->RenderPrimitive(transform, object);
			});

			m_renderer->SecondPass();

			DrawGUI();

			m_renderer->PostRender();
		}
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		if (event.handled) return;

		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResized));
		dispatcher.Dispatch<FramebufferResizeEvent>(BIND_EVENT_FN(OnFramebufferResized));
	}

	bool Application::OnWindowResized(const WindowResizeEvent& event)
	{
		m_renderer->ResizeProjection(event.GetWidth(), event.GetHeight(), m_sceneManager.DefaultScene().GetMainCamera());

		return true;
	}

	bool Application::OnFramebufferResized(const FramebufferResizeEvent& event) const
	{
		m_renderer->ResizeViewport(event.GetWidth(), event.GetHeight());

		return true;
	}

	void Application::Quit() const
	{
		DestroyGUIContext();
		DestroyWindow(m_window);
	}
}