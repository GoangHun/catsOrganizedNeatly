#pragma once
#include "stdafx.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "Singleton.h"
#include "GameObject.h"

enum class BoardType;

class ObjectManager : public Singleton<ObjectManager>
{
    friend Singleton<ObjectManager>;
private:
    ObjectManager() = default;
    virtual ~ObjectManager() override = default;
public:
    void SaveObjects(std::string fPath, BoardType type, const std::list<GameObject*>& objects);
    std::tuple<int, std::vector<GameObject*>> LoadObjects(std::string fPath);
};

#define OBJECT_MGR (ObjectManager::Instance())

