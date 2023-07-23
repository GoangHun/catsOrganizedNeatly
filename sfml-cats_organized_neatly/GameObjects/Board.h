#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
#include "Tile.h"
#include "Cat.h"

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
	sf::RectangleShape shape;
	Tile* tile = nullptr;
	bool isHover = false;
	bool prevHover = false;
	bool isUse = false;
};

class Board :
    public SpriteGo
{
protected:
	BoardInfo boardInfo = { BoardType::_3X3, "board_3x3" };
	AnimationController animation;
	
	std::vector<Room> rooms;
	ObjectPool<Tile> tilePool;

	std::list<Cat*> cats;
	Cat* isCatchCat = nullptr;


public:
	Board(const std::string& textureId = "", const std::string& n = "Board")
		: SpriteGo(textureId, n) {}
	virtual ~Board() override { Release(); }

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void ClearRooms();

	void SetAllRoomIsUse();
	bool SetRoomIsUse(Cat* cat);

	void SetBoard(BoardType type);
	void MakeRooms(BoardType type);
	void SetBoardInfo(BoardType type, std::string aniId) { boardInfo = { type, aniId }; }
	void SetIsCatchCat(Cat* cat) { isCatchCat = cat; }

	const BoardInfo GetBoardInfo() const { return boardInfo; };
	std::vector<Room>* GetRooms() { return &rooms; }
	const Cat* GetIsCatchCat() { return isCatchCat; }
	ObjectPool<Tile>* GetTilePool() { return &tilePool; }

	void OnClick(Room& sRoom);
	void OnEnter(Room& sRoom);
	void OnExit(Room& sRoom);
};

