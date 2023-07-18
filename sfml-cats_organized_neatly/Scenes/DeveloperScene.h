#pragma once
#include "Scene.h"
#include "Ground.h"
#include "SpriteGo.h"

class DeveloperScene : public Scene
{
protected:
	bool isDeveloperMode = true;
	bool isCatch = false;
	bool isRotation = false;
	bool isCatHover = false;

	float startAngle = 0.f;
	float targetRotation = 0.f;
	float rotationSpeed = 400.f;
	float rotationAngle = 0;

	int num = 2;
	int catType = 1;

	std::vector<SpriteGo> ornaments;
	SpriteGo* pSpriteGo = nullptr;
	sf::Texture tex;

	


public:
	DeveloperScene();
	virtual ~DeveloperScene() override = default;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void ToggleIsDeveloperMode() { isDeveloperMode = !isDeveloperMode; }
	
	bool GetIsDeveloperMode() { return isDeveloperMode; }
	bool GetIsCatch() { return isCatch; }
};
