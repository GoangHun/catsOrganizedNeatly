#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
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

struct Room
{
	sf::RectangleShape room;
	Tile* tile = nullptr;
	bool isHover = false;
	bool prevHover = false;
	bool isFull = false;
};


class Board :
    public SpriteGo
{
protected:
	BoardInfo boardInfo = { BoardType::_8X8, "board_8x8" };
	AnimationController animation;
	std::vector<Room> rooms;
	ObjectPool<Tile> tilePool;

	bool isDeveloperMode = true;

public:
	Board(const std::string& textureId = "", const std::string& n = "")
		: SpriteGo(textureId, n) {}
	virtual ~Board() override { Release(); }

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetBoard(BoardType type);
	void SetRoomPos(BoardType type);

	void OnClick(Room& sRoom);
	void OnEnter(Room& sRoom);
	void OnExit(Room& sRoom);
};

