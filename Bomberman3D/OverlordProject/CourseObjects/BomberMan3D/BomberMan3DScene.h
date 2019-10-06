#pragma once
#include "GameScene.h"
#include "BomberMan3DCharacter.h"
#include "BomberMan3DLevel.h"
#include "Timer.h"
#include "SpriteFont.h"
#include "PostChromaticAberration.h"
#include "PostInvert.h"

class BomberMan3DScene :public GameScene
{
public:
	BomberMan3DScene();
	virtual ~BomberMan3DScene();

	BomberMan3DScene(const BomberMan3DScene& other) = delete;
	BomberMan3DScene(BomberMan3DScene&& other) noexcept = delete;
	BomberMan3DScene& operator=(const BomberMan3DScene& other) = delete;
	BomberMan3DScene& operator=(BomberMan3DScene&& other) noexcept = delete;
	void RestartScene();
protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;

private:
	GameObject* m_pGround;
	BomberMan3DLevel* m_pLevel;
	BomberMan3DCharacter* m_pPlayer1;
	BomberMan3DCharacter* m_pPlayer2;
	Timer* m_pTimer;
	GameObject* m_pTimerSprite;
	GameObject* m_pGroundPlane;
	SpriteFont* m_pSpriteFont;
	PostChromaticAberration* m_pPostChromatic;
	PostInvert* m_pPostInvert;
	bool m_ChromicAberration = false;
	bool m_InvertedColors = false;
	float m_EndTime = 2.7f;
	float m_AccuTime = 0.0f;

};

