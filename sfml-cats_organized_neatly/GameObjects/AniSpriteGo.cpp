#include "stdafx.h"
#include "AniSpriteGo.h"

AniSpriteGo::AniSpriteGo(const std::string& textureId, const std::string& n)
	: SpriteGo(textureId, n)
{
}

void AniSpriteGo::Init()
{
	animation.SetTarget(&sprite);
}

void AniSpriteGo::Reset()
{
	animation.Play(animationId);
	sf::Vector2i size = (sf::Vector2i)sprite.getTexture()->getSize();
	sprite.setTextureRect({ 0, 0, size.x, size.y });
	SetOrigin(Origins::MC);
	SetPosition({ 0, 0 });
}

void AniSpriteGo::Update(float dt)
{
	animation.Update(dt);

}

void AniSpriteGo::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);

}
