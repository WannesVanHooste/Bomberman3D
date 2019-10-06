#pragma once
#include "GameObject.h"
#include "ModelComponent.h"
#include "ColliderComponent.h"


class SolidBlock :public GameObject
{
public:
	SolidBlock();
	~SolidBlock() =default;

	SolidBlock(const SolidBlock& other) = delete;
	SolidBlock(SolidBlock&& other) noexcept = delete;
	SolidBlock& operator=(const SolidBlock& other) = delete;
	SolidBlock& operator=(SolidBlock&& other) noexcept = delete;

	void ResetBlock();
	void Initialize(const GameContext& gameContext) override;
	void PostInitialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;

private:
	ModelComponent* m_pModel;
	ColliderComponent*m_pCollider;
};

