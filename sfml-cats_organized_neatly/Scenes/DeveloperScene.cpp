#include "stdafx.h"
#include "DeveloperScene.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "GameObject.h"
#include "Framework.h"
#include "GameBackground.h"
#include "Board.h"
#include "Cat.h"
#include "UIButton.h"

DeveloperScene::DeveloperScene() : Scene(SceneId::Game)
{
}

void DeveloperScene::Init()
{
	Release();

	auto size = FRAMEWORK.GetWindowSize();

	AddGo(new GameBackground("", "Background"));
	AddGo(new Board("", "Board"));

	UIButton* saveButton = (UIButton*)AddGo(new UIButton("sprites/language_speachbubble_1_0.png"));
	saveButton->SetOrigin(Origins::MC);
	saveButton->sprite.setScale({ 0.5f, 0.5f });
	saveButton->sortLayer = 100;
	saveButton->SetPosition(size.x - 100, 50);
	saveButton->SetText("fonts/Roboto-Black.ttf", 35, sf::Color::Black, "SAVE", Origins::BC, saveButton->GetPosition());

	saveButton->OnEnter = [saveButton]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture("sprites/language_speachbubble_1_1.png");
		saveButton->sprite.setTexture(*tex);
	};
	saveButton->OnExit = [saveButton]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture(saveButton->textureId);
		saveButton->sprite.setTexture(*tex);
	};
	saveButton->OnClick = [saveButton, this]() {
		for (auto go : gameObjects)
		{
			auto tile = dynamic_cast<Tile*>(go);
			if (tile != nullptr)
			{
				//타일 정보 저장

				continue;
			}
			auto cat = dynamic_cast<Cat*>(go);
			if (cat != nullptr)
			{
				//퍼즐 정보 저장

				continue;
			}
			auto sGo = dynamic_cast<SpriteGo*>(go);
			if(sGo != nullptr)
			{
				//장식 객체 정보 저장
				std::cout << "장식 저장" << std::endl;
				continue;
			}
			
		}
	};

	saveButton = (UIButton*)AddGo(new UIButton("sprites/language_speachbubble_1_0.png"));
	saveButton->SetOrigin(Origins::MC);
	saveButton->sprite.setScale({ 0.5f, 0.5f });
	saveButton->sortLayer = 100;
	saveButton->SetPosition(size.x - 100, 150);
	saveButton->SetText("fonts/Roboto-Black.ttf", 35, sf::Color::Black, "LOAD", Origins::BC, saveButton->GetPosition());

	saveButton->OnEnter = [saveButton]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture("sprites/language_speachbubble_1_1.png");
		saveButton->sprite.setTexture(*tex);
	};
	saveButton->OnExit = [saveButton]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture(saveButton->textureId);
		saveButton->sprite.setTexture(*tex);
	};
	saveButton->OnClick = [saveButton]() {

	};


	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void DeveloperScene::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void DeveloperScene::Enter()
{
	auto size = FRAMEWORK.GetWindowSize();
	worldView.setSize(size);
	worldView.setCenter({0, 0});

	uiView.setSize(size);
	uiView.setCenter(size * 0.5f);

	Scene::Enter();
}

void DeveloperScene::Exit()
{
	Scene::Exit();
}

void DeveloperScene::Update(float dt)
{
	Scene::Update(dt);

	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
	sf::Vector2f worldMousePos = SCENE_MGR.GetCurrScene()->ScreenToWorldPos(mousePos);

	for (auto& object : gameObjects)
	{
		auto cat = dynamic_cast<Cat*>(object);
		if (cat != nullptr)
		{
			bool prevHover = isCatHover;
			isCatHover = cat->sprite.getGlobalBounds().contains(worldMousePos);

			if (isCatHover && INPUT_MGR.GetKeyDown(sf::Keyboard::C))
			{
				std::cout << "교체" << std::endl;
				catType = cat->GetType();
				catType = catType == 12 ? 1 : catType + 1;
				cat->SetType((CatTypes)catType);
				cat->Init();
				cat->Reset();
			}
			if (isCatHover && INPUT_MGR.GetKeyDown(sf::Keyboard::Escape))
			{
				cat->SetActive(false);
				RemoveGo(cat);
				isCatHover = false;
			}
		}		
	}
	if (!isCatHover && INPUT_MGR.GetKeyDown(sf::Keyboard::C))
	{
		std::cout << "생성" << std::endl;
		Cat* cat1 = (Cat*)AddGo(new Cat(CatTypes::c1));
		cat1->Init();
		cat1->Reset();
	}

	//꾸미기
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Space) && !isCatch)
	{
		pSpriteGo = (SpriteGo*)AddGo(new SpriteGo());
		pSpriteGo->sprite.setTexture(*RESOURCE_MGR.GetTexture("sprites/b1_0.png"));
		pSpriteGo->SetOrigin(Origins::MC);
		isCatch = true;
	}
	else if (INPUT_MGR.GetKeyDown(sf::Keyboard::Space) && isCatch)
	{
		//pSpriteGo->sprite.setTexture(*RESOURCE_MGR.GetTexture("sprites/b2_0.png"));
		sf::Texture* t = RESOURCE_MGR.GetTexture("sprites/b" + std::to_string(num) + "_0.png");
		pSpriteGo->sprite.setTexture(*t);
		sf::Vector2i size = (sf::Vector2i)t->getSize();
		pSpriteGo->sprite.setTextureRect({ 0, 0, size.x, size.y });
		pSpriteGo->SetOrigin(Origins::MC);

		num = num == 20 ? 1 : num + 1;
	}

	//배치 or 해제
	if (isCatch)
	{
		pSpriteGo->SetPosition(worldMousePos);

		if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left))
		{
			pSpriteGo->SetPosition(worldMousePos);
			ornaments.push_back(*pSpriteGo);
			pSpriteGo = nullptr;
			isCatch = false;
			num = 2;
		}
		else if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Right))
		{
			pSpriteGo->sprite.setTexture(sf::Texture());
			RemoveGo(pSpriteGo);
			isCatch = false;
			num = 2;
		}
	}
}

void DeveloperScene::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
