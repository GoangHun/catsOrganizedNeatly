#include "stdafx.h"
#include "Scene.h"
#include "GameObject.h"
#include "ResourceMgr.h"
#include "Framework.h"

Scene::Scene(SceneId id) : sceneId(id), window(FRAMEWORK.GetWindow())
{
}

Scene::~Scene()
{
}

GameObject* Scene::FindGo(const std::string& name)
{
	for (auto go : gameObjects)
	{
		if (go->GetName() == name)
		{
			return go;
		}
	}
	return nullptr;
}

void Scene::FindGos(std::list<GameObject*>& list, const std::string& name)
{
	//list.clear();
	for (auto go : gameObjects)
	{
		if (go->GetName() == name)
		{
			list.push_back(go);
		}
	}
}

bool Scene::Exist(GameObject* go)
{
	return std::find(gameObjects.begin(), gameObjects.end(), go) != gameObjects.end();
}

GameObject* Scene::AddGo(GameObject* go)
{
	if (!Exist(go))
	{
		gameObjects.push_back(go);
	}
	return go;
}

void Scene::RemoveGo(GameObject* go)
{
	removeGameObjects.push_back(go);
}

void Scene::SortGos()
{
	gameObjects.sort([](GameObject* lhs, GameObject* rhs) {
		if (lhs->sortLayer != rhs->sortLayer)
			return lhs->sortLayer < rhs->sortLayer;
		return lhs->sortOrder < rhs->sortOrder;
		});
}

sf::Vector2f Scene::ScreenToWorldPos(sf::Vector2f screenPos)
{
	return window.mapPixelToCoords((sf::Vector2i)screenPos, worldView);
}

sf::Vector2f Scene::ScreenToUiPos(sf::Vector2f screenPos)
{
	return window.mapPixelToCoords((sf::Vector2i)screenPos, uiView);
}

sf::Vector2f Scene::WorldPosToScreen(sf::Vector2f worldPos)
{
	return (sf::Vector2f)window.mapCoordsToPixel(worldPos, worldView);
}

sf::Vector2f Scene::UiPosPosToScreen(sf::Vector2f uiPos)
{
	return (sf::Vector2f)window.mapCoordsToPixel(uiPos, uiView);
}

void Scene::Enter()
{
	RESOURCE_MGR.LoadFromCSV(resourceListPath);

	for (auto go : gameObjects)
	{
		go->Reset();
	}
}

void Scene::Exit()
{
	for (auto go : removeGameObjects)
	{
		gameObjects.remove(go);
	}
	removeGameObjects.clear();

	RESOURCE_MGR.UnLoadAll();
}

void Scene::Update(float dt)
{
	for (auto go : gameObjects)
	{
		if (go->GetActive())
		{
			go->Update(dt);
		}
	}

	for (auto go : removeGameObjects)
	{
		gameObjects.remove(go);
	}
	removeGameObjects.clear();
}

void Scene::Draw(sf::RenderWindow& window)
{
	// layer 100 บฮลอดย ui Layer

	SortGos();

	window.setView(worldView);
	for (auto go : gameObjects)
	{
		if (go->sortLayer >= 100)
			continue;

		if (go->GetActive())
		{
			go->Draw(window);
		}
	}

	window.setView(uiView);
	for (auto go : gameObjects)
	{
		if (go->sortLayer < 100)
			continue;

		if (go->GetActive())
		{
			go->Draw(window);
		}
	}
}
