#pragma once

class Utils
{
private:
	static std::random_device rd;
	static std::mt19937 gen;

public:
	static int RandomRange(int min, int maxExclude);
	static float RandomRange(float min, float max);
	static sf::Vector2f RandomOnCircle(float radius);
	static sf::Vector2f RandomInCircle(float radius);
	static float RandomValue();

	static void SetOrigin(sf::Sprite& sprite, Origins origin);
	static void SetOrigin(sf::Text& text, Origins origin);
	static void SetOrigin(sf::Shape& shape, Origins origin);
	static void SetOrigin(sf::Transformable& obj, Origins origin, const sf::FloatRect& rect);

	static float Clamp(float v, float min, float max);
	static sf::Vector2f Clamp(const sf::Vector2f& v, const sf::Vector2f& min, const sf::Vector2f& max);
	static float Lerp(float a, float b, float t, bool clamping = true);
	static sf::Vector2f Lerp(const sf::Vector2f& a, const sf::Vector2f& b, float t, bool clamping = true);
	static sf::Color Lerp(const sf::Color& a, const sf::Color& b, float t, bool clamping = true);

	static const sf::Vector2f Normalize(const sf::Vector2f& vector);
	static float Magnitude(const sf::Vector2f& vector2);
	static float SqrMagnitude(const sf::Vector2f& vector);
	static float Distance(const sf::Vector2f& p1, const sf::Vector2f& p2);

	static float Angle(const sf::Vector2f& start, const sf::Vector2f& end);
	static float Angle(const sf::Vector2f& dir);
};

