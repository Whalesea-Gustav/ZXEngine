#include "Game.h"
#include "EventManager.h"
#include "RenderEngine.h"
#include "Input/InputManager.h"
#include "Time.h"
#include "SceneManager.h"
#include "LuaManager.h"
#include "GameLogicManager.h"
#include "ProjectSetting.h"
#include "Component/Animator.h"
#include "Audio/AudioEngine.h"
#include "Resources.h"

#ifdef ZX_EDITOR
#include "Editor/EditorGUIManager.h"
#include "Editor/EditorDataManager.h"
#include "Editor/EditorInputManager.h"
#endif

namespace ZXEngine
{
	void Game::Launch(const string& path)
	{
#ifdef ZX_EDITOR
		EditorDataManager::Create();
#endif

		if (!ProjectSetting::InitSetting(path))
		{
			std::cerr << "Invalid project path: " << path << std::endl;
			return;
		}
		else
		{
			std::cout << "ZXEngine launch project: " << path << std::endl;
		}

		EventManager::Create();
		AudioEngine::Create();
		RenderEngine::Create();
		InputManager::Create();
		LuaManager::Create();
		GameLogicManager::Create();
		SceneManager::Create();
#ifdef ZX_EDITOR
		EditorGUIManager::Create();
		EditorInputManager::Create();
#endif

		while (!RenderEngine::GetInstance()->WindowShouldClose())
		{
			// �߼�
			Update();

			// ��Ⱦ
			Render();

#ifdef ZX_DEBUG
			// ����
			Debug::Update();
#endif
		}
	}

	void Game::Update()
	{
		Time::Update();

		Resources::CheckAsyncLoad();

		InputManager::GetInstance()->Update();

#ifdef ZX_EDITOR
		EditorInputManager::GetInstance()->Update();
#endif

#ifdef ZX_EDITOR
		if (EditorDataManager::isGameStart && !EditorDataManager::isGamePause)
		{
			SceneManager::GetInstance()->GetCurScene()->Update();
		}
#else
		SceneManager::GetInstance()->GetCurScene()->Update();
#endif
	}

	void Game::Render()
	{
		RenderEngine::GetInstance()->BeginRender();

		SceneManager::GetInstance()->GetCurScene()->Render();

#ifdef ZX_EDITOR
		EditorGUIManager::GetInstance()->Render();
#endif

		RenderEngine::GetInstance()->EndRender();
	}
}