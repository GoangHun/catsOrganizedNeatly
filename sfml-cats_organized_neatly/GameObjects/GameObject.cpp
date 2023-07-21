#include "stdafx.h"
#include "GameObject.h"

GameObject::GameObject(const std::string& textureId, const std::string& n)
	: name (n), resourcePath(textureId)
{
}

GameObject::~GameObject()
{
	Release();
}

bool GameObject::GetActive() const
{
	return isActive;
}

void GameObject::SetActive(bool active)
{
	isActive = active;
}

const std::string& GameObject::GetName() const
{
	return name;
}

void GameObject::SetName(const std::string& n)
{
	name = n;
}

const sf::Vector2f& GameObject::GetPosition() const
{
	return position;
}

void GameObject::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

void GameObject::SetPosition(const sf::Vector2f& p)
{
	position = p;
}

void GameObject::SetOrigin(Origins origin)
{
	this->origin = origin;
}

void GameObject::SetOrigin(float x, float y)
{
	this->origin = Origins::CUSTOM;
}


