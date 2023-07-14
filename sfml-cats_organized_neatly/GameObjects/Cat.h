#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
enum class CatTypes
{
	KEETO,
};
struct RectInfo
{
	CatTypes type;
	std::vector<sf::Vector2i> coord;
};

class Cat :
    public SpriteGo
{
protected:
	AnimationController animation;
	std::vector<sf::IntRect> rooms;
	RectInfo rectInfo;

	bool isHover = false;
	bool isCatch = false;
	bool isRotation = false;

	float startAngle = 0.f;
	float targetRotation = 0.f;
	float rotationSpeed = 400.f;
	float rotationAngle = 0;

public:
	Cat(const std::string& textureId = "", const std::string& n = "")
		: SpriteGo(textureId, n) {}
	virtual ~Cat() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;

	void OnClick();
	void OnEnter();
	void OnExit();
	void OnClickHold(sf::Vector2f worldMousePos);
};

