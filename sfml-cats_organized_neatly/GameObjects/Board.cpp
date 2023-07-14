#include "stdafx.h"
#include "Board.h"
#include "ResourceMgr.h"

/*
stageId		boardType

*/

void Board::Init()
{
	//animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/board_3x3.csv"));
	SetBoard(BoardType::_4X4);
	animation.SetTarget(&sprite);
}

void Board::Reset()
{
	animation.Play("board_4x4");
	SetOrigin(Origins::MC);
	SetPosition({ 0, 0 });
	center = GetPosition();
	sortLayer = 0;
}

void Board::Update(float dt)
{
	animation.Update(dt);
}

void Board::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	for (auto room : rooms)
	{
		window.draw(room);
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
		sf::RectangleShape room;
		room.setSize( sf::Vector2f(roomSize, roomSize));
		Utils::SetOrigin(room, Origins::MC);
		room.setFillColor(sf::Color::Blue);
		rooms.push_back(room);
	}

	for (int i = 0; i < (int)type; i++)
	{
		for (int j = 0; j < (int)type; j++)
		{
			int index = i * (int)type + j;
			rooms[index].setPosition({pos.x + roomSize * j, pos.y + roomSize * i});
		}
	}
}
