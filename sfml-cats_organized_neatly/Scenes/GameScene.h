#pragma once
#include "Scene.h"
class GameScene :
    public Scene
{
protected:
	bool isCatch = false;
	bool isRotation = false;
	bool isCatHover = false;
	bool isLoad = false;
	bool isClear = false;	//현재 스테이지를 클리어 하면 다음 버튼 활성화

	float startAngle = 0.f;
	float targetRotation = 0.f;
	float rotationSpeed = 400.f;
	float rotationAngle = 0;

	int stageNum = 0;

	std::map<int, std::string> stageInfos;

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
};

