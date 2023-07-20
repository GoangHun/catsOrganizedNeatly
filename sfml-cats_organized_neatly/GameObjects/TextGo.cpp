#include "stdafx.h"
#include "TextGo.h"
#include "ResourceMgr.h"
#include <sstream>

TextGo::TextGo(const std::string& rId, const std::string& n)
	:GameObject(rId, n)
{
}

TextGo::~TextGo()
{
}

void TextGo::SetPosition(const sf::Vector2f& p)
{
	position = p;
	text.setPosition(p);
}

void TextGo::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
	text.setPosition(position);
}

void TextGo::SetOrigin(Origins origin)
{
	Utils::SetOrigin(text, origin);
}

void TextGo::SetOrigin(float x, float y)
{
	text.setOrigin(x, y);
}

void TextGo::SetSize(float w, float h)
{
	text.setScale(w, h);
}

void TextGo::Init()
{
}

void TextGo::Release()
{
}

void TextGo::Reset()
{
	auto font = RESOURCE_MGR.GetFont(textureId);	//fontId
	if (font != nullptr)
	{
		text.setFont(*font);
	}
}

void TextGo::Update(float dt)
{
}

void TextGo::Draw(sf::RenderWindow& window)
{
	window.draw(text);
}

void TextGo::SetTextString(std::string str)
{
	text.setString(str);
}

void TextGo::SetText(std::string str, const int size, sf::Color color,
	Origins origin, int soltLayer, float x, float y)
{
	Utils::SetOrigin(text, origin);
	text.setCharacterSize(size);
	text.setFillColor(color);
	text.setString(str);
	SetPosition(x, y);

	this->sortLayer = soltLayer;
}
