#pragma once

namespace CGEngine
{
	struct Window;

	void CreateGUIContext(const Window& window);
	void DestroyGUIContext();

	void BeginGUIFrame();
	void CreateGUIWindow(const char* name = "Default GUI Window");
	void EndGUIFrame();

	void DrawGUI();
}