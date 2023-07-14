#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"

class Room :
    public SpriteGo
{
protected:
	ObjectPool<Room>* pool = nullptr;

public:
	Room(const std::string& textureId = "", const std::string& n = "");
	virtual ~Room() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
};

