#pragma once
#include "GameObject.h"
class ShapeGo :
    public GameObject
{
protected:
	sf::Shape* shape;

public:
	ShapeGo(const std::string& n = "");
	virtual ~ShapeGo();

	virtual void SetPosition(const sf::Vector2f& p) override;
	virtual void SetPosition(float x, float y) override;

	virtual void SetOrigin(Origins origin) override;

	//virtual std::string GetId() override { return "shape"; }

	virtual void Init() override;
	virtual void Release() override;

	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};

