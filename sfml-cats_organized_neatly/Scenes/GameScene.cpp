#include "stdafx.h"
#include "GameScene.h"

#include "Framework.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "ObjectManager.h"

#include "TextGo.h"
#include "GameBackground.h"
#include "Board.h"
#include "UIButton.h"


GameScene::GameScene() : Scene(SceneId::Game)
{
}

void GameScene::Init()	//한 번만 해주면 되는 것들 위주로
{
	Release();

	auto size = FRAMEWORK.GetWindowSize();

	stageInfos.insert({ 1, "scene_datas/stage_1.txt" });
	stageInfos.insert({ 2, "scene_datas/stage_2.txt" });
	stageInfos.insert({ 3, "scene_datas/stage_3.txt" });

	AddGo(new GameBackground());
	AddGo(new Board());

	//리셋 버튼
	UIButton* button = (UIButton*)AddGo(new UIButton("sprites/interface_replay1_0.png"));
	button->SetOrigin(Origins::BC);
	button->SetPosition(size.x * 0.5f, size.y - 50);
	button->sortLayer = 100;
	button->OnEnter = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture("sprites/interface_replay1_1.png");
		button->sprite.setTexture(*tex);
	};
	button->OnExit = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture(button->GetResourcePath());
		button->sprite.setTexture(*tex);
	};
	button->OnClick = [button, this]() {
		LoadScene(1);
	};
	//뒤로 버튼
	button = (UIButton*)AddGo(new UIButton("sprites/button_back_0.png"));
	button->SetOrigin(Origins::BC);
	button->SetPosition(size.x * 0.5f - 300, size.y - 50);
	button->sortLayer = 100;
	button->OnEnter = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture("sprites/button_back_1.png");
		button->sprite.setTexture(*tex);
	};
	button->OnExit = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture(button->GetResourcePath());
		button->sprite.setTexture(*tex);
	};
	button->OnClick = [button, this]() {

	};
	//다음 버튼
	button = (UIButton*)AddGo(new UIButton("sprites/button_next_0.png"));
	button->SetOrigin(Origins::BC);
	button->SetPosition(size.x * 0.5f + 300, size.y - 50);
	button->sortLayer = 100;
	button->OnEnter = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture("sprites/button_next_1.png");
		button->sprite.setTexture(*tex);
	};
	button->OnExit = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture(button->GetResourcePath());
		button->sprite.setTexture(*tex);
	};
	button->OnClick = [button, this]() {

	};
	button->SetActive(false);

	//스테이지 번호
	TextGo* stageNumber = (TextGo*)AddGo(new TextGo("fonts/ShadowsIntoLight-Regular.ttf", "Stage Number"));
	std::string str = "Level " + std::to_string(stageNum);
	stageNumber->SetText(str, 100, sf::Color::Black, Origins::MC, 101, size.x * 0.5f - 110, 50.f);

	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void GameScene::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
	gameObjects.clear();
}

void GameScene::Enter()
{
	auto size = FRAMEWORK.GetWindowSize();
	worldView.setSize(size);
	worldView.setCenter({ 0, 0 });

	uiView.setSize(size);
	uiView.setCenter(size * 0.5f);

	Scene::Enter();
}

void GameScene::Exit()
{
	Scene::Exit();

}

void GameScene::Update(float dt)
{
	Scene::Update(dt);

	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
	sf::Vector2f worldMousePos = SCENE_MGR.GetCurrScene()->ScreenToWorldPos(mousePos);
}

void GameScene::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);

}

void GameScene::LoadScene(int stageNum)
{
	//OBJECT_MGR.LoadObjects()에서 pool에 Get()을 한 뒤 저장한 값을 세팅해 주기 때문에 그 전에 한 번 tilePool을 비워줌
	Board* board = dynamic_cast<Board*>(FindGo("Board"));
	board->GetTilePool()->Clear();


	std::tuple<int, std::vector<GameObject*>> sceneData = OBJECT_MGR.LoadObjects(stageInfos.find(stageNum)->second);
	int boardType = std::get<0>(sceneData);
	std::vector<GameObject*> vGameObjects = std::get<1>(sceneData);

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

	Exit();

	std::string strBoardType = std::to_string(boardType);
	std::string boardAniId = "board_" + strBoardType + "x" + strBoardType;
	board->SetBoardInfo((BoardType)boardType, boardAniId);
	board->Reset();
	board->SetBoard((BoardType)boardType);

	for (auto go : vGameObjects)
	{
		if (go->GetName() == "Tile")
		{
			std::vector<Room>* rooms = board->GetRooms();
			for (int i = 0; i < rooms->size(); i++)
			{
				if (go->GetPosition() == (*rooms)[i].shape.getPosition())
				{
					(*rooms)[i].tile = dynamic_cast<Tile*>(go);
					(*rooms)[i].isFull = true;
					std::cout << "room["<< i<<"]: "<< (*rooms)[i].isFull << std::endl;
				}	
			}
		}
		else if (go->GetName() == "Cat")
		{
			Cat* cat = dynamic_cast<Cat*>(go);
			cat->SetBoard(board);
		}
		AddGo(go);
	}
}
