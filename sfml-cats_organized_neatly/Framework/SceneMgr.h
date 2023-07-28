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

	sf::Music titleBgm;
	sf::Music gameBgm;
public:
	void Init();
	void Release();

	void UpdateEvent(float dt);
	void Draw(sf::RenderWindow& window);

	void ChangeScene(SceneId id, int stageNum = 1);
	Scene* GetCurrScene() const;
	const std::vector<Scene*>* GetScenes() const;
	const SceneId& GetCurrentSceneId() { return currentSceneId; }

};

#define SCENE_MGR (SceneMgr::Instance())
