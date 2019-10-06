#include "stdafx.h"
#include "Powerup.h"
#include "BomberMan3DLevel.h"
#include "PlayerLocator.h"
#include "BomberMan3DCharacter.h"
#include "ModelComponent.h"
#include "TransformComponent.h"
#include "SoundManager.h"

Powerup::Powerup(int blockID, int powerupId)
	:m_BlockID(blockID)
,m_pFMODSystem{}
,m_pChannel{}
,m_PowerupId(powerupId)
{
	
}

void Powerup::Initialize(const GameContext& ) {
	//SOUNDS
	m_pFMODSystem = SoundManager::GetInstance()->GetSystem();

	m_pFMODSystem->createSound("./Resources/Sounds/Pickup.wav", FMOD_2D, 0, &m_pPickupSound);

	m_pPickupSound->setMode(FMOD_LOOP_OFF);

	m_pChannel->setVolume(0.5f);
	//

	GameObject* obj = nullptr;
	obj = new GameObject{};
	auto model = new ModelComponent{ L"Resources/Meshes/Plus.ovm" };
	obj->AddComponent(model);
	model->SetMaterial(5);
	model->GetTransform()->Scale(0.25f, 0.25f, 0.25f);
	AddChild(obj);
}

void Powerup::PostInitialize(const GameContext& ) {
	
}

void Powerup::Update(const GameContext& ) {
	if (!m_IsUsed)
	{
		auto player1 = dynamic_cast<BomberMan3DCharacter*>(PlayerLocator::GetPlayerOne());
		auto player2 = dynamic_cast<BomberMan3DCharacter*>(PlayerLocator::GetPlayerTwo());
		if (m_BlockID == player1->GetBlockId()) {
			if (m_PowerupId == 0)
			{
				player1->IncreaseRange();
			}
			if(m_PowerupId == 1) {
				player1->IncreaseMoveSpeed();
			}
			if(m_PowerupId == 2) {
				player1->DecreaseBombPlaceDelay();
			}
			GetChild<GameObject>()->GetTransform()->Translate(1000, 0, 0);
			m_IsUsed = true;
			m_pFMODSystem->playSound(m_pPickupSound, 0, false, &m_pChannel);
		}
		if (m_BlockID == player2->GetBlockId()) {
			if (m_PowerupId == 0)
			{
				player2->IncreaseRange();
			}
			if(m_PowerupId == 1) {
				player2->IncreaseMoveSpeed();
			}
			if (m_PowerupId == 2) {
				player2->DecreaseBombPlaceDelay();
			}
			m_IsUsed = true;
			GetChild<GameObject>()->GetTransform()->Translate(1000, 0, 0);
			m_pFMODSystem->playSound(m_pPickupSound, 0, false, &m_pChannel);
		}
	}
}
