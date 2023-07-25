#pragma once
#include "AniSpriteGo.h"
#include "UIButton.h"


class ChapterBoard :
    public AniSpriteGo
{
protected:
	SpriteGo chapterPot;
	SpriteGo backgroundPaper;

	std::vector<UIButton*> uiButtons;
	std::vector<sf::Vector2f> buttonPoss;
public:
	ChapterBoard(int chapterNum, const std::string& animationId, const std::string& resourcePath = "", const std::string& n = "");
	virtual ~ChapterBoard() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetButtonPositions();
	void SetInitValue(GameObject* go, Origins origin, sf::Vector2f pos, float angle = 0, int layer = 100);

};

