#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
//#include "ObjectPool.h"
#include "Tile.h"

enum class BoardType
{
	_3X3 = 3,
	_4X4,
	_5X5,
	_6X6,
	_7X7,
	_8X8,
};

struct BoardInfo
{
	BoardType type;
	std::string animationId;
};


class Board :
    public SpriteGo
{
protected:
	BoardInfo boardInfo = { BoardType::_3X3, "board_3x3" };
	AnimationController animation;
	std::vector<sf::RectangleShape> rooms;
	ObjectPool<Tile> tilePool;

	bool isDeveloperMode = true;
	bool isHover = false;

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

	void OnClick();
	void OnEnter();
	void OnExit();
};

