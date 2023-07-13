#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"

class Player : public SpriteGo
{
protected:
	AnimationController animation;
	sf::Vector2f velocity;

	float accel = 500.f;
	float speed = 500.f;
	float JumpForce = -500.f;
	float gravity = 0.f;

	bool filpX = false;
	bool isGround = true;

public:
	Player(const std::string& textureId = "", const std::string& n = "")
		: SpriteGo(textureId, n) {}
	virtual ~Player() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;

	bool GetFlipX() const;
	void SetFlipX(bool filp);

	void SetIsGround(bool isGround) { this->isGround = isGround; }

	AnimationController GetAnimation() { return animation; }
	
};

