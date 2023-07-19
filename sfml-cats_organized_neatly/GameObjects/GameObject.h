#pragma once
#include "Utils.h"

class GameObject
{
protected:
	sf::Vector2f position;
	float rotation;
	std::string name;
	std::string textureId;
	bool isActive = true;
	Origins origin = Origins::TL;

public:

	int sortLayer = 0;
	int sortOrder = 0;

	GameObject(const std::string& textureId = "", const std::string & n = "");
	virtual ~GameObject();

	bool GetActive() const;
	void SetActive(bool active);

	const std::string& GetName() const;
	virtual void SetName(const std::string& n);

	const sf::Vector2f& GetPosition() const;

	virtual void SetPosition(const sf::Vector2f& p);
	virtual void SetPosition(float x, float y);

	virtual void SetOrigin(Origins origin);
	virtual void SetOrigin(float x, float y);

	virtual void SetRotation(float degree) { rotation = degree; }
	virtual float GetRotation() const { return rotation; }

	const virtual std::string GetTexId() const { return textureId; }
	virtual void SetTexId(std::string id) { textureId = id; }

	virtual void Init() = 0;	
	virtual void Release() {};

	virtual void Reset() = 0;	// √ ±‚»≠

	virtual void Update(float dt) = 0;
	virtual void Draw(sf::RenderWindow& window) = 0;
};

