#include "stdafx.h"
#include "Cat.h"

#include "rapidcsv.h"
#include "ResourceMgr.h"
#include "InputMgr.h"
#include "SceneMgr.h"

#include "Scene.h"
#include "DeveloperScene.h"

#include "Board.h"

Cat::Cat(const CatTypes type, const std::string& textureId, const std::string& n)
	: SpriteGo(textureId, n), type(type)
{
}

void Cat::Init()
{
	SpriteGo::Init();

	rapidcsv::Document doc("scripts/cat_info.csv");
	std::vector<int> types = doc.GetColumn<int>(0);
	std::vector<std::string> aniPaths = doc.GetColumn<std::string>(1);
	std::vector<std::string> spritePaths = doc.GetColumn<std::string>(2);
	std::vector<std::string> boxInfos = doc.GetColumn<std::string>(3);

	auto it = std::find(types.begin(), types.end(), (int)type);

	//it로 인덱스 계산되는 거 원리랑 순서 이해 안감.
	if (it != types.end())
	{
		animationId = aniPaths[*it - 1];
		animation.AddClip(*RESOURCE_MGR.GetAnimationClip(aniPaths[*it - 1]));
		animation.SetTarget(&sprite);
		sortLayer = 50;
		tex = RESOURCE_MGR.GetTexture(spritePaths[*it - 1]);
		boxInfo = boxInfos[*it - 1];
	}
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
			int index = i * boxNumber.x + j;
			boxs[index].shape.setPosition(sprite.getPosition());
			boxs[index].shape.setRotation(sprite.getRotation());
		}
	}
				
	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
	sf::Vector2f worldMousePos = SCENE_MGR.GetCurrScene()->ScreenToWorldPos(mousePos);

	bool prevHover = isHover;
	isHover = sprite.getGlobalBounds().contains(worldMousePos);

	if (!prevHover && isHover)
	{
		OnEnter();
	}

	if (prevHover && !isHover)
	{
		OnExit();
	}

	if (isHover && INPUT_MGR.GetMouseButtonUp(sf::Mouse::Left))
	{
		OnClick();
	}

	//isCatch
	if (isHover && INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left))
	{
		isCatch = true;
	}

	if (isCatch)
	{
		OnClickHold(worldMousePos);		//이렇게 함으로써 마우스 포인터가 빨리 움직여서 퍼즐 밖으로 빠져나가도 isCatch 상태를 유지
		int count = 0;
		for (auto box : boxs)
		{
			for (auto room : *rooms)
			{
				if (box.shape.getGlobalBounds().intersects(room.shape.getGlobalBounds()) && room.isFull && box.isActive)
				{
					box.isCollision = true;
				}
			}
			if (box.isCollision)
			{
				count++;
			}
		}
		if (count == boxs.size())
		{
			sf::Vector2f sPos = { sprite.getGlobalBounds().top, sprite.getGlobalBounds().left };
			for (auto room : *rooms)
			{
				sf::Vector2f gPos = { room.shape.getGlobalBounds().top, room.shape.getGlobalBounds().left };
				float distance = Utils::Distance(sPos, gPos);
				if (distance < 31)
				{
					SetPosition(position + (gPos - sPos));
					isCatch = false;
				}
			}
		}
		
		
	}

	if (isCatch && INPUT_MGR.GetMouseButtonUp(sf::Mouse::Left))
	{
		board->SetIsCatchCat(nullptr);
		isCatch = false;
	}

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
		/*int x = boxSize.x;
		int y = boxSize.y;
		for (int i = 0; i < boxNumber.x; i++)
		{
			for (int j = 0; j < boxNumber.y; j++)
			{
				int index = i * boxNumber.x + j;
				boxs[index].setRotation(rotationAngle);

				sf::Vector2f boxOffset;

				if (boxNumber.x % 2 == 0)
				{
					boxOffset.x = i * boxSize.x + -(boxSize.x * (boxNumber.x / 2 - 0.5f));
				}
				else
				{
					if (boxNumber.x == 1)
						boxOffset.x = 0.f;
					else
						boxOffset.x = i * boxSize.x + -(boxSize.x * floor(boxNumber.x / 2.f));
				}

				if (boxNumber.y % 2 == 0)
				{
					boxOffset.y = j * boxSize.y + -(boxSize.y * (boxNumber.y / 2 - 0.5f));
				}
				else
				{
					if (boxNumber.y == 1)
						boxOffset.y = 0.f;
					else
						boxOffset.y = j * boxSize.y + -(boxSize.y * floor(boxNumber.y / 2.f));
				}

				sf::Transform transform;
				transform.rotate(rotationAngle, sprite.getPosition());
				sf::Vector2f transformedOffset = transform.transformPoint(localPoss[index]);
				boxs[index].setPosition(sprite.getPosition() + transformedOffset);
			}
		}*/
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

void Cat::OnClick()
{
	
}

void Cat::OnEnter()
{
}

void Cat::OnExit()
{
}

void Cat::OnClickHold(sf::Vector2f worldMousePos)
{
	sprite.setTexture(*tex);
	SetPosition(worldMousePos);
	board->SetIsCatchCat(this);
}

void Cat::Makeboxs()
{
	float width = sprite.getTextureRect().width;
	float height = sprite.getTextureRect().height;
	int w = boxNumber.x = floor(width / 62);
	int h = boxNumber.y = floor(height / 62);
	
	boxSize = { 62, 62 };

	sf::FloatRect spriteBounds = sprite.getGlobalBounds();

	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			sf::RectangleShape shape;	
			shape.setSize(boxSize);
			shape.setFillColor(sf::Color::Transparent);
			//develop
			{
				shape.setOutlineThickness(1.f);
				shape.setOutlineColor(sf::Color::White);
			}

			shape.setOrigin(-(spriteBounds.left + i * boxSize.x), -(spriteBounds.top + j * boxSize.y));
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

	bool boxState = boxStates.front();
	for (int i = 0; i < boxs.size(); i++)
	{
		boxs[i].isActive = boxState;
	}

}

void Cat::SetBoard(Board* board)
{
	this->board = board;
	this->rooms = board->GetRooms();
}





