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
#include "ObjectManager.h"

DeveloperScene::DeveloperScene() : Scene(SceneId::Game)
{
}

void DeveloperScene::Init()
{
	Release();

	auto size = FRAMEWORK.GetWindowSize();

	AddGo(new GameBackground("", "Background"));
	AddGo(new Board("", "Board"));

	//Save Button
	UIButton* button = (UIButton*)AddGo(new UIButton("sprites/language_speachbubble_1_0.png"));
	button->SetOrigin(Origins::MC);
	button->sprite.setScale({ 0.5f, 0.5f });
	button->sortLayer = 100;
	button->SetPosition(size.x - 100, 50);
	button->SetText("fonts/Roboto-Black.ttf", 35, sf::Color::Black, "SAVE", Origins::BC, button->GetPosition());

	button->OnEnter = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture("sprites/language_speachbubble_1_1.png");
		button->sprite.setTexture(*tex);
	};
	button->OnExit = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture(button->GetTexId());
		button->sprite.setTexture(*tex);
	};
	button->OnClick = [button, this]() {
		OBJECT_MGR.SaveObjects("scene_datas/stage_1.txt", BoardType::_3X3, gameObjects);
	};

	//Load Button
	button = (UIButton*)AddGo(new UIButton("sprites/language_speachbubble_1_0.png"));
	button->SetOrigin(Origins::MC);
	button->sprite.setScale({ 0.5f, 0.5f });
	button->sortLayer = 100;
	button->SetPosition(size.x - 100, 150);
	button->SetText("fonts/Roboto-Black.ttf", 35, sf::Color::Black, "LOAD", Origins::BC, button->GetPosition());

	button->OnEnter = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture("sprites/language_speachbubble_1_1.png");
		button->sprite.setTexture(*tex);
	};
	button->OnExit = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture(button->GetTexId());
		button->sprite.setTexture(*tex);
	};
	button->OnClick = [button, this]() {
		isLoad = true;
	};

	//Next Button
	button = (UIButton*)AddGo(new UIButton("sprites/button_next_0.png"));
	button->SetOrigin(Origins::BC);
	button->sortLayer = 100;
	button->SetPosition(size.x / 2 + 100, size.y);

	button->OnEnter = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture("sprites/button_next_1.png");
		button->sprite.setTexture(*tex);
	};
	button->OnExit = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture(button->GetTexId());
		button->sprite.setTexture(*tex);
	};
	button->OnClick = [button, this]() {
		Board* board = dynamic_cast<Board*>(FindGo("Board"));
		board->Release();
		board->GetRooms().clear();
		Exit();
		auto bI = (int)board->GetBoardInfo().type;
		bI = bI == 8 ? 3 : bI + 1;
		board->SetBoard((BoardType)bI);
		board->Reset();
	};

	//Previous Button
	button = (UIButton*)AddGo(new UIButton("sprites/button_back_0.png"));
	button->SetOrigin(Origins::BC);
	button->sortLayer = 100;
	button->SetPosition(size.x / 2 - 100, size.y);

	button->OnEnter = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture("sprites/button_back_1.png");
		button->sprite.setTexture(*tex);
	};
	button->OnExit = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture(button->GetTexId());
		button->sprite.setTexture(*tex);
	};
	button->OnClick = [button, this]() {
		Board* board = dynamic_cast<Board*>(FindGo("Board"));
		board->Release();
		board->GetRooms().clear();
		Exit();
		auto bI = (int)board->GetBoardInfo().type;
		bI = bI == 3 ? 8 : bI - 1;
		board->SetBoard((BoardType)bI);
		board->Reset();

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
	gameObjects.clear();
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
				std::cout << "��ü" << std::endl;
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
		std::cout << "����" << std::endl;
		Cat* cat1 = (Cat*)AddGo(new Cat(CatTypes::c1));
		cat1->Init();
		cat1->Reset();
	}

	//�ٹ̱�
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Space) && !isCatch)
	{
		pSpriteGo = (SpriteGo*)AddGo(new SpriteGo("sprites/b1_0.png", "Pot"));
		pSpriteGo->sprite.setTexture(*RESOURCE_MGR.GetTexture("sprites/b1_0.png"));
		pSpriteGo->SetOrigin(Origins::MC);
		isCatch = true;
	}
	else if (INPUT_MGR.GetKeyDown(sf::Keyboard::Space) && isCatch)
	{
		//pSpriteGo->sprite.setTexture(*RESOURCE_MGR.GetTexture("sprites/b2_0.png"));
		sf::Texture* t = RESOURCE_MGR.GetTexture("sprites/b" + std::to_string(num) + "_0.png");
		pSpriteGo->sprite.setTexture(*t);
		pSpriteGo->SetTexId("sprites/b" + std::to_string(num) + "_0.png");
		sf::Vector2i size = (sf::Vector2i)t->getSize();
		pSpriteGo->sprite.setTextureRect({ 0, 0, size.x, size.y });
		pSpriteGo->SetOrigin(Origins::MC);

		num = num == 20 ? 1 : num + 1;
	}

	//��ġ or ����
	if (isCatch)
	{
		pSpriteGo->SetPosition(worldMousePos);

		if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left))
		{
			pSpriteGo->SetPosition(worldMousePos);
			pot.push_back(*pSpriteGo);
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

	if (isLoad)
	{
		isLoad = false;
		LoadScene();
	}
}

void DeveloperScene::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

void DeveloperScene::LoadScene()
{
	//OBJECT_MGR.LoadObjects()���� pool�� Get()�� �� �� ������ ���� ������ �ֱ� ������ �� ���� �� �� tilePool�� �����
	Board* board = dynamic_cast<Board*>(FindGo("Board"));
	board->GetTilePool()->Clear();

	std::tuple<int, std::vector<GameObject*>> sceneData = OBJECT_MGR.LoadObjects("scene_datas/stage_1.txt");
	int boardType = std::get<0>(sceneData);
	std::vector<GameObject*> vGameObjects = std::get<1>(sceneData);

	//������ �ִ� Cat�� Pot�� ������ �� ���� ����
	for (auto go : gameObjects)
	{
		std::string name = go->GetName();
		if (name == "Cat" || name == "Pot" || name == "Tile")
		{
			RemoveGo(go);
			delete go;
		}	
	}

	Exit();

	for (auto go : vGameObjects)
	{
		if (go->GetName() == "Tile")
		{
			std::vector<Room>& rooms = board->GetRooms();
			for (int i = 0; i < rooms.size(); i++)
			{
				if (go->GetPosition() == rooms[i].room.getPosition())
					rooms[i].tile = dynamic_cast<Tile*>(go);
			}
		}
		AddGo(go);
	}
}
