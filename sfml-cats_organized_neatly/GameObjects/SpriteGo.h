#pragma once
#include "GameObject.h"
class SpriteGo : public GameObject
{
protected:

public:
	sf::Sprite sprite;
	std::string textureId;

	SpriteGo(const std::string& textureId, const std::string& n);
	virtual ~SpriteGo() override;

	virtual void SetPosition(const sf::Vector2f& p);
	virtual void SetPosition(float x, float y);

	virtual void SetOrigin(Origins origin);
	virtual void SetOrigin(float x, float y);

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;


};

