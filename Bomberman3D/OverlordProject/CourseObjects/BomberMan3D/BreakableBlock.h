#pragma once
#include "GameObject.h"

class BreakableBlock :public GameObject
{
public:
	BreakableBlock();
	~BreakableBlock() = default;

	BreakableBlock(const BreakableBlock& other) = delete;
	BreakableBlock(BreakableBlock&& other) noexcept = delete;
	BreakableBlock& operator=(const BreakableBlock& other) = delete;
	BreakableBlock& operator=(BreakableBlock&& other) noexcept = delete;

	void ResetBlock();
	void Initialize(const GameContext& gameContext) override;
	void PostInitialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
};


