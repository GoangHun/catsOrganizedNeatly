#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
#include "ObjectPool.h"
#include "Tile.h";

//struct BoxCollider
//{
//	int num1;
//	int num2;
//	int num3;
//	int num4;
//	int num5;
//	int num6;
//};

enum class CatTypes
{
	c1 = 1,
	c2,
	c3,
	c4,
	c5,
	c6,
	c7,
	c8,
	c9,
	c10,
	c11,
	c12,
};


class Cat :
    public SpriteGo
{
protected:
	AnimationController animation;
	CatTypes type;

	//boxs
	std::vector<sf::RectangleShape> boxs;
	sf::Vector2i boxNumber;	//number of box
	sf::Vector2f boxSize;	//lengths of box
	std::vector<int> activeBoxInfo;

	ObjectPool<Tile>* tilePool = nullptr;
	
	bool isHover = false;
	bool isCatch = false;
	bool isRotation = false;
	bool isFixation = false;

	float startAngle = 0.f;
	float targetRotation = 0.f;
	float rotationSpeed = 400.f;
	float rotationAngle = 0;

public:
	Cat(const CatTypes type, const std::string& textureId = "", const std::string& n = "")
		: SpriteGo(textureId, n) { this->type = type; }
	virtual ~Cat() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void OnClick();
	void OnEnter();
	void OnExit();
	void OnClickHold(sf::Vector2f worldMousePos);

	void Makeboxs();

	void SetPool(ObjectPool<Tile>* pool) { tilePool = pool; }
	void SetActiveBox();
};

