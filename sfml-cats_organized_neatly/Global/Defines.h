#pragma once
#include "AnimationClip.h"

#define _USE_MATH_DEFINES
#define _TEXT_COLOR { 71, 39, 58, 255 }

enum class CatTypes
{
	c1 = 1,
	c2,
	c3,
	c4,
	c5,
	c6,
	c7,
	c8,
	c9,
	c10,
	c11,
	c12,
	COUNT,
};

enum class Languages
{
	KOR,
	ENG,
	JP,
	COUNT,
};

enum class Origins
{
	TL,
	TC,
	TR,
	ML,
	MC,
	MR,
	BL,
	BC,
	BR,
	CUSTOM,
};

enum class ResourceTypes
{
	Texture,
	Font,
	SoundBuffer,
	AnimationClip,
};

enum class SceneId
{
	None = -1,
	Title,
	Game,
	Developer,
	Count,
};

