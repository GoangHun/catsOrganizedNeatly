#include "stdafx.h"
#include "Cat.h"

#include "rapidcsv.h"
#include "ResourceMgr.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "DataTableMgr.h"

#include "Scene.h"
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

	//RectangleShape Position & Rotation Update
	for (int i = 0; i < boxNumber.x; i++)
	{
		for (int j = 0; j < boxNumber.y; j++)
		{
			int index = i * boxNumber.y + j;
			boxs[index].shape.setPosition(sprite.getPosition());
			boxs[index].shape.setRotation(sprite.getRotation());

			//test code
			if (SCENE_MGR.GetCurrScene()->isDeveloperMode)
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
	sf::Vector2f worldMousePos = SCENE_MGR.GetCurrScene()->ScreenToWorldPos(mousePos);

	bool prevHover = isHover;
	isHover = sprite.getGlobalBounds().contains(worldMousePos);

	//isCatch
	if (isHover && INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left))
	{
		isCatch = true;
	}

	if (isCatch)
	{
		//board->SetAllRoomIsUse();
		ClearBoxs();
		OnClickHold(worldMousePos);		//마우스 포인터가 빨리 움직여서 퍼즐 밖으로 빠져나가도 isCatch 상태를 유지

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
			sf::Vector2f startPos = { sprite.getGlobalBounds().left, sprite.getGlobalBounds().top };
			for (auto& room : *rooms)
			{
				sf::Vector2f arrivalPos = { room.shape.getGlobalBounds().left, room.shape.getGlobalBounds().top };
				float distance = Utils::Distance(startPos, arrivalPos);
				if (distance < 28)	//room 길이의 절반에 조금 못 미치는 값
				{
					SetPosition(position + (arrivalPos - startPos));
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
		//사용중인지 체크하고 사용중이면 원래 위치로
		if (isSnap)
		{
			if (!board->SetRoomIsUse(this))
			{
				SetPosition(startPos);
			}
		}
		board->SetIsCatchCat(nullptr);
		isCatch = false;
	}

	//퍼즐판에 놓여진 상태에서 회전시 퍼즐판의 isUse 상태를 갱신해야함.
	//Rotation
	if (!isRotation && isHover && INPUT_MGR.GetMouseButtonUp(sf::Mouse::Right))
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
		SetPosition(worldMousePos);
		board->SetIsCatchCat(this);
		return;
	}

	if (Utils::Distance(GetPosition(), worldMousePos) > 62)
		SetPosition(worldMousePos);
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
			//오리진을 부모 객체로 두는 sprite의 오리진과 동일하게 설정함. 부모 객체의 위성처럼 움직이게 됨.
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

void Cat::ClearBoxs()
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





