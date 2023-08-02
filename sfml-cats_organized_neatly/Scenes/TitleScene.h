#pragma once
#include "Scene.h"



class TitleScene :
    public Scene
{
protected:
	bool isSwipe = false;
	float distance = 0.f;
	float currentSpeed = 0.f;
	float remainingDistance = 0.f;
	float lefp = 1.f;

	sf::Vector2f startCenter = { 0,0 };
	sf::Vector2f dir = { 0, 0 };

public:
	int selectNum = 0;

	TitleScene();
	virtual ~TitleScene() override = default;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetInitValue(GameObject* go, Origins origin, sf::Vector2f pos, float angle = 0, int layer = 100);
	void SwipeAnimation(sf::Vector2f dir, float dis, float speed, float dt);

	bool GetIsSwipe() { return isSwipe; }
};

