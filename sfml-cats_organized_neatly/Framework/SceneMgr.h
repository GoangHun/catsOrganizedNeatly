#pragma once
#include "Singleton.h"
#include "Scene.h"

class SceneMgr : public Singleton<SceneMgr>
{
	friend Singleton<SceneMgr>;

protected:
	SceneMgr() = default;
	virtual ~SceneMgr() override = default;

	std::vector<Scene*> scenes;
	SceneId currentSceneId = SceneId::None;
	Scene* currentScene = nullptr;

	SceneId startSceneId = SceneId::Game;

public:
	void Init();
	void Release();

	void UpdateEvent(float dt);
	void Draw(sf::RenderWindow& window);

	void ChangeScene(SceneId id);
	Scene* GetCurrScene() const;

};

#define SCENE_MGR (SceneMgr::Instance())
