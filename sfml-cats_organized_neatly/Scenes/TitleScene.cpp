#include "stdafx.h"
#include "TitleScene.h"

#include "Framework.h"
#include "ResourceMgr.h"
#include "InputMgr.h"
#include "SceneMgr.h"

#include "SpriteGo.h"
#include "AniSpriteGo.h"
#include "UIButton.h"
#include "ChapterBoard.h"

TitleScene::TitleScene()
{
	resourceListPath = "scripts/title_resource_list.csv";
}

void TitleScene::Init()
{
	Release();

	auto size = FRAMEWORK.GetWindowSize();

	SpriteGo* sgo = dynamic_cast<SpriteGo*>(AddGo(new SpriteGo("sprites/titel_0.png", "Title Logo")));
	SetInitValue(sgo, Origins::MC, { size.x * 0.5f, size.y * 0.2f }, 0, 102);
	sgo = dynamic_cast<SpriteGo*>(AddGo(new SpriteGo("sprites/Background1a_0.png", "Main Title Background")));
	SetInitValue(sgo, Origins::TL, { 0, 0 });
	sgo = dynamic_cast<SpriteGo*>(AddGo(new SpriteGo("sprites/Background1b_0.png", "Chapter Background")));
	SetInitValue(sgo, Origins::TL, { 0, size.y });
	sgo = dynamic_cast<SpriteGo*>(AddGo(new SpriteGo("sprites/paper_overlay_0.png", "Paper Overlay")));
	SetInitValue(sgo, Origins::TL, { 0, size.y });
	

	AniSpriteGo* ago = dynamic_cast<AniSpriteGo*>(AddGo(new AniSpriteGo("board_6x6", "animations/board_6x6.csv", "Menu Board")));
	SetInitValue(ago, Origins::MC, { size * 0.5f }, 0, 101);
	//ago = dynamic_cast<AniSpriteGo*>(AddGo(new AniSpriteGo("board_6x6", "animations/board_6x6.csv", "Chapter Board")));
	//SetInitValue(ago, Origins::MC, { size.x * 0.5f, size.y * 1.5f }, 0, 101);

	ChapterBoard* chapBoard = dynamic_cast<ChapterBoard*>(AddGo(new ChapterBoard(1, "board_6x6", "animations/board_6x6.csv", "Chapter Board")));
	SetInitValue(chapBoard, Origins::MC, { size.x * 0.5f, size.y * 1.5f }, 0, 101);


	//Play Button
	UIButton* button = (UIButton*)AddGo(new UIButton("sprites/buttons_play_0.png"));
	SetInitValue(button, Origins::MC, { size.x * 0.5f, size.y * 0.4f }, 0, 103);
	button->OnEnter = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture("sprites/buttons_play_1.png");
		button->sprite.setTexture(*tex);
	};
	button->OnExit = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture(button->GetResourcePath());
		button->sprite.setTexture(*tex);
	};
	button->OnClick = [button, this, size]() {
		startCenter = uiView.getCenter();
		distance = size.y;
		remainingDistance = distance;
		currentSpeed = 5000;
		lefp = 1.f;
		dir = _DOWN;
		isSwipe = true;
	};

	//Exit Button
	button = (UIButton*)AddGo(new UIButton("sprites/buttons_exit2_0.png"));
	SetInitValue(button, Origins::MC, { size.x * 0.5f, size.y * 0.5f }, 0, 103);
	button->OnEnter = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture("sprites/buttons_exit2_1.png");
		button->sprite.setTexture(*tex);
	};
	button->OnExit = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture(button->GetResourcePath());
		button->sprite.setTexture(*tex);
	};
	button->OnClick = [button, this]() {
		Exit();
		window.close();
	};

	//Back to Menu
	button = (UIButton*)AddGo(new UIButton("sprites/return_to_menu_v01_0.png"));
	SetInitValue(button, Origins::MC, { size.x * 0.2f, size.y * 1.1f }, 0, 103);
	button->OnEnter = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture("sprites/return_to_menu_v01_1.png");
		button->sprite.setTexture(*tex);
	};
	button->OnExit = [button]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture(button->GetResourcePath());
		button->sprite.setTexture(*tex);
	};
	button->OnClick = [button, this, size]() {
		startCenter = uiView.getCenter();
		distance = size.y;
		remainingDistance = distance;
		currentSpeed = 5000;
		lefp = 1.f;
		dir = _UP;
		isSwipe = true;
	};

	for (auto& go : gameObjects)
	{
		go->Init();
	}
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

	Scene::Enter();	//사용할 리소스 Load + gameObjects Reset()
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

	if (isSwipe)
		SwipeAnimation(dir, distance, 5000.f, dt);
	
}

void TitleScene::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

void TitleScene::SetInitValue(GameObject* go, Origins origin, sf::Vector2f pos, float angle, int layer)
{
	go->SetOrigin(origin);
	go->SetPosition(pos);
	go->SetRotation(angle);
	go->sortLayer = layer;
}

//함수를 호출할 때 이외에 Button을 누를 때도 값을 따로 세팅해 줘야 하기 때문에 수정 필요
void TitleScene::SwipeAnimation(sf::Vector2f dir, float dis, float speed, float dt)
{
	currentSpeed = speed * lefp;
	uiView.move(dir * currentSpeed * dt);
	remainingDistance -= currentSpeed * dt;
	lefp = remainingDistance / dis;	//1...0

	if (1.f >= remainingDistance)
	{
		uiView.setCenter(startCenter + dir * dis);
		isSwipe = false;
	}
}

