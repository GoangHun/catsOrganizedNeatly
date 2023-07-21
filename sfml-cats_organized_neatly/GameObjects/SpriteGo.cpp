#include "stdafx.h"
#include "SpriteGo.h"
#include "Utils.h"
#include "ResourceMgr.h"

SpriteGo::SpriteGo(const std::string& textureId, const std::string& n)
	: GameObject(textureId, n)
{
}

SpriteGo::~SpriteGo()
{
}

void SpriteGo::SetPosition(const sf::Vector2f& p)
{
	position = p;
	sprite.setPosition(p);
}

void SpriteGo::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
	sprite.setPosition(position);
}

void SpriteGo::SetOrigin(Origins origin)
{
	GameObject::SetOrigin(origin);
	if (this->origin != Origins::CUSTOM)
	{
		Utils::SetOrigin(sprite, origin);
	}
}

void SpriteGo::SetOrigin(float x, float y)
{
	GameObject::SetOrigin(x, y);
	sprite.setOrigin(x, y);
}

void SpriteGo::SetRotation(float degree)
{
	rotation = degree;
	sprite.setRotation(degree);
}


void SpriteGo::Init()
{

}

void SpriteGo::Release()
{
}

void SpriteGo::Reset()
{
	sf::Texture* tex = RESOURCE_MGR.GetTexture(resourcePath);
	if (tex != nullptr)
	{
		sprite.setTexture(*tex);
	}
	SetOrigin(origin);
}

void SpriteGo::Update(float dt)
{
}

void SpriteGo::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}
