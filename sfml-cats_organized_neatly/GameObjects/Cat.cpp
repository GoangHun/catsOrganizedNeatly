#include "stdafx.h"
#include "Cat.h"
#include "ResourceMgr.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "Scene.h"

void Cat::Init()
{
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/cat_c3.csv"));
	animation.SetTarget(&sprite);
	SetOrigin(Origins::MC);
	sortLayer = 50;
}

void Cat::Reset()
{
	animation.Play("cat_c3");
	SetOrigin(Origins::MC);
	SetPosition({ 0, 0 });
	isHover = false;
	Makeboxs();
}

void Cat::Update(float dt)
{
	if (!isHover || !INPUT_MGR.GetMouseButton(sf::Mouse::Left))
	{
		animation.Update(dt);
	}

	//RectangleShape Position Update
	{
		//if (!isRotation)
		//{
		//	// Sprite의 로컬 변환 행렬
		//	for (int i = 0; i < boxNumber.x; i++)
		//	{
		//		for (int j = 0; j < boxNumber.y; j++)
		//		{
		//			int index = i * boxNumber.x + j;	
		//			sf::Transform spriteTransform = sprite.getTransform();
		//			sf::Vector2f boxGlobalPos = boxs[index].getPosition();
		//			sf::Vector2f localPos = spriteTransform.getInverse().transformPoint(boxGlobalPos);

		//			boxs[index].setPosition(localPos);

		//			std::cout << "sprite pos: " << sprite.getPosition().x << std::endl;
		//			std::cout << "local pos" << localPos.x<<localPos.y << std::endl;

		//			//boxs[index].setPosition(spriteBounds.left + i * boxSize.x, spriteBounds.top + j * boxSize.y);
		//			// box의 전역 위치
		//			//sf::Vector2f globalposition = boxs[index].getPosition();

		//			// box의 전역 위치를 sprite의 로컬 위치로 변환
		//			//sf::Vector2f localposition = spriteTransform.getInverse().transformPoint(globalposition);
		//			//boxs[index].setPosition(localposition);
		//		}
		//	}
		//}
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
	{
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
	}

	//Rotation
	{
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


			int x = boxSize.x;
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
					sf::Vector2f transformedOffset = transform.transformPoint(boxOffset);
					boxs[index].setPosition(sprite.getPosition() + transformedOffset);
				}
			}
		}

		
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
	//이팩트
	

}

void Cat::OnEnter()
{
}

void Cat::OnExit()
{
}

void Cat::OnClickHold(sf::Vector2f worldMousePos)
{
	sf::Texture* tex = RESOURCE_MGR.GetTexture("sprites/d3_0.png");
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

	std::cout << "boxSize(" << boxSize.x << ", " << boxSize.y << std::endl;

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
			box.setPosition(spriteBounds.left + i * boxSize.x, spriteBounds.top + j * boxSize.y);
			Utils::SetOrigin(box, Origins::MC);
			boxs.push_back(box);
		}
	}
}


