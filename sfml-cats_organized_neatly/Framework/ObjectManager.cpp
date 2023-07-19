#include "stdafx.h"
#include "ObjectManager.h"
#include "ResourceMgr.h"
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
        if (name == "Tile" || name == "Cat" || name == "Pot")
        {
            file << object->GetName() << "," << object->GetTexId() << "," << object->GetPosition().x
                << "," << object->GetPosition().y << "," << object->GetRotation() << "\n";
        } 
    }
    file.close();
    std::cout << "Objects saved to " << fPath << std::endl;
}

std::tuple<int, std::vector<GameObject*>> ObjectManager::LoadObjects(std::string fPath)
{
    std::vector<GameObject*> objects;

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

        if (name == "Tile")
        {
            Tile* tile = new Tile();
            tile->Init();
            tile->Reset();
            tile->SetTexId(texId);
            tile->SetPosition(posX, posY);
            tile->SetRotation(0.f);
            objects.push_back(tile);
        }
        else if (name == "Cat")
        {
            Cat* cat = new Cat((CatTypes)type);
            cat->Init();
            cat->Reset();
            cat->SetTexId(texId);
            cat->SetOrigin(Origins::MC);
            cat->SetPosition(posX, posY);
            cat->SetRotation(rotation);
            objects.push_back(cat);
        }
        else if (name == "Pot")
        {
            SpriteGo* pot = new SpriteGo();
            pot->Init();
            pot->Reset();
            pot->SetTexId(texId);
            pot->sprite.setTexture(*RESOURCE_MGR.GetTexture(texId));
            pot->SetOrigin(Origins::MC);
            pot->SetPosition(posX, posY);
            pot->SetRotation(0.f);
            objects.push_back(pot);
        }   
    }

    file.close();
    std::cout << "Objects loaded from " << fPath << std::endl; 

    return std::make_tuple(type, objects);
}
