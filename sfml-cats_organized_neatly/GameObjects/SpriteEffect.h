#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"

class SpriteEffect : public SpriteGo
{
protected:
	float duration = 0.f;
	float timer = 0.f;

	ObjectPool<SpriteEffect>* pool = nullptr;

public:
	SpriteEffect(const std::string& textureId = "", const std::string& n = "");
	virtual ~SpriteEffect() override { };

	void SetDuration(float duration) { this->duration = duration; }
	void SetPool(ObjectPool<SpriteEffect>* pool) { this->pool = pool; }

	virtual void Init() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
};

