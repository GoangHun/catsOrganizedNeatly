#include "stdafx.h"
#include "GameBackground.h"
#include "ResourceMgr.h"

GameBackground::GameBackground(const std::string& textureId, const std::string& n)
	: SpriteGo(textureId, n)
{
}

GameBackground::~GameBackground()
{
}

void GameBackground::Init()
{
	textureId = "sprites/Background_Ingame_0.png";
	SetOrigin(Origins::MC);
	SetPosition({0, 0});
	sortLayer = -1;
}

void GameBackground::Release()
{
}

void GameBackground::Reset()
{
	SpriteGo::Reset();
	SetPosition({ 0, 0 });
	sortLayer = -1;
}
