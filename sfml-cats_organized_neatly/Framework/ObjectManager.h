#pragma once
#include "Singleton.h"

enum class BoardType;

class ObjectManager : public Singleton<ObjectManager>
{
    friend Singleton<ObjectManager>;
private:
    ObjectManager() = default;
    virtual ~ObjectManager() override = default;
public:
    void SaveObjects(std::string fPath, BoardType type, const std::list<GameObject*>& objects);
    std::tuple<int, std::vector<std::tuple<std::string, std::string, float, float, float>>> LoadObjects(std::string fPath);
};

#define OBJECT_MGR (ObjectManager::Instance())

