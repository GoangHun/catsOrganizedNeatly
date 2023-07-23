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
            Board* board = (Board*)SCENE_MGR.GetCurrScene()->FindGo("Board");
            Tile* tile = board->GetTilePool()->Get();
            tile->Init();
            tile->Reset();
            tile->SetResourcePath(texId);
            tile->SetPosition(posX, posY);
            tile->SetRotation(0.f);
            objects.push_back(tile);
        }
        else if (name == "Cat")
        {
            Cat* cat = new Cat((CatTypes)stoi(texId));    //texId가 아니라 CatTypes가 저장되 있음
            cat->Init();
            cat->Reset();
            cat->SetOrigin(Origins::MC);
            cat->SetPosition(posX, posY);
            cat->SetStartPos({ posX, posY });
            //cat->SetRotation(rotation);   //box를 만든 뒤에 돌리거나 돌린 다음에 박스 정보도 배열로 만들어서 회전 시킨 값을 적용시킬 필요 있음
            objects.push_back(cat);
        }
        else if (name == "Pot")
        {
            SpriteGo* pot = new SpriteGo();
            pot->Init();
            pot->Reset();
            pot->SetResourcePath(texId);
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
