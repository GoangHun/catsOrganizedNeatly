#pragma once
#include "Scene.h"
class TitleScene :
    public Scene
{
protected:
public:
	TitleScene();
	virtual ~TitleScene() override = default;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};

