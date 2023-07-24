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
	SetOrigin(origin);	//�ִϸ��̼��� Addclip�� ���� ������ ������ ��� �ϱ� ������ Reset���� �� �� �� �ڱ� �ڽ��� ������ �ִ� ������ ����
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
