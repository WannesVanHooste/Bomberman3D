#pragma once
#include <GameObject.h>
class BombParticle :
	public GameObject
{
public:
	BombParticle(int playerId);
	~BombParticle() = default;

	BombParticle(const BombParticle& other) = delete;
	BombParticle(BombParticle&& other) noexcept = delete;
	BombParticle& operator=(const BombParticle& other) = delete;
	BombParticle& operator=(BombParticle&& other) noexcept = delete;

	void Initialize(const GameContext& gameContext) override;
	void PostInitialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;

private:
	float m_AccuSec = 0.0f;
	float m_LifeTime = 0.3f;
	int m_PlayerId;
};

