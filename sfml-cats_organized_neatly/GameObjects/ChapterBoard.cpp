#include "stdafx.h"
#include "ChapterBoard.h"

#include "Framework.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"

ChapterBoard::ChapterBoard(int chapterNum, const std::string& animationId, const std::string& resourcePath, const std::string& n)
	: AniSpriteGo(animationId, resourcePath, n)
{
}

void ChapterBoard::Init()
{
	AniSpriteGo::Init();

	auto size = FRAMEWORK.GetWindowSize();

	chapterPot.resourcePath = "sprites/menu_blocker_0002_Ebene_1_0.png";
	chapterPot.SetName("Chapter Pot");
	SetInitValue(&chapterPot, Origins::MC, { size.x * 0.5f, size.y * 1.5f }, 0, 102);
	backgroundPaper.SetResourcePath("sprites/background_paper_0.png");
	backgroundPaper.SetName("Background Paper");
	SetInitValue(&backgroundPaper, Origins::MC, { size.x * 0.5f, size.y * 1.5f });

	SetButtonPositions();
}

void ChapterBoard::Reset()
{
	AniSpriteGo::Reset();

	auto* scene = SCENE_MGR.GetCurrScene();
	scene->AddGo(&chapterPot);
	scene->AddGo(&backgroundPaper);

	bool buttonInfos[16] = {
	true, true, false, true,
	true, true, false, true, 
	false, true, false, false,
	false, true, true, true};

	int stageNum = 1;
	for (int i = 0; i < buttonPoss.size(); i++)
	{
		if (buttonInfos[i])
		{
			UIButton* button = (UIButton*)SCENE_MGR.GetCurrScene()->AddGo(new UIButton("sprites/board_menu_0.png"));
			SetInitValue(button, Origins::TL, buttonPoss[i], 0, 101);
			button->SetText("fonts/ShadowsIntoLight-Regular.ttf", 102, { 71, 39, 58, 255}, std::to_string(stageNum),
				Origins::ML, { buttonPoss[i].x + 35, buttonPoss[i].y + 30});
			button->OnEnter = [button]() {
				sf::Texture* tex = RESOURCE_MGR.GetTexture("sprites/board_menu_1.png");
				button->sprite.setTexture(*tex);
			};
			button->OnExit = [button]() {
				sf::Texture* tex = RESOURCE_MGR.GetTexture(button->GetResourcePath());
				button->sprite.setTexture(*tex);
			};
			button->OnClick = [button, this]() {
			};
			uiButtons.push_back(button);
			stageNum++;
		}
	}
}

void ChapterBoard::Update(float dt)
{
	AniSpriteGo::Update(dt);
}

void ChapterBoard::Draw(sf::RenderWindow& window)
{
	AniSpriteGo::Draw(window);
}

void ChapterBoard::SetButtonPositions()
{
	int interval = 112;

	sf::Vector2f leftTopPos = { position.x - interval * 2, position.y - interval * 2 };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			sf::Vector2f pos = {leftTopPos.x + interval * j, leftTopPos.y + interval * i };
			buttonPoss.push_back(pos);
		}
	}
}

void ChapterBoard::SetInitValue(GameObject* go, Origins origin, sf::Vector2f pos, float angle, int layer)
{
	go->SetOrigin(origin);
	go->SetPosition(pos);
	go->SetRotation(angle);
	go->sortLayer = layer;
}