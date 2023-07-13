#pragma once

enum class AnimationLoopTypes
{
	Single,
	Loop,
};

struct AnimationFrame
{
	std::string textureId;	
	sf::IntRect texCoord;	//texture 좌표
	std::function<void()> action;	//함수 포인터 대신 std::function사용
};


struct AnimationClip
{
	std::string id;
	std::vector<AnimationFrame> frames;
	AnimationLoopTypes loopType;
	int fps;

	bool LoadFromFile(const std::string path);
};