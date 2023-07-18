#pragma once
#include "SpriteGo.h"
class UIButton :
    public SpriteGo
{
protected:
	bool isHover = false;

public:
	UIButton(const std::string& textureId = "", const std::string& n = "");
	virtual ~UIButton() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	sf::Text text;

	void SetText(std::string fontPath, int size, sf::Color color, std::string str, Origins origin, sf::Vector2f pos);

	std::function<void()> OnClick;
	std::function<void()> OnEnter;
	std::function<void()> OnExit;
};


