#include "stdafx.h"
#include "Ground.h"
#include "SceneMgr.h"
#include "Framework.h"

Ground::Ground(const std::string& n) : ShapeGo(n)
{
	sf::Vector2f size(2000.f, 50.f);
	shape = new sf::RectangleShape(size);
}

Ground::~Ground()
{
}

void Ground::Init()
{
	sf::Vector2f size = FRAMEWORK.GetWindowSize();	
	shape->setFillColor(sf::Color::White);
	SetOrigin(Origins::MC);
	SetPosition(0, 300);
}

void Ground::Release()
{
}

void Ground::Reset()
{
	shape->setFillColor(sf::Color::White);
	SetOrigin(Origins::MC);
	SetPosition(0, 300);
}

void Ground::Update(float dt)
{
	if (shape->getGlobalBounds().intersects(player->GetAnimation().GetCurrentRect()))
	{
		player->SetPosition(player->GetPosition().x, GetPosition().y);
	}
}
