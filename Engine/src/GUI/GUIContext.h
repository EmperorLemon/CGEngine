#pragma once

#include <cstdint>

namespace CGEngine
{
	class Scene;
	struct Camera;
	struct Window;

	void CreateGUIContext(const Window& window);
	void DestroyGUIContext();

	void BeginGUIFrame();
	void CreateViewport(uint32_t viewportID);
	void CreateEditorWindow(Scene& scene);
	void EndGUIFrame();

	void DrawGUI();
}
