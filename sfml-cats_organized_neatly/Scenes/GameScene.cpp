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
	resourceListPath = "scripts/stage_resource_list.csv";
}

void GameScene::Init()	//한 번만 해주면 되는 것들 위주로
{	
	Release();

	auto size = FRAMEWORK.GetWindowSize();

	stageInfos.insert({ 1, "scene_datas/stage_1.txt" });
	stageInfos.insert({ 2, "scene_datas/stage_2.txt" });
	stageInfos.insert({ 3, "scene_datas/stage_3.txt" });
	stageInfos.insert({ 4, "scene_datas/stage_4.txt" });
	stageInfos.insert({ 5, "scene_datas/stage_5.txt" });
	stageInfos.insert({ 6, "scene_datas/stage_6.txt" });
	stageInfos.insert({ 7, "scene_datas/stage_7.txt" });
	stageInfos.insert({ 8, "scene_datas/stage_8.txt" });
	stageInfos.insert({ 9, "scene_datas/stage_9.txt" });
	stageInfos.insert({ 10, "scene_datas/stage_10.txt" });

	AddGo(new GameBackground());
	board = dynamic_cast<Board*>(AddGo(new Board()));

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
		LoadScene(stageNum);
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
		SCENE_MGR.ChangeScene(SceneId::Title);
	};
	//다음 버튼
	button = (UIButton*)AddGo(new UIButton("sprites/button_next_0.png", "Next Button"));
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
		stageNum = stageNum == 10 ? 1 : stageNum + 1;
		LoadScene(stageNum);
		button->SetActive(false);
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

	RESOURCE_MGR.UnLoadAll();
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

	if (INPUT_MGR.GetKey(sf::Keyboard::LShift) && INPUT_MGR.GetKeyDown(sf::Keyboard::D))
	{
		ToggleIsDeveloperMode();
	}

	bool prevBoard = isComplete;
	isComplete = board->CheckBoard();
	if (isComplete)
	{
		//클리어 애니메이션 재생
		if (!prevBoard)
		{
		}
		
		//Next 버튼 활성화
		FindGo("Next Button")->SetActive(true);
	}
}

void GameScene::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);

}

void GameScene::LoadScene(int stageNum)
{
	this->stageNum = stageNum;

	//OBJECT_MGR.LoadObjects()에서 pool에 Get()을 한 뒤 저장한 값을 세팅해 주기 때문에 그 전에 한 번 tilePool을 비워줌
	if (board == nullptr)
		board = dynamic_cast<Board*>(FindGo("Board"));
	board->ClearRooms();

	std::tuple<int, std::vector<std::tuple<std::string, std::string, float, float, float>>> sceneData = 
		OBJECT_MGR.LoadObjects(stageInfos.find(stageNum)->second);
	int boardType = std::get<0>(sceneData);
	std::vector<std::tuple<std::string, std::string, float, float, float>> infos = std::get<1>(sceneData);

	std::string str = "Level " + std::to_string(stageNum);
	TextGo* textGo = (TextGo*)FindGo("Stage Number");
	textGo->SetTextString(str);

	for (auto go : gameObjects)
	{
		std::string name = go->GetName();
		if (name == "Cat" || name == "Pot")
		{
			RemoveGo(go);
			delete go;
		}
	}
	cats.clear();

	for (auto go : removeGameObjects)
	{
		gameObjects.remove(go);
	}
	removeGameObjects.clear();

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
			std::cout << "new Cat(), Position: " <<cat->GetPosition().x<<","<<cat->GetPosition().y<< " "<<cat->GetActive() << std::endl;
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
