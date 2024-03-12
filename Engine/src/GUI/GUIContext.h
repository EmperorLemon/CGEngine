#pragma once

namespace CGEngine
{
	class Scene;
	struct Window;

	void CreateGUIContext(const Window& window);
	void DestroyGUIContext();

	void BeginGUIFrame();
	void CreateEditorWindow(Scene& scene);
	void CreateGUIWindow(const char* name = "Default GUI Window");
	void EndGUIFrame();

	void DrawGUI();
}