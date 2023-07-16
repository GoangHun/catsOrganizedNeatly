#include "stdafx.h"
#include "Utils.h"

std::random_device Utils::rd;
std::mt19937 Utils::gen(Utils::rd());

int Utils::RandomRange(int min, int maxExclude)
{
	std::uniform_int_distribution<> dist(min, maxExclude - 1);
	return dist(gen);
}

float Utils::RandomRange(float min, float max)
{
	std::uniform_real_distribution<> dist(min, max);
	return (float)dist(gen);
}

sf::Vector2f Utils::RandomOnCircle(float radius)
{
	sf::Vector2f result(RandomRange(-1.0f, 1.0f), RandomRange(-1.0f, 1.0f));
	result = Normalize(result) * radius;
	return result;
}

sf::Vector2f Utils::RandomInCircle(float radius)
{
	return RandomOnCircle(radius) * RandomValue();
}

float Utils::RandomValue()
{
	return RandomRange(0.0f, 1.0f);
}

void Utils::SetOrigin(sf::Sprite& sprite, Origins origin)
{
	SetOrigin(sprite, origin, sprite.getLocalBounds());
}

void Utils::SetOrigin(sf::Text& text, Origins origin)
{
	SetOrigin(text, origin, text.getLocalBounds());
}

void Utils::SetOrigin(sf::Shape& shape, Origins origin)
{
	SetOrigin(shape, origin, shape.getLocalBounds());
}

void Utils::SetOrigin(sf::Transformable& obj, Origins origin, const sf::FloatRect& rect)
{
	sf::Vector2f originPos(rect.width, rect.height);
	originPos.x *= ((int)origin % 3) * 0.5f;
	originPos.y *= ((int)origin / 3) * 0.5f;
	obj.setOrigin(originPos);
}

float Utils::Clamp(float v, float min, float max)
{
	return std::max(min, std::min(max, v));
}

sf::Vector2f Utils::Clamp(const sf::Vector2f& v, const sf::Vector2f& min, const sf::Vector2f& max)
{
	sf::Vector2f result;
	result.x = Clamp(v.x, min.x, max.x);
	result.y = Clamp(v.y, min.y, max.y);
	return result;
}

float Utils::Lerp(float a, float b, float t, bool clamping)
{
	float v = a + (b - a) * t;
	if (clamping)
	{
		v = Utils::Clamp(v, std::min(a, b), std::max(a, b));
	}
	return v;
}

sf::Vector2f Utils::Lerp(const sf::Vector2f& a, const sf::Vector2f& b, float t, bool clamping)
{
	if (clamping)
	{
		t = Utils::Clamp(t, 0.f , 1.f);
	}
	return  a + Utils::Normalize(b - a) * (Utils::Distance(a, b) * t);;
}

sf::Color Utils::Lerp(const sf::Color& a, const sf::Color& b, float t, bool clamping)
{
	sf::Color v;
	v.r = (UINT8)Lerp(a.r, b.r, t, clamping);
	v.g = (UINT8)Lerp(a.g, b.g, t, clamping);
	v.b = (UINT8)Lerp(a.b, b.b, t, clamping);
	v.a = (UINT8)Lerp(a.a, b.a, t, clamping);
	return v;
}

const sf::Vector2f Utils::Normalize(const sf::Vector2f& vector)
{
	float mag = Magnitude(vector);
	if (mag == 0.f)
	{
		return vector;
	}
	return vector / mag;
}

float Utils::Magnitude(const sf::Vector2f& vector)
{
	//sqrt(제곱근 함수) x^2 + y^2의 제곱근 == vector의 길이
	return sqrt(vector.x * vector.x + vector.y * vector.y);
}

float Utils::SqrMagnitude(const sf::Vector2f& vector)
{
	//vector의 길이^2
	//vector끼리 비교할 때 sqrt() 연산 비용을 아끼기 위해 사용
	return vector.x * vector.x + vector.y * vector.y;
}

float Utils::Distance(const sf::Vector2f& p1, const sf::Vector2f& p2)
{
	return Magnitude(p2 - p1);
}

float Utils::Angle(const sf::Vector2f& start, const sf::Vector2f& end)
{
	return Angle(Normalize(end - start));
}

float Utils::Angle(const sf::Vector2f& dir)
{
	return (float)(atan2(dir.y, dir.x) * (180.f / M_PI));
}

//각도를 넘겨서 x, y좌표 계산
sf::Vector2f Utils::Direction(const float angle)
{
	//PI는 원의 지름 / 원의 둘레 == 반지름 / (원의 둘레/2) 
	//각도 단위는 반지름이 1인 원의 라디안
	float radians = angle * (M_PI / 180.f);

	// x와 y 좌표 계산
	float x = cos(radians);
	float y = sin(radians);

	return sf::Vector2f(x, y);
}