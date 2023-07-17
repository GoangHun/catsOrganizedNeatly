#include "stdafx.h"
#include "SceneGame.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "GameObject.h"
#include "Framework.h"
#include "GameBackground.h"
#include "Board.h"
#include "Cat.h"

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
	AddGo(new Board("", "Board"));

	Cat* cat = (Cat*)AddGo(new Cat(CatTypes::c1));
	cat->SetPosition(size.x, 0.f);
	
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

	if (INPUT_MGR.GetKey(sf::Keyboard::LShift) && INPUT_MGR.GetKeyDown(sf::Keyboard::D))
	{
		ToggleIsDeveloperMode();
	}

	if (isDeveloperMode)
	{
		sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
		sf::Vector2f worldMousePos = SCENE_MGR.GetCurrScene()->ScreenToWorldPos(mousePos);
		int index = 2;
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Space) && !isCatch)
		{
			sf::Texture* tex = RESOURCE_MGR.GetTexture("sprites/b1_0.png");
			sprite.setTexture(*tex);
			Utils::SetOrigin(sprite, Origins::MC);
			isCatch = true;
		}
		else if (INPUT_MGR.GetKeyDown(sf::Keyboard::Space) && isCatch)
		{
			sf::Texture* tex = RESOURCE_MGR.GetTexture("sprites/b" + std::to_string(index) + "_0.png");
			sprite.setTexture(*tex);
			Utils::SetOrigin(sprite, Origins::MC);
			index = index == 20 ? 1 : ++index;
		}

		if (isCatch)
		{
			sprite.setPosition(worldMousePos);
			std::cout << worldMousePos.x << " / " << worldMousePos.y << std::endl;

			if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left))
			{
				SpriteGo* go = new SpriteGo();
				go->sprite.setTexture(*sprite.getTexture());
				go->SetOrigin(Origins::MC);
				go->SetPosition(worldMousePos);
				ornaments.push_back(*go);

				AddGo(go);
			}
		}
		
	}
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
	
	if (isCatch)
	{
		window.setView(worldView);
		window.draw(sprite);
	}
}
