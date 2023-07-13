#include "stdafx.h"
#include "ShapeGo.h"

ShapeGo::ShapeGo(const std::string& n) : shape(shape)
{
}

ShapeGo::~ShapeGo()
{
}

void ShapeGo::SetPosition(const sf::Vector2f& p)
{
	position = p;
	shape->setPosition(p);
}

void ShapeGo::SetPosition(float x, float y)
{
	position = { x, y };
	shape->setPosition(x, y);
}

void ShapeGo::SetOrigin(Origins origin)
{
	this->origin = origin;
	Utils::SetOrigin(*shape, origin);
}

void ShapeGo::Init()
{
}

void ShapeGo::Release()
{
}

void ShapeGo::Reset()
{
}

void ShapeGo::Update(float dt)
{
}

void ShapeGo::Draw(sf::RenderWindow& window)
{
	window.draw(*shape);
}
