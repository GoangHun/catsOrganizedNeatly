#pragma once
#include "ShapeGo.h"
#include "Player1.h"

class Ground :
    public ShapeGo
{
protected:
	Player1* player = nullptr;

public:
	Ground(const std::string& n = "");
	virtual ~Ground();

	virtual void Init() override;
	virtual void Release() override;

	virtual void Reset() override;

	virtual void Update(float dt) override;

	void SetPlayer(Player1* player) { this->player = player; }
	
};

