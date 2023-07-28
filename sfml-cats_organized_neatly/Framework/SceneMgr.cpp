#include "stdafx.h"
#include "SceneMgr.h"
#include "ResourceMgr.h"
#include "DeveloperScene.h"
#include "TitleScene.h"
#include "GameScene.h"

void SceneMgr::Init()
{
	if (!scenes.empty())
	{
		Release();
	}

	if (!titleBgm.openFromFile("Exported_Sounds/music/Menu_loop.wav"))
	{
		std::cout << "Open sound error" << std::endl;
	}
	if (!gameBgm.openFromFile("Exported_Sounds/music/Gameplay_loop.wav"))
	{
		std::cout << "Open sound error" << std::endl;
	}

	titleBgm.setLoop(true);
	gameBgm.setLoop(true);

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

	if (titleBgm.getStatus() != sf::SoundSource::Status::Playing && currentSceneId == SceneId::Title)
	{
		gameBgm.stop();
		titleBgm.play();
	}
	else if (gameBgm.getStatus() != sf::SoundSource::Status::Playing && currentSceneId == SceneId::Game)
	{
		titleBgm.stop();
		gameBgm.play();
	}
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
		return;
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
