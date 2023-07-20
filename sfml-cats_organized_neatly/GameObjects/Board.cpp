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

	//개발자 모드 
	DeveloperScene* scene = (DeveloperScene*)SCENE_MGR.GetCurrScene();
	if (scene->GetIsDeveloperMode())
	{
		sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
		sf::Vector2f worldMousePos = SCENE_MGR.GetCurrScene()->ScreenToWorldPos(mousePos);
		DeveloperScene* scene = (DeveloperScene*)SCENE_MGR.GetCurrScene();
		bool isCatch = scene->GetIsCatch();

		for (auto& sRoom : rooms)
		{
			sRoom.prevHover = sRoom.isHover;
			sRoom.isHover = sRoom.room.getGlobalBounds().contains(worldMousePos);
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
	std::cout << "SetBoard " << (int)type << "x" << (int)type << std::endl;
	switch(type)
	{
	case BoardType::_3X3:
		animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/board_3x3.csv"));
		SetRoomPos(type);
		SetBoardInfo(type, "board_3x3");
		break;
	case BoardType::_4X4:
		animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/board_4x4.csv"));
		SetRoomPos(type);
		SetBoardInfo(type, "board_4x4");
		break;
	case BoardType::_5X5:
		animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/board_5x5.csv"));
		SetRoomPos(type);
		SetBoardInfo(type, "board_5x5");
		break;
	case BoardType::_6X6:
		animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/board_6x6.csv"));
		SetRoomPos(type);
		SetBoardInfo(type, "board_6x6");
		break;
	case BoardType::_7X7:
		animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/board_7x7.csv"));
		SetRoomPos(type);
		SetBoardInfo(type, "board_7x7");
		break;
	case BoardType::_8X8:
		animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/board_8x8.csv"));
		SetRoomPos(type);
		SetBoardInfo(type, "board_8x8");
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

void Board::OnClick(Room& sRoom)
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
	}
}

void Board::OnEnter(Room& sRoom)
{
	sRoom.room.setOutlineThickness(3.f);
	sRoom.room.setOutlineColor(sf::Color::White);
	if (sRoom.isFull && INPUT_MGR.GetKeyDown(sf::Keyboard::Up))
	{
		
	}
	if (sRoom.isFull && INPUT_MGR.GetKeyDown(sf::Keyboard::Down))
	{

	}
}

void Board::OnExit(Room& sRoom)
{
	sRoom.room.setOutlineThickness(0.f);
}
