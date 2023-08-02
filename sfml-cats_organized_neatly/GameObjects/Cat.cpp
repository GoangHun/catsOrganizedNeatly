#include "stdafx.h"
#include "Cat.h"

#include <typeinfo>

#include "rapidcsv.h"
#include "ResourceMgr.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "DataTableMgr.h"

#include "Scene.h"
#include "GameScene.h"
#include "DeveloperScene.h"
#include "CatTable.h"

#include "Board.h"

Cat::Cat(const CatTypes type, const std::string& textureId, const std::string& n)
	: SpriteGo(textureId, n), type(type)
{
}

void Cat::Init()
{
	SpriteGo::Init();

	std::tuple<std::string, std::string, std::string> info = DATATABLE_MGR.Get<CatTable>(DataTable::Ids::Cat)->Get((int)type);

	animationId = std::get<0>(info);
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip(animationId));
	animation.SetTarget(&sprite);
	sortLayer = 50;
	tex = RESOURCE_MGR.GetTexture(std::get<1>(info));
	boxInfo = std::get<2>(info);
}

void Cat::Reset()
{
	animation.Play("cat_c" + std::to_string((int)type));
	sf::Vector2i size = (sf::Vector2i)tex->getSize();
	sprite.setTextureRect({ 0, 0, size.x, size.y });
	SetOrigin(Origins::MC);
	SetPosition({ 0, 0 });
	isHover = false;
	
	if (!boxs.empty())
	{
		boxs.clear();
	}
	Makeboxs();
	SetBoxState();
}

void Cat::Update(float dt)
{
	if (!isHover || !INPUT_MGR.GetMouseButton(sf::Mouse::Left))
	{
		animation.Update(dt);
	}

	Scene* scene = SCENE_MGR.GetCurrScene();

	//RectangleShape Position & Rotation Update
	for (int i = 0; i < boxNumber.x; i++)
	{
		for (int j = 0; j < boxNumber.y; j++)
		{
			int index = i * boxNumber.y + j;
			boxs[index].shape.setPosition(sprite.getPosition());
			boxs[index].shape.setRotation(sprite.getRotation());

			//test code
			if (scene->isDeveloperMode)
			{
				boxs[index].shape.setOutlineThickness(1.f);
				boxs[index].shape.setOutlineColor(sf::Color::Green);
			}
			else
			{
				boxs[index].shape.setOutlineThickness(0.f);
			}
		}
	}
				
	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
	sf::Vector2f worldMousePos = scene->ScreenToWorldPos(mousePos);

	bool prevHover = isHover;
	isHover = sprite.getGlobalBounds().contains(worldMousePos);

	//isCatch
	if (isHover && INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left) && board->GetIsCatchCat() == nullptr)
	{
		isCatch = true;
		board->SetIsCatchCat(this);
	}
	if (isCatch)
	{
		ClearRooms();
		OnClickHold(worldMousePos);		//���콺 �����Ͱ� ���� �������� ���� ������ ���������� isCatch ���¸� ����
		int collisionCount = 0;
		for (auto& box : boxs)
		{	
			if (!box.isActive)
				continue;

			for (auto& room : *rooms)
			{
				if (room.tile == nullptr)
					continue;

				if (box.shape.getGlobalBounds().contains(room.shape.getPosition()))
				{
					collisionCount++;
					box.shape.setOutlineColor(sf::Color::Blue);
					break;
				}
			}
		}
		if (collisionCount == activeBoxNum)
		{
			sf::Vector2f catPos = { sprite.getGlobalBounds().left, sprite.getGlobalBounds().top };
			for (auto& room : *rooms)
			{
				sf::Vector2f roomPos = { room.shape.getGlobalBounds().left, room.shape.getGlobalBounds().top };
				float distance = Utils::Distance(catPos, roomPos);
				if (distance < 28)	//room ������ ���ݿ� ���� �� ��ġ�� ��
				{
					SetPosition(position + (roomPos - catPos));
					isSnap = true;
					break;
				}
			}
		}
		else
			isSnap = false;
	}

	if (isCatch && INPUT_MGR.GetMouseButtonUp(sf::Mouse::Left))
	{
		//��������� üũ�ϰ� ������̸� ���� ��ġ��
		if (isSnap)
		{
			if (!board->SetRoomIsUse(this))
				SetPosition(startPos);
		}
		else
		{
			std::vector<Cat*> cats;
			auto* sc = dynamic_cast<GameScene*>(scene);
			if (sc != nullptr)
				cats = sc->GetCats();
			else
				cats = dynamic_cast<DeveloperScene*>(scene)->GetCats();

			for (auto& cat : cats)
			{
				if (cat == this)
					continue;
				if (sprite.getGlobalBounds().intersects(cat->sprite.getGlobalBounds()))
					SetPosition(startPos);
			}
		}
		board->SetIsCatchCat(nullptr);
		sortLayer = 50;
		isCatch = false;
	}

	//�����ǿ� ������ ���¿��� ȸ���� �������� isUse ���¸� �����ؾ���.
	//Rotation
	if (!isRotation && isHover && INPUT_MGR.GetMouseButtonUp(sf::Mouse::Right)&& board->GetIsCatchCat() == this)
	{
		isRotation = true;
		startAngle = GetRotation();
		targetRotation = startAngle + 90.f;
		rotationAngle = startAngle;
	}
	if (isRotation)
	{
		rotationAngle += rotationSpeed * dt;
		if (rotationAngle > targetRotation)
		{
			rotationAngle = targetRotation;
			isRotation = false;
		}
		SetRotation(rotationAngle);
	}
}

void Cat::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);

	for (auto box : boxs)
	{
		window.draw(box.shape);
	}
}

void Cat::OnClickHold(sf::Vector2f worldMousePos)
{
	if (!isSnap)
	{
		sprite.setTexture(*tex);
		sortLayer = 51;
		SetPosition(worldMousePos);
		return;
	}
	else
	{
		if (Utils::Distance(GetPosition(), worldMousePos) > 62)
		{
			sf::Vector2f direction = Utils::Normalize(worldMousePos - GetPosition());
			if (abs(direction.x) > abs(direction.y))
			{
				if (direction.x > 0)
					direction = _RIGHT;
				else
					direction = _LEFT;
			}
			else
			{
				if (direction.y > 0)
					direction = _DOWN;
				else
					direction = _UP;
			}
			SetPosition(GetPosition() + direction * 62.f);
		}
	}
}

void Cat::Makeboxs()
{
	float width = sprite.getTextureRect().width;
	float height = sprite.getTextureRect().height;
	boxNumber.x = floor(width / 62);
	boxNumber.y = floor(height / 62);
	
	boxSize = { 62, 62 };

	sf::FloatRect spriteBounds = sprite.getGlobalBounds();

	for (int i = 0; i < boxNumber.y; i++)
	{
		for (int j = 0; j < boxNumber.x; j++)
		{
			sf::RectangleShape shape;	
			shape.setSize(boxSize);
			shape.setFillColor(sf::Color::Transparent);
			//�������� �θ� ��ü�� �δ� sprite�� �������� �����ϰ� ������. �θ� ��ü�� ����ó�� �����̰� ��.
			shape.setOrigin(-(spriteBounds.left + j * boxSize.x), -(spriteBounds.top + i * boxSize.y));
			boxs.push_back({ shape, false });
		}
	}
}

void Cat::SetBoxState()
{
	std::stringstream ss(boxInfo);
	std::string token;
	std::vector<bool> boxStates;

	while (std::getline(ss, token, ',')) {
		boxStates.push_back(std::stoi(token));
	}

	for (int i = 0; i < boxs.size(); i++)
	{
		boxs[i].isActive = boxStates[i];
		if (boxs[i].isActive)
			activeBoxNum++;
	}
}

void Cat::ClearRooms()	//Cat�� Box�� �����Ǿ� �ִ� Board�� Room�� �����
{
	for (auto& box : boxs)
	{
		if (box.room != nullptr)
		{
			box.room->isUse = false;
			box.room = nullptr;
		}	
	}
}

void Cat::SetBoard(Board* board)
{
	this->board = board;
	this->rooms = board->GetRooms();
}





