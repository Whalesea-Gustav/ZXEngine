#pragma once
#include "../pubh.h"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw.h"
#include "../ImGui/imgui_impl_opengl3.h"

namespace ZXEngine
{
	class EditorPanel;
	class EditorAssetPreviewer;
	class EditorGUIManager
	{
		friend class EditorInputManager;
	public:
		static void Create();
		static EditorGUIManager* GetInstance();

	private:
		static EditorGUIManager* mInstance;

	public:
		EditorGUIManager();
		~EditorGUIManager();

		void BeginEditorRender();
		void EditorRender();
		void EndEditorRender();

	private:
		vector<EditorPanel*> allPanels;
		EditorAssetPreviewer* assetPreviewer;
	};
}