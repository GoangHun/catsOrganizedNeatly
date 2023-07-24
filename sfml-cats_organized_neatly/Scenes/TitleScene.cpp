#include "stdafx.h"
#include "TitleScene.h"

#include "Framework.h"
#include "ResourceMgr.h"
#include "InputMgr.h"
#include "SceneMgr.h"

#include "SpriteGo.h"
#include "AniSpriteGo.h"

TitleScene::TitleScene()
{
	resourceListPath = "title_resource_list.csv";
}

void TitleScene::Init()
{
	Release();

	auto size = FRAMEWORK.GetWindowSize();

	AddGo(new SpriteGo("sprites/titel_0.png", "Title Logo"));
	AddGo(new SpriteGo("sprites/Background1a_0.png", "Main Title Background"));
	AddGo(new AniSpriteGo());
}

void TitleScene::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
	gameObjects.clear();

	RESOURCE_MGR.UnLoadAll();
}

void TitleScene::Enter()
{
	auto size = FRAMEWORK.GetWindowSize();
	worldView.setSize(size);
	worldView.setCenter({ 0, 0 });

	uiView.setSize(size);
	uiView.setCenter(size * 0.5f);

	Scene::Enter();
}

void TitleScene::Exit()
{
	Scene::Exit();
}

void TitleScene::Update(float dt)
{
	Scene::Update(dt);

	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
	sf::Vector2f worldMousePos = SCENE_MGR.GetCurrScene()->ScreenToWorldPos(mousePos);
}

void TitleScene::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
