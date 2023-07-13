#include "stdafx.h"
#include "UIButton.h"
#include "InputMgr.h"
#include "SceneMgr.h"

UIButton::UIButton(const std::string& textureId, const std::string& n) :
	SpriteGo(textureId, n)
{

}

UIButton::~UIButton()
{
	Release();
}

void UIButton::Init()
{
	SpriteGo::Init();
}

void UIButton::Release()
{
	SpriteGo::Release();
}

void UIButton::Reset()
{
	SpriteGo::Reset();
	isHover = false;
}

void UIButton::Update(float dt)
{
	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
	sf::Vector2f uiMousePos = SCENE_MGR.GetCurrScene()->ScreenToUiPos(mousePos);

	bool prevHover = isHover;
	isHover = sprite.getGlobalBounds().contains(uiMousePos);

	if (!prevHover && isHover)
	{
		if (OnEnter != nullptr)
			OnEnter();
		
	}

	if (prevHover && !isHover)
	{
		if (OnExit != nullptr)
			OnExit();

	}

	if (isHover && INPUT_MGR.GetMouseButtonUp(sf::Mouse::Left))
	{
		if (OnClick != nullptr)
			OnClick();

	}
}

void UIButton::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}
