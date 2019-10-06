#pragma once
#include "GameObject.h"

class ModelComponent;
class ControllerComponent;

class BomberMan3DCharacter : public GameObject
{
public:
	enum CharacterMovement : UINT
	{
		LEFT = 0,
		RIGHT,
		FORWARD,
		BACKWARD,
		DROPBOMB,
		LEFT2,
		RIGHT2,
		FORWARD2,
		BACKWARD2,
		DROPBOMB2
	};

	BomberMan3DCharacter(float radius = 2, float height = 5, float moveSpeed = 100, int playerID = 1, int cols = 15, int rows = 15);
	~BomberMan3DCharacter() = default;

	BomberMan3DCharacter(const BomberMan3DCharacter& other) = delete;
	BomberMan3DCharacter(BomberMan3DCharacter&& other) noexcept = delete;
	BomberMan3DCharacter& operator=(const BomberMan3DCharacter& other) = delete;
	BomberMan3DCharacter& operator=(BomberMan3DCharacter&& other) noexcept = delete;

	void DecreaseBombPlaceDelay() { m_BombPlaceDelay -= 0.2f; }
	void IncreaseMoveSpeed() { m_MoveSpeed += 10; };
	void IncreaseRange() { ++m_Range; }
	int GetBlockId();
	DirectX::XMFLOAT3 GetPositionOnGrid();
	void LoseLife();
	void PlayVictoryAnimation();
	int GetLifes() { return m_Lives; }
	bool GetDead() { return m_IsDead; }
	float GetMoveSpeed() { return m_MoveSpeed; }
	int GetRange() { return m_Range; }
	float GetBombPlaceDelay() { return m_BombPlaceDelay; }
	void DropBomb(const GameContext& gameContext);
	void Initialize(const GameContext& gameContext) override;
	void PostInitialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void FixCollisionOnRestart();

protected:
	ModelComponent* m_pModel;
	ControllerComponent* m_pController;
	bool m_IsDead = false;
	float m_MoveSpeed;
	float m_Radius, m_Height;
	int m_PlayerID;
	int m_BlockId;
	int m_Cols;
	int m_Rows;
	int m_Range = 1;
	int m_Lives = 3;
	//Running
	float m_MaxRunVelocity,
		m_TerminalVelocity,
		m_RunAccelerationTime,
		m_RunAcceleration,
		m_JumpAcceleration,
		m_RunVelocity;
	DirectX::XMFLOAT3 m_Velocity;
	float m_BombPlaceDelay;
	float m_AccuTime;
};

