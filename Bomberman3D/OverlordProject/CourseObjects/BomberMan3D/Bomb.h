#pragma once
#include "GameObject.h"

class Bomb : public GameObject
{
public:
	Bomb(int range, float explodeTime, int playerID, int blockID, int cols, int rows);
	~Bomb() = default;

	Bomb(const Bomb& other) = delete;
	Bomb(Bomb&& other) noexcept = delete;
	Bomb& operator=(const Bomb& other) = delete;
	Bomb& operator=(Bomb&& other) noexcept = delete;

	void Explode();
	void Initialize(const GameContext& gameContext) override;
	void PostInitialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;

private:
	FMOD::System *m_pFMODSystem;
	FMOD::Channel *m_pChannel;
	FMOD::Sound* m_pExplodeSound;
	FMOD::Sound* m_pDropSound;
	bool m_HasExploded = false;
	bool m_GameEnd = false;
	int m_PlayerID;
	int m_BlockId;
	int m_Range;
	int m_Cols;
	int m_Rows;
	float m_Radius;
	float m_ExplodeTime;
	float m_GameEndDelay = 1.0f;
	float m_AccuTime = 0.0f;
};

