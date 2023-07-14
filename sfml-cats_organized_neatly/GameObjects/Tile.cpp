#include "stdafx.h"
#include "Tile.h"

Tile::Tile(const std::string& textureId, const std::string& n)
	: SpriteGo(textureId, n)
{
}

Tile::~Tile()
{
}

void Tile::Init()
{
	textureId = "sprites/board_tile_0.png";
	SetOrigin(Origins::MC);
	sortLayer = 2;
}

void Tile::Release()
{

}

void Tile::Reset()
{
	SpriteGo::Reset();
}
