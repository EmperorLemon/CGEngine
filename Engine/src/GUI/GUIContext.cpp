#include "GUIContext.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ImGuizmo/ImGuizmo.h>

#include "Core/Logger.hpp"
#include "Core/Window.h"
#include "Scene/Scene.h"

#include "ECS/Entity/Entity.hpp"

#include "ECS/Component/Light.h"
#include "ECS/Component/Tag.h"
#include "ECS/Component/Transform.h"

#include "Math/Math.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace CGEngine
{
	static ImGuizmo::OPERATION currentGizmoOperation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE currentGizmoMode(ImGuizmo::LOCAL);

	static ImVec2 viewportBounds[2] = { {0, 0}, {0, 0} };

	static bool viewportFocused = false;
	static bool viewportHovered = false;

	static bool useDockSpace = false;

	void CreateGUIContext(const Window& window)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(window.window), true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	static void CreateDockSpace()
	{
		useDockSpace = true;

		static bool dockspaceOpen = true;
		static bool fullscreen_cond = true;
		const bool fullscreen = fullscreen_cond;

		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		static ImGuiWindowFlags   window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		const auto viewport = ImGui::GetMainViewport();

		if (fullscreen)
		{
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);

			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);

			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Dockspace", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (fullscreen)
			ImGui::PopStyleVar(2);

		ImGuiStyle& style = ImGui::GetStyle();

		const float minWindowSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 250.0f;

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			const auto dockspace_id = ImGui::GetID("Dockspace_0");
			ImGui::DockSpace(dockspace_id, ImVec2(0, 0), dockspace_flags);
		}

		style.WindowMinSize.x = minWindowSizeX;
	}

	void BeginGUIFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	static void EditTransform(const bool useScale, Component::Transform& transform, const Camera& camera)
	{
		ImGui::SeparatorText("Transform");

		if (ImGui::RadioButton("Translate", currentGizmoOperation == ImGuizmo::TRANSLATE))
			currentGizmoOperation = ImGuizmo::TRANSLATE;

		ImGui::SameLine();

		if (ImGui::RadioButton("Rotate", currentGizmoOperation == ImGuizmo::ROTATE))
			currentGizmoOperation = ImGuizmo::ROTATE;

		ImGui::SameLine();

		if (useScale)
		{
			if (ImGui::RadioButton("Scale", currentGizmoOperation == ImGuizmo::SCALE))
				currentGizmoOperation = ImGuizmo::SCALE;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		if (currentGizmoOperation != ImGuizmo::SCALE)
		{
			if (ImGui::RadioButton("Local", currentGizmoMode == ImGuizmo::LOCAL))
				currentGizmoMode = ImGuizmo::LOCAL;

			ImGui::SameLine();

			if (ImGui::RadioButton("World", currentGizmoMode == ImGuizmo::WORLD))
				currentGizmoMode = ImGuizmo::WORLD;
		}

		ImGui::DragFloat3("##Translation", Math::ToPtr(transform.position), 0.1f, 0, 0, "%.2f");
		ImGui::DragFloat3("##Rotation", Math::ToPtr(transform.rotation), 0.1f, 0, 0, "%.2f");
		if (useScale) ImGui::DragFloat3("##Scale", Math::ToPtr(transform.scale), 0.1f, 0.0000001f, std::numeric_limits<float>::max(), "%.2f");
		ImGuizmo::RecomposeMatrixFromComponents(Math::ToArray(transform.position), Math::ToArray(transform.rotation), Math::ToArray(transform.scale), Math::ToPtr(transform.model));

		Manipulate(Math::ToArray(camera.view), Math::ToArray(camera.projection), currentGizmoOperation, currentGizmoMode, Math::ToPtr(transform.model), nullptr, nullptr, nullptr, nullptr);

		if (ImGuizmo::IsUsing())
			ImGuizmo::DecomposeMatrixToComponents(Math::ToArray(transform.model), Math::ToPtr(transform.position), Math::ToPtr(transform.rotation), Math::ToPtr(transform.scale));

		ImGui::Spacing();
	}

	static void EditLight(Component::Light& light)
	{
		ImGui::SeparatorText("Light");

		ImGui::ColorEdit4("Diffuse Color", Math::ToPtr(light.diffuseColor));
		ImGui::ColorEdit4("Specular Color", Math::ToPtr(light.specularColor));

		const char* types[3] = { "Directional", "Point", "Spot" };
		const char* preview_value = types[static_cast<uint32_t>(light.type)];

		if (ImGui::BeginCombo("Light Type", preview_value))
		{
			for (uint32_t n = 0; n < IM_ARRAYSIZE(types); ++n)
			{
				const bool is_selected = light.type == static_cast<Component::LightType>(n);

				if (ImGui::Selectable(types[n], is_selected))
					light.type = static_cast<Component::LightType>(n);
			}

			ImGui::EndCombo();
		}

		ImGui::Spacing();

		if (light.type == Component::LightType::POINT_LIGHT || light.type == Component::LightType::SPOT_LIGHT)
		{
			ImGui::DragFloat("Constant", &light.constant, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Linear", &light.linear , 0.01f, 0.014f, 0.7f);
			ImGui::DragFloat("Quadratic", &light.quadratic, 0.01f, 0.00007f, 1.8f);

			ImGui::Spacing();

			if (light.type == Component::LightType::SPOT_LIGHT)
			{
				static float inner = 12.5f, outer = 15.0f;

				ImGui::DragFloat("Inner Spotlight Angle", &inner, 0.01f, 0.0f, outer);
				ImGui::DragFloat("Outer Spotlight Angle", &outer, 0.01f, inner, 180.0f);

				light.innerSpotAngle = Math::Cos(inner);
				light.outerSpotAngle = Math::Cos(outer);
			}
		}
	}

	void CreateViewport(const uint32_t colorID, const uint32_t depthID)
	{
		//CreateDockSpace();

		//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		//ImGui::Begin("Viewport Color");

		//const auto& viewportMinRegion = ImGui::GetWindowContentRegionMin();
		//const auto& viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		//const auto& viewportOffset = ImGui::GetWindowPos();

		//viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		//viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		//viewportFocused = ImGui::IsWindowFocused();
		//viewportHovered = ImGui::IsWindowHovered();

		//const auto& viewportSize = ImGui::GetContentRegionAvail();

		//ImGui::Image(reinterpret_cast<ImTextureID>(colorID), viewportSize, ImVec2(0, 1), ImVec2(1, 0));

		//ImGuizmo::SetOrthographic(false);
		//ImGuizmo::SetDrawlist();

		//ImGuizmo::SetRect(viewportBounds[0].x, viewportBounds[0].y, viewportBounds[1].x - viewportBounds[0].x, viewportBounds[1].y - viewportBounds[0].y);

		//ImGui::End();
		//ImGui::PopStyleVar();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Depth");

		ImGui::Image(reinterpret_cast<ImTextureID>(depthID), ImVec2(1024, 1024), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void CreateEditorWindow(Scene& scene)
	{
		//auto& entities = scene.GetEntities();
		auto& camera = scene.GetMainCamera();

		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);

		if (ImGui::Begin("Editor Window"))
		{
			if (ImGui::IsKeyPressed(ImGuiKey_W))
				currentGizmoOperation = ImGuizmo::TRANSLATE;
			if (ImGui::IsKeyPressed(ImGuiKey_E))
				currentGizmoOperation = ImGuizmo::ROTATE;
			if (ImGui::IsKeyPressed(ImGuiKey_R))
				currentGizmoOperation = ImGuizmo::SCALE;

			if (ImGui::CollapsingHeader("Scene Hierarchy"))
			{
				if (ImGui::BeginChild("Entities"))
				{
					if (ImGui::CollapsingHeader("Main Camera"))
					{
						ImGui::DragFloat3(std::string("Position##" + std::string("Camera")).c_str(), Math::ToPtr(camera.position), 0.1f);

						//ImGui::Text("Projection");
						//ImGui::Text("[%f] [%f] [%f] [%f]", camera.projection[0][0], camera.projection[0][1], camera.projection[0][2], camera.projection[0][3]);
						//ImGui::Text("[%f] [%f] [%f] [%f]", camera.projection[1][0], camera.projection[1][1], camera.projection[1][2], camera.projection[1][3]);
						//ImGui::Text("[%f] [%f] [%f] [%f]", camera.projection[2][0], camera.projection[2][1], camera.projection[2][2], camera.projection[2][3]);
						//ImGui::Text("[%f] [%f] [%f] [%f]", camera.projection[3][0], camera.projection[3][1], camera.projection[3][2], camera.projection[3][3]);

						//ImGui::Text("View");
						//ImGui::Text("[%f] [%f] [%f] [%f]", camera.view[0][0], camera.view[0][1], camera.view[0][2], camera.view[0][3]);
						//ImGui::Text("[%f] [%f] [%f] [%f]", camera.view[1][0], camera.view[1][1], camera.view[1][2], camera.view[1][3]);
						//ImGui::Text("[%f] [%f] [%f] [%f]", camera.view[2][0], camera.view[2][1], camera.view[2][2], camera.view[2][3]);
						//ImGui::Text("[%f] [%f] [%f] [%f]", camera.view[3][0], camera.view[3][1], camera.view[3][2], camera.view[3][3]);
					}


					for (auto& entity : EntityList::GetEntities())
					{
						const auto& tag = entity.GetComponent<Component::Tag>();
						auto& transform = entity.GetComponent<Component::Transform>();
;
						if (ImGui::CollapsingHeader((tag.name + "##" + tag.guid.str()).c_str()))
						{
							ImGui::PushID(tag.guid.str().c_str());

							EditTransform(tag.type != Component::EntityType::LIGHT ? true : false, transform, camera);

							if (entity.HasComponent<Component::Light>())
							{
								auto& light = entity.GetComponent<Component::Light>();

								EditLight(light);
							}

							ImGui::PopID();
						}
					}
				}

				ImGui::EndChild();
			}
		}

		ImGui::End();

		if (useDockSpace)
			ImGui::End();
	}

	void EndGUIFrame()
	{
		ImGui::Render();

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			const auto context = OpenGL::GetContext();

			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();

			OpenGL::SetContext(context);
		}
	}

	void DrawGUI()
	{
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void DestroyGUIContext()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}
