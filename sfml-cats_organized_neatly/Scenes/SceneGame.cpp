#include "stdafx.h"
#include "SceneGame.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "GameObject.h"
#include "Framework.h"
#include "GameBackground.h"
#include "Board.h"

SceneGame::SceneGame() : Scene(SceneId::Game)
{

	//resourceListPath = "scripts/SceneGameResourceList.csv";
	/*resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/RubySheet.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/sprite_sheet.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/button.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/button2.png"));*/

}

void SceneGame::Init()
{
	Release();

	auto size = FRAMEWORK.GetWindowSize();

	AddGo(new GameBackground());
	AddGo(new Board());

	/*Player1* player = (Player1*)AddGo(new Player1());
	UIButton * testButton = (UIButton*)AddGo(new UIButton("graphics/button.png"));
	testButton->SetOrigin(Origins::TR);
	testButton->sortLayer = 100;
	testButton->SetPosition(size.x, 0.f);
	testButton->OnEnter = [testButton]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture("graphics/button2.png");
		testButton->sprite.setTexture(*tex);
	};
	testButton->OnExit = [testButton]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture(testButton->textureId);
		testButton->sprite.setTexture(*tex);
	};
	testButton->OnClick = [testButton]() {
		
	};*/
	
	//Ground* ground = (Ground*)AddGo(new Ground());
	//ground->SetPlayer(player);

	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void SceneGame::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneGame::Enter()
{
	auto size = FRAMEWORK.GetWindowSize();
	worldView.setSize(size);
	worldView.setCenter({0, 0});

	uiView.setSize(size);
	uiView.setCenter(size * 0.5f);

	Scene::Enter();
}

void SceneGame::Exit()
{
	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
