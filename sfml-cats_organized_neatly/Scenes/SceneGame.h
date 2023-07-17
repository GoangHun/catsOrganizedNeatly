#pragma once
#include "Scene.h"
#include "Ground.h"
#include "SpriteGo.h"

class SceneGame : public Scene
{
protected:
	bool isDeveloperMode = true;
	bool isCatch = false;

	std::vector<SpriteGo> ornaments;
	sf::Sprite sprite;


public:
	SceneGame();
	virtual ~SceneGame() override = default;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void ToggleIsDeveloperMode() { isDeveloperMode = !isDeveloperMode; }
	bool GetIsDeveloperMode() { return isDeveloperMode; }
};

