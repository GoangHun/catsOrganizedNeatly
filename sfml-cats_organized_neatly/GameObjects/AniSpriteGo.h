#pragma once
#include "SpriteGo.h"

#include "AnimationController.h"

class AniSpriteGo :
    public SpriteGo
{
protected:
	AnimationController animation;

	sf::Texture* tex = nullptr;	//고정 이미지에 사용

	std::string animationId;

public:
	AniSpriteGo(const std::string& animationId, const std::string& resourcePath = "", const std::string& n = "");
	virtual ~AniSpriteGo() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	AnimationController& GetAnimation() { return animation; }
};

