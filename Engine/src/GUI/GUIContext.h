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
	void CreateViewport(uint32_t colorID, uint32_t glowID, uint32_t depthID);
	void CreateEditorWindow(Scene& scene);
	void EndGUIFrame();

	void DrawGUI();
}
