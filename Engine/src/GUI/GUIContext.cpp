#include "GUIContext.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Core/Window.h"

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