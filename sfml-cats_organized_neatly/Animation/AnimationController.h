#pragma once

class AnimationController
{
protected:
	std::unordered_map<std::string, AnimationClip> clips;
	std::queue<std::string> queue;

	float speed = 1.f;
	bool isPlaying = false;

	AnimationClip* currentClip = nullptr;
	int currentFrame = -1;
	int totalFrame = 0;
	float clipDuration = 0.f;	//몇 프레임에 sprite를 교체할 건지
	float accumTime = 0.f;

	sf::Sprite* target = nullptr;

public:
	void AddClip(const AnimationClip& newClip);
	void SetTarget(sf::Sprite* sprite) { target = sprite; }
	sf::Sprite* GetTarget() const { return target; }
	bool IsPlaying() { return isPlaying; }
	void SetSpeed(float s) { speed = s; }
	float Speed() const { return speed; }

	void Update(float dt);

	void Play(const std::string& clipid, bool clearQueue = true);
	void PlayQueue(const std::string& clipid);
	void Stop();

	void SetFrame(const AnimationFrame& frame);
	 sf::FloatRect GetCurrentRect();

	 const std::string& GetCurrentClipId() { return currentClip->id; }
};

