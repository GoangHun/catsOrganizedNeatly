#include "stdafx.h"
#include "Board.h"

#include "ResourceMgr.h"
#include "InputMgr.h"
#include "SceneMgr.h"

#include "Scene.h"
#include "GameScene.h"
#include "DeveloperScene.h"

/*
stageId		boardType
*/
void Board::Init()
{
	SetOrigin(Origins::MC);
	SetPosition(0, 0);
	sortLayer = 0;
	
	ObjectPool<Tile>* ptr = &tilePool;
	tilePool.OnCreate = [ptr](Tile* tile)
	{
		tile->resourcePath = "sprites/board_tile_0.png";
	};

	animation.SetTarget(&sprite);
	tilePool.Init();
}

void Board::Release()
{
	tilePool.Release();
}

void Board::Reset()
{
	SetBoard(boardInfo.type);
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

	//Map Editor
	if (scene->isDeveloperMode)
	{
		sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
		sf::Vector2f worldMousePos = scene->ScreenToWorldPos(mousePos);

		for (auto& sRoom : rooms)
		{
			sRoom.prevHover = sRoom.isHover;
			sRoom.isHover = sRoom.shape.getGlobalBounds().contains(worldMousePos);

			if (!sRoom.prevHover && sRoom.isHover)
			{
				OnEnter(sRoom);
			}
			if (sRoom.prevHover && !sRoom.isHover)
			{
				OnExit(sRoom);
			}
			if (sRoom.isHover && INPUT_MGR.GetMouseButtonUp(sf::Mouse::Left))
			{
				OnClick(sRoom);
			}

			//���� �浹 üũ + isCatch ������ Cat�� üũ
			if (isCatchCat != nullptr)
			{
				std::vector<Box>* boxs = isCatchCat->GetBoxs();
				for (auto& box : *boxs)
				{
					if (!box.isActive)
						continue;

					sf::Vector2f center = { box.shape.getGlobalBounds().left + 31, box.shape.getGlobalBounds().top + 31 };

					if (sRoom.shape.getGlobalBounds().contains(center))
					{
						sRoom.shape.setOutlineThickness(2.f);
						sRoom.shape.setOutlineColor(sf::Color::Red);
						break;
					}
					else
					{
						sRoom.shape.setOutlineThickness(0.f);
					}
				}
			}
		}
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

void Board::SetAllRoomIsUse()
{
	const std::vector<Cat*>& cats = dynamic_cast<GameScene*>(SCENE_MGR.GetCurrScene())->GetCats();
	std::vector<Box> allBoxs;

	for (auto& cat : cats)
	{
		const std::vector<Box>* boxs = cat->GetBoxs();
		for (auto& box : *boxs)
		{
			allBoxs.push_back(box);
		}
	}
	
	for (auto& room : rooms)
	{
		if (room.tile == nullptr)
			continue;
		for (auto& box : allBoxs)
		{
			if (!box.isActive)
				continue;

			if (room.shape.getGlobalBounds().contains(box.shape.getGlobalBounds().left + 31, box.shape.getGlobalBounds().top + 31))
			{
				room.isUse = true;
				break;
			}
			room.isUse = false;
		}
	}
}


void Board::ClearRooms()
{
	if (rooms.empty())
		return;

	for (auto& room : rooms)
	{
		if (room.tile != nullptr)
		{
			SCENE_MGR.GetCurrScene()->RemoveGo(room.tile);
			room.tile = nullptr;
		}
	}
	rooms.clear();
	tilePool.Clear();
}

bool Board::CheckBoard()
{
	for (auto& room : rooms)
	{
		if (room.tile == nullptr)
			continue;
		if (!room.isUse)
			return false;
	}
	return true;
}

bool Board::SetRoomIsUse(Cat* cat)
{
	std::vector<Box>* boxs = cat->GetBoxs();
	for (auto& box : *boxs)
	{
		if (!box.isActive)
			continue;

		sf::FloatRect bounds = box.shape.getGlobalBounds();
		for (auto& room : rooms)
		{
			if (room.isUse || room.tile == nullptr)
				continue;

			if (room.shape.getGlobalBounds().contains(bounds.left + 31, bounds.top + 31))
			{
				box.room = &room;
				room.isUse = true;
				break;
			}
		}
		if (box.room == nullptr)
		{
			cat->ClearRooms();
			return false;
		}		
	}
	return true;
}

void Board::SetBoard(BoardType type)
{	
	std::cout << "SetBoard(" << (int)type << ")" << std::endl;
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
	
	sf::Vector2f pos = { -coord, -coord };	//ù ��° ������ ��ǥ(�»��)

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
			rooms[index].shape.setPosition({pos.x + roomSize * j, pos.y + roomSize * i});	//����� ���ʴ�� ��������
		}
	}
}


void Board::OnClick(Room& sRoom)
{
	if (sRoom.tile == nullptr)
	{
		sRoom.tile = tilePool.Get();
		sRoom.tile->SetPosition(sRoom.shape.getPosition());
		SCENE_MGR.GetCurrScene()->AddGo(sRoom.tile);
	}
	else
	{
		tilePool.Return(sRoom.tile);
		SCENE_MGR.GetCurrScene()->RemoveGo(sRoom.tile);
		sRoom.tile = nullptr;
	}
}

void Board::OnEnter(Room& sRoom)
{
	sRoom.shape.setOutlineThickness(2.f);
	sRoom.shape.setOutlineColor(sf::Color::White);
}

void Board::OnExit(Room& sRoom)
{
	sRoom.shape.setOutlineThickness(0.f);
}
