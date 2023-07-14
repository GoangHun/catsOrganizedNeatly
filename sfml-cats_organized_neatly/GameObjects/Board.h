#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"

enum class BoardType
{
	_3X3 = 3,
	_4X4,
	_5X5,
	_6X6,
	_7X7,
	_8X8,
};


class Board :
    public SpriteGo
{
protected:
	BoardType type;
	AnimationController animation;
	std::vector<sf::RectangleShape> rooms;

	sf::Vector2f center;

public:
	Board(const std::string& textureId = "", const std::string& n = "")
		: SpriteGo(textureId, n) {}
	virtual ~Board() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetBoard(BoardType type);
	void SetRoomPos(BoardType type);
};

