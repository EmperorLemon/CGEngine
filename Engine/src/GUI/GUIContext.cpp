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
				}

				ImGui::Spacing();

				entities.Iterate<Utils::GUID, Component::Transform, Component::DrawObject>([&](const Utils::GUID& GUID, Component::Transform& transform, const Component::DrawObject& object)
					{
						if (ImGui::CollapsingHeader(std::string("Object##" + GUID.str()).c_str()))
						{
							static float model_matrix[16];

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

							ImGui::InputFloat3("Translation", &transform.position.x, "%.2f");
							ImGui::InputFloat3("Rotation", &transform.eulerAngles.x, "%.2f");
							ImGui::InputFloat3("Scale", &transform.scale.x, "%.2f");

							const ImGuiIO& io = ImGui::GetIO(); static_cast<void>(io);
							ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

							auto model = GetModelMatrix(transform);
							const auto matrix = Math::ToPtr(model);

							Manipulate(Math::ToArray(camera.view), Math::ToArray(camera.projection), currentGizmoOperation, currentGizmoMode, matrix);

							if (ImGuizmo::IsUsing())
							{
								const auto& tf_matrix = Math::ToMat4(matrix);
								Math::Decompose(tf_matrix, transform.position, transform.rotation, transform.scale);
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
