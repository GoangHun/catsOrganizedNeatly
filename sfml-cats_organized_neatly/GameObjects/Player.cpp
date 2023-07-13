#include "stdafx.h"
#include "Player.h"
#include "InputMgr.h"
#include "Framework.h"
#include "ResourceMgr.h"


void Player::Init()
{
	RESOURCE_MGR.Load(ResourceTypes::AnimationClip, "animations/Fox_IdleLR.csv");
	RESOURCE_MGR.Load(ResourceTypes::AnimationClip, "animations/Fox_IdleUp.csv");
	RESOURCE_MGR.Load(ResourceTypes::AnimationClip, "animations/Fox_IdleDown.csv");

	RESOURCE_MGR.Load(ResourceTypes::AnimationClip, "animations/Fox_MoveLR.csv");
	RESOURCE_MGR.Load(ResourceTypes::AnimationClip, "animations/Fox_MoveUp.csv");
	RESOURCE_MGR.Load(ResourceTypes::AnimationClip, "animations/Fox_MoveDown.csv");

	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Fox_IdleLR.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Fox_IdleUp.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Fox_IdleDown.csv"));

	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Fox_MoveLR.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Fox_MoveUp.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Fox_MoveDown.csv"));

	// Idle
	//{
	//	AnimationClip clip;
	//	clip.id = "IdleLR";
	//	clip.fps = 10;
	//	clip.loopType = AnimationLoopTypes::Loop;

	//	sf::IntRect coord(0, 768, 256, 256);
	//	for (int i = 0; i < 2; ++i)
	//	{
	//		clip.frames.push_back({ textureId, coord });
	//		coord.top += coord.height;
	//	}
	//	animation.AddClip(clip);
	//}

	//{
	//	AnimationClip clip;
	//	clip.id = "IdleDown";
	//	clip.fps = 10;
	//	clip.loopType = AnimationLoopTypes::Loop;

	//	sf::IntRect coord(256, 768, 256, 256);
	//	for (int i = 0; i < 2; ++i)
	//	{
	//		clip.frames.push_back({ textureId, coord });
	//		coord.top += coord.height;
	//	}
	//	animation.AddClip(clip);
	//}

	//{
	//	AnimationClip clip;
	//	clip.id = "IdleUp";
	//	clip.fps = 10;
	//	clip.loopType = AnimationLoopTypes::Loop;

	//	sf::IntRect coord(512, 768, 256, 256);
	//	for (int i = 0; i < 2; ++i)
	//	{
	//		clip.frames.push_back({ textureId, coord });
	//		coord.top += coord.height;
	//	}
	//	animation.AddClip(clip);
	//}

	//// Move
	//{
	//	AnimationClip clip;
	//	clip.id = "MoveLR";
	//	clip.fps = 10;
	//	clip.loopType = AnimationLoopTypes::Loop;

	//	sf::IntRect coord(0, 0, 256, 256);
	//	for (int i = 0; i < 4; ++i)
	//	{
	//		clip.frames.push_back({ textureId, coord });
	//		coord.left += coord.width;
	//	}
	//	animation.AddClip(clip);
	//}

	//{
	//	AnimationClip clip;
	//	clip.id = "MoveUp";
	//	clip.fps = 10;
	//	clip.loopType = AnimationLoopTypes::Loop;

	//	sf::IntRect coord(0, 256, 256, 256);
	//	for (int i = 0; i < 4; ++i)
	//	{
	//		clip.frames.push_back({ textureId, coord });
	//		coord.left += coord.width;
	//	}
	//	animation.AddClip(clip);
	//}

	//{
	//	AnimationClip clip;
	//	clip.id = "MoveDown";
	//	clip.fps = 10;
	//	clip.loopType = AnimationLoopTypes::Loop;

	//	sf::IntRect coord(0, 512, 256, 256);
	//	for (int i = 0; i < 4; ++i)
	//	{
	//		clip.frames.push_back({ textureId, coord });
	//		coord.left += coord.width;
	//	}
	//	animation.AddClip(clip);
	//}

	// Jump
	//{
	//	AnimationClip clip;
	//	clip.id = "Jump";
	//	clip.fps = 10;
	//	clip.loopType = AnimationLoopTypes::Single;

	//	sf::IntRect coord(0, 360, 120, 120);
	//	for (int i = 0; i < 7; ++i)
	//	{
	//		clip.frames.push_back({ textureId, coord });
	//		coord.left += coord.width;
	//	}

	//	//clip.frames[6].action = []() {
	//	//	std::cout << "On Complete Jump Clip" << std::endl;
	//	//};

	//	animation.AddClip(clip);
	//}

	animation.SetTarget(&sprite);	//SpriteGo::Reset에서 spriteId로 찾은 리소스를 setTexture 해줌

	SetOrigin(Origins::BC);
}

void Player::Reset()
{
	animation.Play("IdleDown");
	SetOrigin(origin);
	SetPosition({ 0, 0 });
	SetFlipX(false);
}

void Player::Update(float dt)
{
	animation.Update(dt);
	float h = INPUT_MGR.GetAxis(Axis::Horizontal);
	float v = INPUT_MGR.GetAxis(Axis::Vertical);

	// 플립
	if (h != 0.f)
	{
		bool flip = h < 0.f;
		if (GetFlipX() != flip)
		{
			SetFlipX(flip);
		}
	}

	// 점프
	/*if (isGround && INPUT_MGR.GetKeyDown(sf::Keyboard::Space))
	{
		velocity.y += JumpForce;
		animation.Play("Jump");
		isGround = false;
	}*/

	// 이동
	velocity.x = h * speed;
	velocity.y = v * speed;	// * gravity
	position += velocity * dt;

	// 바닥 충돌 처리
	/*if (position.y > 0.f)
	{
		isGround = true;
		position.y = 0.f;
		velocity.y = 0.f;
	}*/

	SetPosition(position);


	std::string currentClipId = animation.GetCurrentClipId();	
	// 에니메이션
	{
		if (h != 0.f && currentClipId != "MoveLR")
		{
			animation.Play("MoveLR");
		}
		else if (v > 0.f && h == 0.f && currentClipId != "MoveDown")
		{
			animation.Play("MoveDown");
		}
		else if (v < 0.f && h == 0.f && currentClipId != "MoveUp")
		{
			animation.Play("MoveUp");
		}
	}
	if (currentClipId.compare(0, 4, "Move", 4) == 0)
	{
		char ch = currentClipId[currentClipId.find_first_not_of("Move")];
		if (h == 0.f && v == 0.f)
		{
			if (ch == 'L')
			{
				animation.Play("IdleLR");
			}
			else if (ch == 'D')
			{
				animation.Play("IdleDown");
			}
			else if (ch == 'U')
			{
				animation.Play("IdleUp");
			}
		}	
	}
	/*else if (animation.GetCurrentClipId() == "Jump")
	{
		if (isGround)
		{
			animation.Play((h == 0.f) ? "Idle" : "Move");
		}
	}*/
}

bool Player::GetFlipX() const
{
	return filpX;
}

void Player::SetFlipX(bool filp)
{
	filpX = filp;

	sf::Vector2f scale = sprite.getScale();
	scale.x = !filpX ? -abs(scale.x) : abs(scale.x);
	sprite.setScale(scale);
}