#include "stdafx.h"
#include "DeveloperScene.h"

#include "Framework.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "ObjectManager.h"

#include "GameObject.h"
#include "TextGo.h"

#include "GameBackground.h"
#include "Board.h"
#include "Cat.h"
#include "UIButton.h"

DeveloperScene::DeveloperScene() : Scene(SceneId::None)
{
}

void DeveloperScene::Init()
{
	Release();
	isDeveloperMode = true;

	auto size = FRAMEWORK.GetWindowSize();

	/*stageInfos.insert({ 1, "scene_datas/stage_1.txt" });
	stageInfos.insert({ 2, "scene_datas/stage_2.txt" });
	stageInfos.insert({ 3, "scene_datas/stage_3.txt" });*/


	for (int i = 1; i < 11; i++)
	{
		stageInfos.insert({ i, "scene_datas/stage_" + std::to_string(i) + ".txt" });
	}

	AddGo(new GameBackground("", "Background"));
	AddGo(new Board("", "Board"));

	//Save Button
	UIButton* button = (UIButton*)AddGo(new UIButton("sprites/language_speachbubble_1_0.png"));
	button->SetOrigin(Origins::TR);
	button->sortLayer = 100;
	button->SetPosition(size.x - 100, 100);
	sf::Vector2f textPos = button->GetPosition();
	textPos.x -= 100;
	button->SetText("fonts/ShadowsIntoLight-Regular.ttf", 100, sf::Color::Black, "SAVE", Origins::TR, textPos);

	button->OnEnter = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture("sprites/language_speachbubble_1_1.png");
		button->sprite.setTexture(*tex);
	};
	button->OnExit = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture(button->GetResourcePath());
		button->sprite.setTexture(*tex);
	};
	button->OnClick = [button, this]() {
		Board* board = dynamic_cast<Board*>(FindGo("Board"));
		OBJECT_MGR.SaveObjects(stageInfos.find(stageNum)->second, board->GetBoardInfo().type, gameObjects);
	};

	//Load Button
	button = (UIButton*)AddGo(new UIButton("sprites/language_speachbubble_1_0.png"));
	button->SetOrigin(Origins::TR);
	button->sortLayer = 100;
	button->SetPosition(size.x - 100, 300);
	textPos = button->GetPosition();
	textPos.x -= 100;
	button->SetText("fonts/ShadowsIntoLight-Regular.ttf", 100, sf::Color::Black, "LOAD", Origins::TR, textPos);

	button->OnEnter = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture("sprites/language_speachbubble_1_1.png");
		button->sprite.setTexture(*tex);
	};
	button->OnExit = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture(button->GetResourcePath());
		button->sprite.setTexture(*tex);
	};
	button->OnClick = [button, this]() {
		isLoad = true;
	};

	//Next Button
	button = (UIButton*)AddGo(new UIButton("sprites/button_next_0.png"));
	button->SetOrigin(Origins::BC);
	button->sortLayer = 100;
	button->SetPosition(size.x / 2 + 200, size.y - 50);

	button->OnEnter = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture("sprites/button_next_1.png");
		button->sprite.setTexture(*tex);
	};
	button->OnExit = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture(button->GetResourcePath());
		button->sprite.setTexture(*tex);
	};
	button->OnClick = [button, this]() {
		Board* board = dynamic_cast<Board*>(FindGo("Board"));
		board->Release();
		board->GetRooms()->clear();
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
	button->SetPosition(size.x / 2 - 200, size.y - 50);

	button->OnEnter = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture("sprites/button_back_1.png");
		button->sprite.setTexture(*tex);
	};
	button->OnExit = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture(button->GetResourcePath());
		button->sprite.setTexture(*tex);
	};
	button->OnClick = [button, this]() {
		Board* board = dynamic_cast<Board*>(FindGo("Board"));
		board->Release();
		board->GetRooms()->clear();
		Exit();
		auto bI = (int)board->GetBoardInfo().type;
		bI = bI == 3 ? 8 : bI - 1;
		board->SetBoard((BoardType)bI);
		board->Reset();
	};

	SpriteGo* sgo = (SpriteGo*)AddGo(new SpriteGo("sprites/soundleiste_0004_Abgerundetes_Rechteck_1_Kopie_4_0.png"));
	sgo->SetOrigin(Origins::TC);
	sgo->SetPosition(size.x / 2, 50);
	sgo->sortLayer = 100;

	TextGo* tgo = (TextGo*)AddGo(new TextGo("fonts/ShadowsIntoLight-Regular.ttf", "Stage Number"));
	tgo->SetText(std::to_string(stageNum), 50, sf::Color::Black, Origins::TC, 101, sgo->GetPosition().x - 10, sgo->GetPosition().y + 5);

	//Stage Up
	button = (UIButton*)AddGo(new UIButton("sprites/soundleiste_0001___0.png"));
	button->SetOrigin(Origins::TC);
	button->sortLayer = 100;
	button->SetPosition(size.x / 2 + 80, 50);

	button->OnEnter = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture("sprites/soundleiste_0001___1.png");
		button->sprite.setTexture(*tex);
	};
	button->OnExit = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture(button->GetResourcePath());
		button->sprite.setTexture(*tex);
	};
	button->OnClick = [this, tgo]() {
		stageNum = stageNum == 10 ? 1 : stageNum + 1;
		tgo->SetTextString(std::to_string(stageNum));
	};

	//Stage Down
	button = (UIButton*)AddGo(new UIButton("sprites/soundleiste_0000___0.png"));
	button->SetOrigin(Origins::TC);
	button->sortLayer = 100;
	button->SetPosition(size.x / 2 - 80, 50);

	button->OnEnter = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture("sprites/soundleiste_0000___1.png");
		button->sprite.setTexture(*tex);
	};
	button->OnExit = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture(button->GetResourcePath());
		button->sprite.setTexture(*tex);
	};
	button->OnClick = [this, tgo]() {
		stageNum = stageNum == 1 ? 10 : stageNum - 1;
		tgo->SetTextString(std::to_string(stageNum));
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
		cat1->SetBoard(dynamic_cast<Board*>(FindGo("Board")));
	}

	//Pot 생성 및 교체
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Space) && !isCatch)
	{
		pSpriteGo = (SpriteGo*)AddGo(new SpriteGo("sprites/b1_0.png", "Pot"));
		pSpriteGo->sprite.setTexture(*RESOURCE_MGR.GetTexture("sprites/b1_0.png"));
		pSpriteGo->SetOrigin(Origins::MC);
		isCatch = true;
	}
	else if (INPUT_MGR.GetKeyDown(sf::Keyboard::Space) && isCatch)
	{
		sf::Texture* t = RESOURCE_MGR.GetTexture("sprites/b" + std::to_string(num) + "_0.png");
		pSpriteGo->sprite.setTexture(*t);
		pSpriteGo->SetResourcePath("sprites/b" + std::to_string(num) + "_0.png");
		sf::Vector2i size = (sf::Vector2i)t->getSize();
		pSpriteGo->sprite.setTextureRect({ 0, 0, size.x, size.y });
		pSpriteGo->SetOrigin(Origins::MC);

		num = num == 20 ? 1 : num + 1;
	}

	//Pot 배치 or 해제
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
	//OBJECT_MGR.LoadObjects()에서 pool에 Get()을 한 뒤 저장한 값을 세팅해 주기 때문에 그 전에 한 번 tilePool을 비워줌
	board->ClearRooms();


	std::tuple<int, std::vector<std::tuple<std::string, std::string, float, float, float>>> sceneData =
		OBJECT_MGR.LoadObjects(stageInfos.find(stageNum)->second);
	int boardType = std::get<0>(sceneData);
	std::vector<std::tuple<std::string, std::string, float, float, float>> infos = std::get<1>(sceneData);

	this->stageNum = stageNum;
	std::string str = "Level " + std::to_string(stageNum);
	TextGo* textGo = (TextGo*)FindGo("Stage Number");
	textGo->SetTextString(str);

	for (auto go : gameObjects)
	{
		std::string name = go->GetName();
		if (name == "Cat" || name == "Pot" || name == "Tile")
		{
			RemoveGo(go);
			delete go;
		}
	}
	cats.clear();

	Exit();

	std::string strBoardType = std::to_string(boardType);
	std::string boardAniId = "board_" + strBoardType + "x" + strBoardType;
	board->SetBoardInfo((BoardType)boardType, boardAniId);
	board->SetBoard((BoardType)boardType);
	board->Reset();

	for (auto& info : infos)
	{
		if (std::get<0>(info) == "Tile")
		{
			ObjectPool<Tile>* tilePool = board->GetTilePool();
			Tile* tile = tilePool->Get();
			tile->Init();
			tile->Reset();
			tile->SetResourcePath(std::get<1>(info));
			tile->SetPosition(std::get<2>(info), std::get<3>(info));
			tile->SetRotation(0.f);
			std::vector<Room>* rooms = board->GetRooms();
			for (int i = 0; i < rooms->size(); i++)
			{
				if (tile->GetPosition() == (*rooms)[i].shape.getPosition())
				{
					(*rooms)[i].tile = tile;
				}
			}
			AddGo(tile);
		}
		else if (std::get<0>(info) == "Cat")
		{
			Cat* cat = new Cat((CatTypes)stoi(std::get<1>(info)));    //texId가 아니라 CatTypes가 저장되 있음
			cat->Init();
			cat->Reset();
			cat->SetOrigin(Origins::MC);
			cat->SetPosition(std::get<2>(info), std::get<3>(info));
			cat->SetStartPos({ std::get<2>(info), std::get<3>(info) });
			//cat->SetRotation(rotation);   //box를 만든 뒤에 돌리거나 돌린 다음에 박스 정보도 배열로 만들어서 회전 시킨 값을 적용시킬 필요 있음
			cat->SetBoard(board);
			cats.push_back(cat);
			AddGo(cat);
		}
		else if (std::get<0>(info) == "Pot")
		{
			SpriteGo* pot = new SpriteGo("", "Pot");
			pot->Init();
			pot->Reset();
			pot->SetResourcePath(std::get<1>(info));
			pot->sprite.setTexture(*RESOURCE_MGR.GetTexture(std::get<1>(info)));
			pot->SetOrigin(Origins::MC);
			pot->SetPosition(std::get<2>(info), std::get<3>(info));
			pot->SetRotation(0.f);
			AddGo(pot);
		}
	}
}
