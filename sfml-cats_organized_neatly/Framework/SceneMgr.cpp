#include "stdafx.h"
#include "SceneMgr.h"
#include "DeveloperScene.h"
#include "TitleScene.h"
#include "GameScene.h"

void SceneMgr::Init()
{
	if (!scenes.empty())
	{
		Release();
	}

	scenes.push_back(new TitleScene());
	scenes.push_back(new GameScene());
	scenes.push_back(new DeveloperScene());

	for (auto scene : scenes)
	{
		scene->Init();
	}

	currentSceneId = startSceneId;
	currentScene = scenes[(int)currentSceneId];
	currentScene->Enter();
}

void SceneMgr::Release()
{
	if (scenes.empty())
	{
		return;
	}

	for (auto scene : scenes)
	{
		//scene->Release();
		delete scene;
	}
	scenes.clear();

	currentSceneId = SceneId::Title;
	currentScene = nullptr;
}

void SceneMgr::UpdateEvent(float dt)
{
	currentScene->Update(dt);
}

void SceneMgr::Draw(sf::RenderWindow& window)
{
	currentScene->Draw(window);
}

void SceneMgr::ChangeScene(SceneId id, int stageNum)
{
	currentScene->Exit();
	currentSceneId = id;
	currentScene = scenes[(int)currentSceneId];
	currentScene->Enter();
	
	GameScene* scene = dynamic_cast<GameScene*>(currentScene);
	if (scene != nullptr)
	{
		scene->LoadScene(stageNum);
	}
}

Scene* SceneMgr::GetCurrScene() const
{
	return currentScene;
}

const std::vector<Scene*>* SceneMgr::GetScenes() const
{
	return &scenes;
}
