#pragma once
#include "GameObject.h"
class TextGo :
    public GameObject
{
	
public:
	sf::Text text;
	TextGo(const std::string& resourceId = "", const std::string& n = "");
	virtual ~TextGo() override;

	virtual void SetPosition(const sf::Vector2f& p);
	virtual void SetPosition(float x, float y);

	virtual void SetOrigin(Origins origin);
	virtual void SetOrigin(float x, float y);
	virtual void SetSize(float w, float h);

	virtual void Init() override; //���� �Ҵ�(������)
	virtual void Release() override; //���� �Ҵ� ����(�Ҹ���)
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetTextString(std::string str);
	void SetText(std::string str, const int size, sf::Color color, 
		Origins origin, int soltLayer, float x, float y);

};
