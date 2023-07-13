#pragma once
#include "SpriteGo.h"
class GameBackground :
    public SpriteGo
{
protected:
	std::vector<std::string> textureIds;

public:
	GameBackground(const std::string& textureId = "", const std::string& n = "");
	virtual ~GameBackground() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
};

