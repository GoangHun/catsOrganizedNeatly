#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"

class Player1 : public SpriteGo
{
public:

protected:
	AnimationController animation;
	sf::Vector2f velocity;

	float accel = 500.f;
	float speed = 500.f;
	float JumpForce = -250.f;
	float gravity = 500.f;

	bool filpX = false;
	bool isGround = true;

public:
	Player1(const std::string& textureId = "", const std::string& n = "")
		: SpriteGo(textureId, n) {}
	virtual ~Player1() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;

	bool GetFlipX() const;
	void SetFlipX(bool filp);
	
};

