#include "Core/Application.h"

#include "GUI/GUIContext.h"

#include "ECS/Component/DrawObject.h"
#include "ECS/Component/Instance.h"
#include "ECS/Component/Light.h"
#include "ECS/Component/Transform.h"

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

			CreateViewport(m_renderer->GetHDRSceneTextureID(), m_renderer->GetGlowTextureID(), m_renderer->GetDepthTextureID());
			CreateEditorWindow(defaultScene);

			EndGUIFrame();

			m_renderer->UpdateUniforms(main_camera, m_time);

			{
				int32_t offset = 0;

				entities.Iterate<Component::Transform, Component::Instance>([&](const Component::Transform& transform, const Component::Instance& instance [[maybe_unused]])
				{
					m_renderer->UpdateInstance(offset, transform);
					offset++;
				});
			}

			{
				int32_t offset = 0;

				const auto num_lights = static_cast<uint32_t>(entities.Count<Component::Light>());

				entities.Iterate<Component::Transform, Component::Light>([&](const Component::Transform& transform, Component::Light& light)
				{
					if (light.type == Component::LightType::DIRECTIONAL_LIGHT)
						light.direction = Math::Vec4(Math::DegToRad(transform.rotation), 0.0f);
					else
						light.direction = Math::Vec4(transform.position, 0.0f);

					m_renderer->UpdateLight(offset, num_lights, light);
					offset++;
				});
			}

			m_renderer->EnableShader(ShaderEnable::DEPTH_SHADER);

			m_renderer->FirstPass();

			entities.Iterate<Component::DrawObject>([&](const Component::DrawObject& primitive)
			{
				m_renderer->RenderPrimitive(primitive);
			});

			m_renderer->SecondPass();

			m_renderer->EnableShader(ShaderEnable::DEFAULT_SHADER);

			entities.Iterate<Component::DrawObject>([&](const Component::DrawObject& primitive)
			{
				m_renderer->RenderPrimitive(primitive);
			});

			m_renderer->EnableShader(ShaderEnable::LIGHT_SHADER);

			entities.Iterate<Component::Transform, Component::Light>([&](const Component::Transform& transform, const Component::Light& light)
			{
				m_renderer->DrawLight(transform, light);
			});

			m_renderer->ThirdPass();

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