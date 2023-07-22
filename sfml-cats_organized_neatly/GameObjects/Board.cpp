#include "stdafx.h"
#include "Board.h"
#include "ResourceMgr.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "DeveloperScene.h"

/*
stageId		boardType
*/
void Board::Init()
{
	SetOrigin(Origins::MC);
	SetPosition(0, 0);
	sortLayer = 0;

	SetBoard(boardInfo.type);	//빼고 다른 곳에서 따로 해주는 게 좋을 거 같기도... 

	animation.SetTarget(&sprite);
	tilePool.Init();
}

void Board::Release()
{
	tilePool.Release();
}

void Board::Reset()
{
	animation.Play(boardInfo.animationId);
	sf::Vector2i size = (sf::Vector2i)sprite.getTexture()->getSize();
	sprite.setTextureRect({0, 0, size.x, size.y});
	SetOrigin(Origins::MC);
	SetPosition({ 0, 0 });
}

void Board::Update(float dt)
{
	animation.Update(dt);

	auto* scene = SCENE_MGR.GetCurrScene();
	if (scene->isDeveloperMode)
	{
		sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
		sf::Vector2f worldMousePos = SCENE_MGR.GetCurrScene()->ScreenToWorldPos(mousePos);
		DeveloperScene* scene = (DeveloperScene*)SCENE_MGR.GetCurrScene();
		bool isCatch = scene->GetIsCatch();

		for (auto& sRoom : rooms)
		{
			sRoom.shape.setOutlineColor(sf::Color::White);

			sRoom.prevHover = sRoom.isHover;
			sRoom.isHover = sRoom.shape.getGlobalBounds().contains(worldMousePos);
			if (!sRoom.prevHover && sRoom.isHover && !isCatch)
			{
				OnEnter(sRoom);
			}
			if (sRoom.prevHover && !sRoom.isHover && !isCatch)
			{
				OnExit(sRoom);
			}
			if (sRoom.isHover && INPUT_MGR.GetMouseButtonUp(sf::Mouse::Left) && !isCatch)
			{
				OnClick(sRoom);
			}

			//퍼즐 충돌 체크
			//isCatch 상태인 Cat과 체크
			if (isCatchCat != nullptr)
			{
				std::vector<Box> boxs = isCatchCat->GetBoxs();
				for (auto& box : boxs)
				{
					if (sRoom.shape.getGlobalBounds().intersects(box.shape.getGlobalBounds()) && sRoom.isFull
						&& box.isActive)
					{
						//테스트 코드
						std::cout << "test" << std::endl;
						sRoom.shape.setOutlineThickness(3.f);
						sRoom.shape.setOutlineColor(sf::Color::Red);
					}	
				}	
			}
		}
		return;
	}
}

void Board::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	for (auto structRoom : rooms)
	{
		window.draw(structRoom.shape);
	}
}

void Board::ClearRooms()
{
	for (auto& room : rooms)
	{
		tilePool.Return(room.tile);
		room.tile = nullptr;
	}
	rooms.clear();
}

void Board::SetBoard(BoardType type)
{	
	switch(type)
	{
	case BoardType::_3X3:
		animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/board_3x3.csv"));
		MakeRooms(type);
		SetBoardInfo(type, "board_3x3");
		break;
	case BoardType::_4X4:
		animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/board_4x4.csv"));
		MakeRooms(type);
		SetBoardInfo(type, "board_4x4");
		break;
	case BoardType::_5X5:
		animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/board_5x5.csv"));
		MakeRooms(type);
		SetBoardInfo(type, "board_5x5");
		break;
	case BoardType::_6X6:
		animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/board_6x6.csv"));
		MakeRooms(type);
		SetBoardInfo(type, "board_6x6");
		break;
	case BoardType::_7X7:
		animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/board_7x7.csv"));
		MakeRooms(type);
		SetBoardInfo(type, "board_7x7");
		break;
	case BoardType::_8X8:
		animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/board_8x8.csv"));
		MakeRooms(type);
		SetBoardInfo(type, "board_8x8");
		break;
	}
}

void Board::MakeRooms(BoardType type)
{
	ClearRooms();

	//room size 62x62
	int roomSize = 62;
	int roomNumber = pow((int)type, 2);
	float coord;

	if ((int)type % 2 == 0)
	{
		coord = roomSize * ((int)type - 1) / 2.0;	//1.5, 2.5, 3.5...* 62
	}
	else
	{
		coord = roomSize * floor((int)type * 0.5f);	//1, 2, 3...* 62
	}
	
	sf::Vector2f pos = { -coord, -coord };	//첫 번째 퍼즐의 좌표(좌상단)

	for (int i = 0; i < roomNumber; i++)
	{
		Room room;
		room.shape.setSize( sf::Vector2f(roomSize, roomSize));
		Utils::SetOrigin(room.shape, Origins::MC);

		room.shape.setFillColor(sf::Color::Transparent);
		rooms.push_back(room);
	}

	for (int i = 0; i < (int)type; i++)
	{
		for (int j = 0; j < (int)type; j++)
		{
			int index = i * (int)type + j;
			rooms[index].shape.setPosition({pos.x + roomSize * j, pos.y + roomSize * i});	//행부터 차례대로 셋포지션
		}
	}
}

void Board::OnClick(Room& sRoom)
{
	if (!sRoom.isFull)
	{
		sRoom.tile = tilePool.Get();
		sRoom.tile->SetPosition(sRoom.shape.getPosition());
		SCENE_MGR.GetCurrScene()->AddGo(sRoom.tile);
		sRoom.isFull = true;
	}
	else
	{
		tilePool.Return(sRoom.tile);
		SCENE_MGR.GetCurrScene()->RemoveGo(sRoom.tile);
		sRoom.isFull = false;	
	}
}

void Board::OnEnter(Room& sRoom)
{
	sRoom.shape.setOutlineThickness(1.f);
	sRoom.shape.setOutlineColor(sf::Color::Red);
}

void Board::OnExit(Room& sRoom)
{
}
