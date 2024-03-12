#include "GUIContext.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

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
						ImGui::Text("Transform");
						ImGui::DragFloat3("Position", &transform.position.x, 0.1f, 0, 0, "%.2f");
						ImGui::DragFloat3("Rotation", &transform.rotation.x, 0.1f, -360.0f, 360.0f, "%.2f");
						ImGui::DragFloat3("Scale", &transform.scale.x, 0.1f, 0, 0, "%.2f");
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
