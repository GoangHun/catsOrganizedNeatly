#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"

class Board :
    public SpriteGo
{
protected:
	AnimationController animation;
	std::vector<sf::IntRect> rooms;

public:
	Board(const std::string& textureId = "", const std::string& n = "")
		: SpriteGo(textureId, n) {}
	virtual ~Board() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
};

