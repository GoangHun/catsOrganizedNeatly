#pragma once
#include "Utils.h"

class GameObject
{
protected:
	sf::Vector2f position;
	std::string name;
	bool isActive = true;
	Origins origin = Origins::TL;

public:

	int sortLayer = 0;
	int sortOrder = 0;

	GameObject(const std::string& n = "");
	virtual ~GameObject();

	bool GetActive() const;
	void SetActive(bool active);

	const std::string& GetName();
	virtual void SetName(const std::string& n);

	const sf::Vector2f& GetPosition();

	virtual void SetPosition(const sf::Vector2f& p);
	virtual void SetPosition(float x, float y);

	virtual void SetOrigin(Origins origin);
	virtual void SetOrigin(float x, float y);

	virtual void Init() = 0;	
	virtual void Release() {};

	virtual void Reset() = 0;	// √ ±‚»≠

	virtual void Update(float dt) = 0;
	virtual void Draw(sf::RenderWindow& window) = 0;
};

