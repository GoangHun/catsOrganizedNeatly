#include "stdafx.h"
#include "Cat.h"
#include "ResourceMgr.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "DeveloperScene.h"
#include "rapidcsv.h"

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

	auto it = std::find(types.begin(), types.end(), (int)type);

	if (it != types.end())
	{
		animationId = aniPaths[*it - 1];
		animation.AddClip(*RESOURCE_MGR.GetAnimationClip(aniPaths[*it - 1]));
		animation.SetTarget(&sprite);
		sortLayer = 50;
		tex = RESOURCE_MGR.GetTexture(spritePaths[*it - 1]);
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
			boxs[index].setPosition(sprite.getPosition());
			boxs[index].setRotation(sprite.getRotation());
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
	if (isHover && INPUT_MGR.GetMouseButton(sf::Mouse::Left))
	{
		isCatch = true;
	}

	if (isCatch)
	{
		OnClickHold(worldMousePos);
	}
	if (isCatch && INPUT_MGR.GetMouseButtonUp(sf::Mouse::Left))
	{
		isCatch = false;
	}

	//Rotation
	if (!isRotation && isHover && INPUT_MGR.GetMouseButtonUp(sf::Mouse::Right))
	{
		isRotation = true;
		startAngle = sprite.getRotation();
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
		sprite.setRotation(rotationAngle);
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
		window.draw(box);
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
}

void Cat::Makeboxs()
{
	float width = sprite.getTextureRect().width;
	float height = sprite.getTextureRect().height;
	int w = boxNumber.x = floor(width / 62);
	int h = boxNumber.y = floor(height / 62);
	
	boxSize.x = width / w;
	boxSize.y = height / h;

	sf::FloatRect spriteBounds = sprite.getGlobalBounds();

	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			sf::RectangleShape box;	
			box.setSize(boxSize);
			box.setFillColor(sf::Color::Transparent);
			//develop
			{
				box.setOutlineThickness(1.f);
				box.setOutlineColor(sf::Color::White);
			}

			box.setOrigin(-(spriteBounds.left + i * boxSize.x), -(spriteBounds.top + j * boxSize.y));
			boxs.push_back(box);
		}
	}
}

//void Cat::SetActiveBox()
//{
//	rapidcsv::Document doc("scripts/cat_box_info.csv");
//	std::vector<int> types = doc.GetColumn<int>(0);
//	std::vector<std::string> paths = doc.GetColumn<std::string>(1);
//
//	auto it = std::find(types.begin(), types.end(), type);
//	if (it != types.end())
//	{
//		std::string str = paths[*it];
//
//		std::vector<std::string> tokens;
//		std::istringstream iss(str);
//		std::string token;
//
//		while (std::getline(iss, token, ',')) 
//		{
//			tokens.push_back(token);
//		}
//
//		for (auto info : tokens)
//		{
//			activeBoxInfo.push_back(std::stoi(info));
//		}
//	}
//}




