#include "stdafx.h"
#include "Board.h"
#include "ResourceMgr.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
/*
stageId		boardType
*/
void Board::Init()
{
	SetBoard(boardInfo.type);
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
	SetOrigin(Origins::MC);
	SetPosition({ 0, 0 });
	sortLayer = 0;
}

void Board::Update(float dt)
{
	animation.Update(dt);

	//개발자 모드 
	if (isDeveloperMode)
	{
		sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
		sf::Vector2f worldMousePos = SCENE_MGR.GetCurrScene()->ScreenToWorldPos(mousePos);

		for (auto sRoom : rooms)
		{
			bool prevHover = sRoom.isHover;
			sRoom.isHover = sRoom.room.getGlobalBounds().contains(worldMousePos);
			if (!prevHover && sRoom.isHover)
			{
				OnEnter(sRoom);
			}

			if (prevHover && !sRoom.isHover)
			{
				OnExit(sRoom);
			}
			if (sRoom.isHover && INPUT_MGR.GetMouseButtonUp(sf::Mouse::Left))
			{
				OnClick(sRoom);
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
		window.draw(structRoom.room);
	}
}

void Board::SetBoard(BoardType type)
{	
	switch(type)
	{
	case BoardType::_3X3:
		animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/board_3x3.csv"));
		SetRoomPos(type);
		break;
	case BoardType::_4X4:
		animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/board_4x4.csv"));
		SetRoomPos(type);
		break;
	case BoardType::_5X5:
		animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/board_5x5.csv"));
		SetRoomPos(type);
		break;
	case BoardType::_6X6:
		animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/board_6x6.csv"));
		SetRoomPos(type);
		break;
	case BoardType::_7X7:
		animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/board_7x7.csv"));
		SetRoomPos(type);
		break;
	case BoardType::_8X8:
		animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/board_8x8.csv"));
		SetRoomPos(type);
		break;
	}
}

void Board::SetRoomPos(BoardType type)
{
	//room size 64x64
	int roomSize = 64;
	int roomNumber = pow((int)type, 2);
	float coord;

	if ((int)type % 2 == 0)
	{
		coord = roomSize * ((int)type - 1) / 2.0;
	}
	else
	{
		coord = roomSize * floor((int)type * 0.5f);
	}
	sf::Vector2f pos = { -coord, -coord };
	std::cout << coord << std::endl;

	for (int i = 0; i < roomNumber; i++)
	{
		Room structRoom;
		structRoom.room.setSize( sf::Vector2f(roomSize, roomSize));
		Utils::SetOrigin(structRoom.room, Origins::MC);
		//개발자 모드
		{
			//structRoom.room.setOutlineThickness(5.f);
			//structRoom.room.setOutlineColor(sf::Color::Blue);
		}
		structRoom.room.setFillColor({ 255, 255, 255, 0 });
		rooms.push_back(structRoom);
	}

	for (int i = 0; i < (int)type; i++)
	{
		for (int j = 0; j < (int)type; j++)
		{
			int index = i * (int)type + j;
			rooms[index].room.setPosition({pos.x + roomSize * j, pos.y + roomSize * i});
		}
	}
}

void Board::OnClick(Room sRoom)
{
	if (!sRoom.isFull)
	{
		sRoom.tile = tilePool.Get();
		sRoom.tile->SetPosition(sRoom.room.getPosition());
		SCENE_MGR.GetCurrScene()->AddGo(sRoom.tile);
		sRoom.isFull = true;
	}
	else
	{
		tilePool.Return(sRoom.tile);
		SCENE_MGR.GetCurrScene()->RemoveGo(sRoom.tile);
		sRoom.isFull = false;
		std::cout << sRoom.tile->GetActive() << std::endl;
	}
}

void Board::OnEnter(Room sRoom)
{
}

void Board::OnExit(Room sRoom)
{
}
