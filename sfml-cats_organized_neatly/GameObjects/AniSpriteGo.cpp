#include "stdafx.h"
#include "AniSpriteGo.h"
#include "ResourceMgr.h"

AniSpriteGo::AniSpriteGo(const std::string& animationId, const std::string& resourcePath, const std::string& n)
	: SpriteGo(resourcePath, n), animationId(animationId)
{
}

void AniSpriteGo::Init()
{
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip(resourcePath));
	animation.SetTarget(&sprite);
}

void AniSpriteGo::Reset()
{
	animation.Play(animationId);
	sf::Vector2i size = (sf::Vector2i)sprite.getTexture()->getSize();
	sprite.setTextureRect({ 0, 0, size.x, size.y });
	SetOrigin(origin);	//애니메이션은 Addclip을 해준 다음에 세팅해 줘야 하기 때문에 Reset에서 한 번 더 자기 자신이 가지고 있는 값으로 세팅
	SetPosition(position);
}

void AniSpriteGo::Update(float dt)
{
	animation.Update(dt);

}

void AniSpriteGo::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);

}
