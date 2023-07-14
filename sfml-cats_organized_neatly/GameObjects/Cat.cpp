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
}

void Cat::Update(float dt)
{
	if (!isHover || !INPUT_MGR.GetMouseButton(sf::Mouse::Left))
	{
		animation.Update(dt);
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

	//»∏¿¸
	{
		if (isHover && INPUT_MGR.GetMouseButtonUp(sf::Mouse::Right))
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
		}
	}
	
	
	

}

void Cat::OnClick()
{
	//¿Ã∆—∆Æ
	

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


