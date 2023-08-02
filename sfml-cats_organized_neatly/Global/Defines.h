#pragma once
#include "AnimationClip.h"

#define _USE_MATH_DEFINES
#define _TEXT_COLOR { 71, 39, 58, 255 }
#define _UP {0.0f, -1.0f}
#define _DOWN {0.0f, 1.0f}
#define _RIGHT {1.0f, 0.0f}
#define _LEFT {-1.0f, 0.0f}

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

