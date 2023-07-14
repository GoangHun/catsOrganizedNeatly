#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"

class Tile :
    public SpriteGo
{
protected:

public:
	Tile(const std::string& textureId = "", const std::string& n = "");
	virtual ~Tile() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

};

