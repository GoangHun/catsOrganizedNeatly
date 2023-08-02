#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
#include "ObjectPool.h"
#include "Tile.h";

class Board;
struct Room;

//box에 올라가 있는 room의 정보를 담아서 이중 for문을 줄일 수 있음(작업중)
struct Box
{
	sf::RectangleShape shape;
	bool isActive;
	Room* room = nullptr;
};


class Cat :
    public SpriteGo
{
protected:
	AnimationController animation;
	CatTypes type;

	std::vector<Box> boxs;

	sf::Vector2i boxNumber;	//number of box
	sf::Vector2f boxSize;	//lengths of box
	sf::Vector2f startPos;

	std::vector<Room>* rooms = nullptr;
	ObjectPool<Tile>* tilePool = nullptr;
	Board* board = nullptr;
	sf::Texture* tex = nullptr;

	std::string animationId;
	std::string boxInfo;

	bool isHover = false;
	bool isCatch = false;
	bool isSnap = false;
	bool isRotation = false;
	bool isFixation = false;

	float startAngle = 0.f;
	float targetRotation = 0.f;
	float rotationSpeed = 400.f;
	float rotationAngle = 0;

	int activeBoxNum = 0;

public:
	Cat(const CatTypes type, const std::string& textureId = "", const std::string& n = "Cat");
	virtual ~Cat() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void OnClickHold(sf::Vector2f worldMousePos);

	void Makeboxs();
	void SetBoxState();

	void ClearRooms();

	void SetPool(ObjectPool<Tile>* pool) { tilePool = pool; }
	void SetType(CatTypes type) { this->type = type; }
	void SetBoard(Board* board);
	void SetRooms(std::vector<Room>* rooms) { this->rooms = rooms; }
	void SetStartPos(sf::Vector2f pos) { startPos = pos; }

	int GetType() { return (int)type; }
	std::vector<Box>* GetBoxs() { return &boxs; }

};

