#pragma once
#include "ModelComponent.h"
#include "GameObject.h"

class SkyBox : public GameObject
{
public:
	SkyBox();
	~SkyBox();

	void Initialize(const GameContext& gameContext) override;
private:
	ModelComponent* model;
};

