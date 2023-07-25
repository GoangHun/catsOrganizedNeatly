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
	SceneId startSceneId = SceneId::Title;

public:
	void Init();
	void Release();

	void UpdateEvent(float dt);
	void Draw(sf::RenderWindow& window);

	void ChangeScene(SceneId id, int stageNum = 1);
	Scene* GetCurrScene() const;
	const std::vector<Scene*>* GetScenes() const;

};

#define SCENE_MGR (SceneMgr::Instance())
