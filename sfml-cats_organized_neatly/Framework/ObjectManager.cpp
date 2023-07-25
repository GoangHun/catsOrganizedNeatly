#include "stdafx.h"
#include "ObjectManager.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "Board.h"
#include "Tile.h"
#include "Cat.h"
#include "SpriteGo.h"
#include <fstream>


void ObjectManager::SaveObjects(std::string fPath, BoardType type,const std::list<GameObject*>& objects)
{
    std::ofstream file(fPath, std::ios::trunc);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open the file for saving.");
    }

    file << (int)type << "\n";

    for (const auto& object : objects) 
    {
        std::string name = object->GetName();
        if (name == "Tile" || name == "Pot")
        {
            file << object->GetName() << "," << object->GetResourcePath() << "," << object->GetPosition().x
                << "," << object->GetPosition().y << "," << object->GetRotation() << "\n";
        } 
        else if (name == "Cat")
        {
            file << object->GetName() << "," << dynamic_cast<Cat*>(object)->GetType() << "," << object->GetPosition().x
                << "," << object->GetPosition().y << "," << object->GetRotation() << "\n";
        }
    }
    file.close();
    std::cout << "Objects saved to " << fPath << std::endl;
}

std::tuple<int, std::vector<std::tuple<std::string, std::string, float, float, float>>> ObjectManager::LoadObjects(std::string fPath)
{
    std::ifstream file(fPath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open the file for loading.");
    }

    std::string line;
    std::getline(file, line);
    std::istringstream iss(line);
    
    std::string strType;
    std::getline(iss, strType, ',');
    int type = std::stoi(strType);

    std::vector<std::tuple<std::string, std::string, float, float, float>> info;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string name, texId, strPosX, strPosY, strRotation;

        if (!(std::getline(iss, name, ',') && std::getline(iss, texId, ',') &&
            std::getline(iss, strPosX, ',') && std::getline(iss, strPosY, ',') && std::getline(iss, strRotation, ','))) {
            throw std::runtime_error("Invalid data format in the CSV file.");
        }

        float posX = std::stof(strPosX);
        float posY = std::stof(strPosY);
        float rotation = std::stof(strRotation);

        info.push_back({ name, texId, posX, posY, rotation }); 
    }

    file.close();
    std::cout << "Objects loaded from " << fPath << std::endl; 

    return std::make_tuple(type, info);
}
