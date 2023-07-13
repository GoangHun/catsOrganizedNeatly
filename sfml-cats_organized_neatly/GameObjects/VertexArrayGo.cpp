#include "stdafx.h"
#include "VertexArrayGo.h"
#include "ResourceMgr.h"

VertexArrayGo::VertexArrayGo(const std::string& textureId, const std::string& n)
	: GameObject(n), textureId(textureId), texture(nullptr)
{
}

VertexArrayGo::~VertexArrayGo()
{
}

void VertexArrayGo::SetPosition(const sf::Vector2f& p)
{
	sf::Vector2f diff = p - position;
	position = p;

	for (int i = 0; i < vertexArray.getVertexCount(); ++i)
	{
		vertexArray[i].position += diff;
	}
}

void VertexArrayGo::SetPosition(float x, float y)
{
	SetPosition({ x, y });
}

void VertexArrayGo::SetOrigin(Origins origin)
{
	GameObject::SetOrigin(origin);

	if (origin == Origins::CUSTOM)
		return;

	sf::Vector2f prevOrigin = originPosition;
	sf::FloatRect bounds = vertexArray.getBounds();
	originPosition.x = bounds.width * ((int)origin % 3) * 0.5f;
	originPosition.y = bounds.height * ((int)origin / 3) * 0.5f;

	sf::Vector2f diff = prevOrigin - originPosition;
	for (int i = 0; i < vertexArray.getVertexCount(); ++i)
	{
		vertexArray[i].position += diff;
	}
}

void VertexArrayGo::SetOrigin(float x, float y)
{
	GameObject::SetOrigin(x, y);

	sf::Vector2f prevOrigin = originPosition;
	originPosition.x = x;
	originPosition.y = y;

	sf::Vector2f delta = prevOrigin - originPosition;
	for (int i = 0; i < vertexArray.getVertexCount(); ++i)
	{
		vertexArray[i].position += delta;
	}
}

void VertexArrayGo::Init()
{

}

void VertexArrayGo::Release()
{
}

void VertexArrayGo::Reset()
{
	texture = RESOURCE_MGR.GetTexture(textureId);
}

void VertexArrayGo::Update(float dt)
{
}

void VertexArrayGo::Draw(sf::RenderWindow& window)
{
	window.draw(vertexArray, texture);
}
