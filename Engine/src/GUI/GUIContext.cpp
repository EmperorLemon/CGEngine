#include "GUIContext.h"

#include <imgui.h>
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

namespace CGEngine
{
	void CreateGUIContext(const Window& window)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); static_cast<void>(io);
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(window.window), true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	void BeginGUIFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void CreateEditorWindow(Scene& scene)
	{
		auto& entities = scene.GetEntities();
		auto& camera = scene.GetMainCamera();

		if (ImGui::Begin("Editor Window"))
		{
			if (ImGui::CollapsingHeader("Scene Hierarchy"))
			{
				ImGui::BeginChild("Entities");
				if (ImGui::CollapsingHeader("Main Camera"))
				{
					ImGui::DragFloat3(std::string("Position##" + std::string("Camera")).c_str(), &camera.position.x, 0.1f);

					ImGui::Text("Projection");
					ImGui::Text("[%f] [%f] [%f] [%f]", camera.projection[0],  camera.projection[1],  camera.projection[2],   camera.projection[3]);
					ImGui::Text("[%f] [%f] [%f] [%f]", camera.projection[4],  camera.projection[5],  camera.projection[6],   camera.projection[7]);
					ImGui::Text("[%f] [%f] [%f] [%f]", camera.projection[8],  camera.projection[9],  camera.projection[10],  camera.projection[11]);
					ImGui::Text("[%f] [%f] [%f] [%f]", camera.projection[12], camera.projection[13], camera.projection[14],  camera.projection[15]);

					ImGui::Spacing();

					ImGui::Text("View");
					ImGui::Text("[%f] [%f] [%f] [%f]", camera.view[0],  camera.view[1],  camera.view[2],  camera.view[3]);
					ImGui::Text("[%f] [%f] [%f] [%f]", camera.view[4],  camera.view[5],  camera.view[6],  camera.view[7]);
					ImGui::Text("[%f] [%f] [%f] [%f]", camera.view[8],  camera.view[9],  camera.view[10], camera.view[11]);
					ImGui::Text("[%f] [%f] [%f] [%f]", camera.view[12], camera.view[13], camera.view[14], camera.view[15]);
				}

				ImGui::Spacing();

				entities.Iterate<Utils::GUID, Component::Transform, Component::DrawObject>([&](const Utils::GUID& GUID, Component::Transform& transform, const Component::DrawObject& object)
					{
						if (ImGui::CollapsingHeader(std::string("Object##" + GUID.str()).c_str()))
						{
							static ImGuizmo::OPERATION currentGizmoOperation(ImGuizmo::TRANSLATE);
							static ImGuizmo::MODE currentGizmoMode(ImGuizmo::WORLD);

							if (ImGui::IsKeyPressed(ImGuiKey_W))
								currentGizmoOperation = ImGuizmo::TRANSLATE;
							if (ImGui::IsKeyPressed(ImGuiKey_E))
								currentGizmoOperation = ImGuizmo::ROTATE;
							if (ImGui::IsKeyPressed(ImGuiKey_R))
								currentGizmoOperation = ImGuizmo::SCALE;

							if (ImGui::RadioButton("Translate", currentGizmoOperation == ImGuizmo::TRANSLATE))
								currentGizmoOperation = ImGuizmo::TRANSLATE;
							ImGui::SameLine();
							if (ImGui::RadioButton("Rotate", currentGizmoOperation == ImGuizmo::ROTATE))
								currentGizmoOperation = ImGuizmo::ROTATE;
							ImGui::SameLine();
							if (ImGui::RadioButton("Scale", currentGizmoOperation == ImGuizmo::SCALE))
								currentGizmoOperation = ImGuizmo::SCALE;

							ImGui::InputFloat3("Translation", Math::ToPtr(transform.position), "%.2f");
							ImGui::InputFloat3("Rotation", Math::ToPtr(transform.eulerAngles), "%.2f");
							ImGui::InputFloat3("Scale", Math::ToPtr(transform.scale), "%.2f");

							const ImGuiIO& io = ImGui::GetIO(); static_cast<void>(io);
							ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

							auto matrix = GetModelMatrix(transform);

							Manipulate(Math::ToArray(camera.view), Math::ToArray(camera.projection), currentGizmoOperation, currentGizmoMode, Math::ToPtr(matrix));

							if (ImGuizmo::IsUsing())
							{

							}
						}
				});

				ImGui::EndChild();
			}

			ImGui::End();
		}
	}

	void CreateGUIWindow(const char* name)
	{
		ImGui::Begin(name);
		ImGui::End();
	}

	void EndGUIFrame()
	{
		ImGui::Render();
		//ImGui::UpdatePlatformWindows();
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
