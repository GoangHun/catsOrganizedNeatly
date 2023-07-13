#include "stdafx.h"
#include "Board.h"
#include "ResourceMgr.h"

void Board::Init()
{
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/board_3x3.csv"));
	animation.SetTarget(&sprite);
	SetOrigin(Origins::BC);
}

void Board::Reset()
{
	animation.Play("board_3x3");
	SetOrigin(Origins::MC);
	SetPosition({ 0, 0 });
}

void Board::Update(float dt)
{
	animation.Update(dt);
}
