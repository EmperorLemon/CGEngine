#include "GUIContext.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ImGuizmo/ImGuizmo.h>

#include "Core/Logger.hpp"
#include "Core/Window.h"
#include "Scene/Scene.h"

#include "Utils/GUID.h"

#include "ECS/Component/Transform.h"
#include "ECS/Component/DrawObject.h"

#include "Math/Math.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace CGEngine
{
	static float MODEL_MATRIX[16] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	//static float  VIEW_MATRIX[16] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	static ImGuizmo::OPERATION currentGizmoOperation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE currentGizmoMode(ImGuizmo::LOCAL);

	static ImVec2 viewportBounds[2] = { {0, 0}, {0, 0} };

	static bool viewportFocused = false;
	static bool viewportHovered = false;

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
		style.WindowMinSize.x = 350.0f;

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

	static void EditTransform(Component::Transform& transform, const Camera& camera)
	{
		if (ImGui::RadioButton("Translate", currentGizmoOperation == ImGuizmo::TRANSLATE))
			currentGizmoOperation = ImGuizmo::TRANSLATE;

		ImGui::SameLine();

		if (ImGui::RadioButton("Rotate", currentGizmoOperation == ImGuizmo::ROTATE))
			currentGizmoOperation = ImGuizmo::ROTATE;

		ImGui::SameLine();

		if (ImGui::RadioButton("Scale", currentGizmoOperation == ImGuizmo::SCALE))
			currentGizmoOperation = ImGuizmo::SCALE;

		ImGuizmo::DecomposeMatrixToComponents(MODEL_MATRIX, Math::ToPtr(transform.position), Math::ToPtr(transform.rotation), Math::ToPtr(transform.scale));
		ImGui::DragFloat3("Translation", Math::ToPtr(transform.position), 0.1f, 0, 0, "%.2f");
		ImGui::DragFloat3("Rotation", Math::ToPtr(transform.rotation), 0.1f, 0, 0, "%.2f");
		ImGui::DragFloat3("Scale", Math::ToPtr(transform.scale), 0.1f, 0.0000001f, std::numeric_limits<float>::max(), "%.2f");
		ImGuizmo::RecomposeMatrixFromComponents(Math::ToArray(transform.position), Math::ToArray(transform.rotation), Math::ToArray(transform.scale), MODEL_MATRIX);

		if (currentGizmoOperation != ImGuizmo::SCALE)
		{
			if (ImGui::RadioButton("Local", currentGizmoMode == ImGuizmo::LOCAL))
				currentGizmoMode = ImGuizmo::LOCAL;

			ImGui::SameLine();

			if (ImGui::RadioButton("World", currentGizmoMode == ImGuizmo::WORLD))
				currentGizmoMode = ImGuizmo::WORLD;
		}

		Manipulate(Math::ToArray(camera.view), Math::ToArray(camera.projection), currentGizmoOperation, currentGizmoMode, MODEL_MATRIX, nullptr, nullptr, nullptr, nullptr);
	}

	void CreateViewport(const uint32_t viewportID)
	{
		CreateDockSpace();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		const auto& viewportMinRegion = ImGui::GetWindowContentRegionMin();
		const auto& viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		const auto& viewportOffset = ImGui::GetWindowPos();

		viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		viewportFocused = ImGui::IsWindowFocused();
		viewportHovered = ImGui::IsWindowHovered();

		const auto& viewportSize = ImGui::GetContentRegionAvail();

		ImGui::Image(reinterpret_cast<ImTextureID>(viewportID), viewportSize, ImVec2(0, 1), ImVec2(1, 0));

		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();

		ImGuizmo::SetRect(viewportBounds[0].x, viewportBounds[0].y, viewportBounds[1].x - viewportBounds[0].x, viewportBounds[1].y - viewportBounds[0].y);

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void CreateEditorWindow(Scene& scene)
	{
		auto& entities = scene.GetEntities();
		auto& camera = scene.GetMainCamera();

		{
			ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Appearing);
			ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Appearing);

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
					{
						if (ImGui::BeginChild("Entities"))
						{
							if (ImGui::CollapsingHeader("Main Camera"))
							{
								ImGui::DragFloat3(std::string("Position##" + std::string("Camera")).c_str(), Math::ToPtr(camera.position), 0.1f);

								ImGui::Text("Projection");
								ImGui::Text("[%f] [%f] [%f] [%f]", camera.projection[0][0], camera.projection[0][1], camera.projection[0][2], camera.projection[0][3]);
								ImGui::Text("[%f] [%f] [%f] [%f]", camera.projection[1][0], camera.projection[1][1], camera.projection[1][2], camera.projection[1][3]);
								ImGui::Text("[%f] [%f] [%f] [%f]", camera.projection[2][0], camera.projection[2][1], camera.projection[2][2], camera.projection[2][3]);
								ImGui::Text("[%f] [%f] [%f] [%f]", camera.projection[3][0], camera.projection[3][1], camera.projection[3][2], camera.projection[3][3]);

								ImGui::Text("View");
								ImGui::Text("[%f] [%f] [%f] [%f]", camera.view[0][0], camera.view[0][1], camera.view[0][2], camera.view[0][3]);
								ImGui::Text("[%f] [%f] [%f] [%f]", camera.view[1][0], camera.view[1][1], camera.view[1][2], camera.view[1][3]);
								ImGui::Text("[%f] [%f] [%f] [%f]", camera.view[2][0], camera.view[2][1], camera.view[2][2], camera.view[2][3]);
								ImGui::Text("[%f] [%f] [%f] [%f]", camera.view[3][0], camera.view[3][1], camera.view[3][2], camera.view[3][3]);
							}

							entities.Iterate<Utils::GUID, Component::Transform, Component::DrawObject>([&](const Utils::GUID& GUID, Component::Transform& transform, const Component::DrawObject& object)
								{
									if (ImGui::CollapsingHeader(std::string("Object##" + GUID.str()).c_str()))
									{
										EditTransform(transform, camera);
									}
								});
						}

						ImGui::EndChild();
					}
				}
			}

			ImGui::End();
		}

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
