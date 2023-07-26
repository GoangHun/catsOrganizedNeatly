#pragma once
#include "Scene.h"

class Board;
class Cat;

class GameScene :
    public Scene
{
protected:
	bool isClick = false;
	bool isCatch = false;
	bool isRotation = false;
	bool isCatHover = false;
	bool isLoad = false;
	bool isComplete = false;	//현재 스테이지를 클리어 하면 다음 버튼 활성화

	float startAngle = 0.f;
	float targetRotation = 0.f;
	float rotationSpeed = 400.f;
	float rotationAngle = 0;

	int stageNum = 1;

	std::map<int, std::string> stageInfos;
	std::vector<Cat*> cats;

	Board* board = nullptr;

public:
	GameScene();
	virtual ~GameScene() override = default;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void LoadScene(int stageNum);

	bool GetIsCatch() { return isCatch; }
	const std::vector<Cat*>& GetCats() { return cats; }
};

