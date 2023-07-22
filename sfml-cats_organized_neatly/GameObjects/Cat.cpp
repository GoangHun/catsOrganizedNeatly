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
			int index = i * boxNumber.y + j;
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
		//마우스 포인터가 빨리 움직여서 퍼즐 밖으로 빠져나가도 isCatch 상태를 유지
		OnClickHold(worldMousePos);		

		int count = 0;
		for (auto& box : boxs)
		{
			if (SCENE_MGR.GetCurrScene()->isDeveloperMode)
			{
				box.shape.setOutlineThickness(1.f);
				box.shape.setOutlineColor(sf::Color::Green);
			}

			for (auto& room : *rooms)
			{
				//intersects가 아니라 contains를 써서 room의 좌표를 확인하는 게 좋을듯
				if (box.shape.getGlobalBounds().contains(room.shape.getPosition()))
				{

				}

				if (box.shape.getGlobalBounds().intersects(room.shape.getGlobalBounds()) && room.isFull && box.isActive)
				{	
					box.isCollision = true;
					box.shape.setOutlineColor(sf::Color::Blue);
				}
			}
			if (box.isCollision)
			{
				count++;
			}
		}
		if (count == boxs.size())
		{
			sf::Vector2f sPos = { sprite.getGlobalBounds().left, sprite.getGlobalBounds().top };
			for (auto& room : *rooms)
			{
				if (room.isFull)
				{
					sf::Vector2f gPos = { room.shape.getGlobalBounds().left, room.shape.getGlobalBounds().top };
					float distance = Utils::Distance(sPos, gPos);
					if (distance < 31)
					{
						SetPosition(position + (gPos - sPos));
						isCatch = false;
					}
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
	boxNumber.x = floor(width / 62);
	boxNumber.y = floor(height / 62);
	
	boxSize = { 62, 62 };

	sf::FloatRect spriteBounds = sprite.getGlobalBounds();

	for (int i = 0; i < boxNumber.x; i++)
	{
		for (int j = 0; j < boxNumber.y; j++)
		{
			sf::RectangleShape shape;	
			shape.setSize(boxSize);
			shape.setFillColor(sf::Color::Transparent);
			//오리진을 부모 객체로 두는 sprite의 오리진과 동일하게 설정함. 부모 객체의 위성처럼 움직이게 됨.
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





