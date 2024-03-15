#pragma once

namespace CGEngine
{
	class Scene;
	struct Camera;
	struct Window;

	void CreateGUIContext(const Window& window);
	void DestroyGUIContext();

	void BeginGUIFrame();
	void CreateViewport(const Camera& camera);
	void CreateEditorWindow(Scene& scene);
	void EndGUIFrame();

	void DrawGUI();
}
