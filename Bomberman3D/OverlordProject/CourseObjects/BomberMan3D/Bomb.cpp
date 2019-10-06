#include "stdafx.h"
#include "Bomb.h"
#include "BomberMan3DLevel.h"
#include "TransformComponent.h"
#include "ModelComponent.h"
#include "BombParticle.h"
#include "PlayerLocator.h"
#include "SceneManager.h"
#include "BomberMan3DCharacter.h"
#include "GameObject.h"
#include "GameScene.h"
#include "BomberMan3DScene.h"
#include "Powerup.h"
#include "SoundManager.h"

Bomb::Bomb(int range, float explodeTime, int playerID, int blockID, int cols, int rows)
:m_Range(range)
,m_ExplodeTime(explodeTime)
,m_Radius(1.0f)
,m_PlayerID(playerID)
,m_BlockId(blockID)
,m_Cols(cols)
,m_Rows(rows)
,m_pChannel{}
,m_pFMODSystem{}
{
}

void Bomb::Explode() {
	auto blockInfo = BomberMan3DLevel::GetBlockInfo();
	auto blocks = BomberMan3DLevel::GetBlocks();

	//Block itself
	//SPAWN PARTICLE
	BombParticle* bombParticle = nullptr;
	bombParticle = new BombParticle{ m_PlayerID };
	AddChild(bombParticle);
	bombParticle->GetChild<GameObject>()->GetTransform()->Translate(blockInfo[m_BlockId].position);


	//CHECK PLAYER DEAD
	int player1Id = dynamic_cast<BomberMan3DCharacter*>(PlayerLocator::GetPlayerOne())->GetBlockId();
	int player2Id = dynamic_cast<BomberMan3DCharacter*>(PlayerLocator::GetPlayerTwo())->GetBlockId();
	if (player1Id == m_BlockId) {
		dynamic_cast<BomberMan3DCharacter*>(PlayerLocator::GetPlayerOne())->LoseLife();
	}
	if (player2Id == m_BlockId) {
		dynamic_cast<BomberMan3DCharacter*>(PlayerLocator::GetPlayerTwo())->LoseLife();
	}


	//right side
	for (int i{1}; i < m_Range + 1; ++i)
	{
		if (!blockInfo[m_BlockId + i].isBreakable) { //if the box is unbreakable stop loop
			break;
		}
		if(!blockInfo[m_BlockId + i].hasBlock) { //if the block has no box continue next space
			//SPAWN PARTICLE
			bombParticle = new BombParticle{ m_PlayerID };
			AddChild(bombParticle);
			bombParticle->GetChild<GameObject>()->GetTransform()->Translate(blockInfo[m_BlockId + i].position);

			//CHECK PLAYER DEAD
			if (player1Id == m_BlockId + i) {
				dynamic_cast<BomberMan3DCharacter*>(PlayerLocator::GetPlayerOne())->LoseLife();
			}
			if (player2Id == m_BlockId + i) {
				dynamic_cast<BomberMan3DCharacter*>(PlayerLocator::GetPlayerTwo())->LoseLife();
			}

			continue;
		}
		if (blockInfo[m_BlockId + i].hasBlock && blockInfo[m_BlockId + i].isBreakable) { //if block has a box destroy and brake
			BomberMan3DLevel::SetHasBlock(m_BlockId + i, false);
			blocks[m_BlockId + i]->GetTransform()->Translate(1000, 1000, 1000);

			//SPAWN PARTICLE
			bombParticle = new BombParticle{ m_PlayerID };
			AddChild(bombParticle);
			bombParticle->GetChild<GameObject>()->GetTransform()->Translate(blockInfo[m_BlockId + i].position);

			//CHECK PLAYER DEAD
			if (player1Id == m_BlockId + i) {
				dynamic_cast<BomberMan3DCharacter*>(PlayerLocator::GetPlayerOne())->LoseLife();
			}
			if (player2Id == m_BlockId + i) {
				dynamic_cast<BomberMan3DCharacter*>(PlayerLocator::GetPlayerTwo())->LoseLife();
			}

			int random = rand() % 6;
			if (random < 3) {
				//SPAWN RANDOM POWERUP (Extra range, faster placing, faster moving, extra life)
				auto powerup = new Powerup{ m_BlockId + i, random};
				AddChild(powerup);
				powerup->GetChild<GameObject>()->GetTransform()->Translate(DirectX::XMFLOAT3{ float(5 * i),0,0.5f });
			}

			break;
		}
	}

	//left side
	for (int i{ 1 }; i < m_Range + 1; ++i)
	{
		if (!blockInfo[m_BlockId - i].isBreakable) { //if the box is unbreakable stop loop
			break;
		}
		if (!blockInfo[m_BlockId - i].hasBlock) { //if the block has no box continue next space
			//SPAWN PARTICLE
			bombParticle = new BombParticle{ m_PlayerID };
			AddChild(bombParticle);
			bombParticle->GetChild<GameObject>()->GetTransform()->Translate(blockInfo[m_BlockId - i].position);

			//CHECK PLAYER DEAD
			if (player1Id == m_BlockId - i) {
				dynamic_cast<BomberMan3DCharacter*>(PlayerLocator::GetPlayerOne())->LoseLife();
			}
			if (player2Id == m_BlockId - i) {
				dynamic_cast<BomberMan3DCharacter*>(PlayerLocator::GetPlayerTwo())->LoseLife();
			}

			continue;
		}
		if (blockInfo[m_BlockId - i].hasBlock && blockInfo[m_BlockId - i].isBreakable) { //if block has a box destroy and brake
			BomberMan3DLevel::SetHasBlock(m_BlockId - i, false);
			blocks[m_BlockId - i]->GetTransform()->Translate(1000, 1000, 1000);

			//SPAWN PARTICLE
			bombParticle = new BombParticle{ m_PlayerID };
			AddChild(bombParticle);
			bombParticle->GetChild<GameObject>()->GetTransform()->Translate(blockInfo[m_BlockId - i].position);

			//CHECK PLAYER DEAD
			if (player1Id == m_BlockId - i) {
				dynamic_cast<BomberMan3DCharacter*>(PlayerLocator::GetPlayerOne())->LoseLife();
			}
			if (player2Id == m_BlockId - i) {
				dynamic_cast<BomberMan3DCharacter*>(PlayerLocator::GetPlayerTwo())->LoseLife();
			}

			int random = rand() % 6;
			if (random < 3) {
				//SPAWN RANDOM POWERUP (Extra range, faster placing, faster moving, extra life)
				auto powerup = new Powerup{ m_BlockId - i, random};
				AddChild(powerup);
				powerup->GetChild<GameObject>()->GetTransform()->Translate(DirectX::XMFLOAT3{ float(-5 * i),0,0.5f });
			}

			break;
		}
	}

	//top side
	for (int i{ 1 }; i < m_Range + 1; ++i)
	{
		if (!blockInfo[m_BlockId + i * m_Cols].isBreakable) { //if the box is unbreakable stop loop
			break;
		}
		if (!blockInfo[m_BlockId + i * m_Cols].hasBlock) { //if the block has no box continue next space
			//SPAWN PARTICLE
			bombParticle = new BombParticle{ m_PlayerID };
			AddChild(bombParticle);
			bombParticle->GetChild<GameObject>()->GetTransform()->Translate(blockInfo[m_BlockId + i * m_Cols].position);

			//CHECK PLAYER DEAD
			if (player1Id == (m_BlockId + i * m_Cols)) {
				dynamic_cast<BomberMan3DCharacter*>(PlayerLocator::GetPlayerOne())->LoseLife();
			}
			if (player2Id == (m_BlockId + i * m_Cols)) {
				dynamic_cast<BomberMan3DCharacter*>(PlayerLocator::GetPlayerTwo())->LoseLife();
			}

			continue;
		}
		if (blockInfo[m_BlockId + i * m_Cols].hasBlock && blockInfo[m_BlockId + i * m_Cols].isBreakable) { //if block has a box destroy and brake
			BomberMan3DLevel::SetHasBlock(m_BlockId + i * m_Cols, false);
			blocks[m_BlockId + i * m_Cols]->GetTransform()->Translate(1000, 1000, 1000);

			//SPAWN PARTICLE
			bombParticle = new BombParticle{ m_PlayerID };
			AddChild(bombParticle);
			bombParticle->GetChild<GameObject>()->GetTransform()->Translate(blockInfo[m_BlockId + i * m_Cols].position);

			//CHECK PLAYER DEAD
			if (player1Id == (m_BlockId + i * m_Cols)) {
				dynamic_cast<BomberMan3DCharacter*>(PlayerLocator::GetPlayerOne())->LoseLife();
			}
			if (player2Id == (m_BlockId + i * m_Cols)) {
				dynamic_cast<BomberMan3DCharacter*>(PlayerLocator::GetPlayerTwo())->LoseLife();
			}

			int random = rand() % 6;
			if (random < 3) {
				//SPAWN RANDOM POWERUP (Extra range, faster placing, faster moving, extra life)
				auto powerup = new Powerup{ m_BlockId + i * m_Cols,random };
				AddChild(powerup);
				powerup->GetChild<GameObject>()->GetTransform()->Translate(DirectX::XMFLOAT3{ 0,0,(5 * i) + .5f });
			}

			break;
		}
	}

	//down side
	for (int i{ 1 }; i < m_Range + 1; ++i)
	{
		if (!blockInfo[m_BlockId - i * m_Cols].isBreakable) { //if the box is unbreakable stop loop
			break;
		}
		if (!blockInfo[m_BlockId - i * m_Cols].hasBlock) { //if the block has no box continue next space
			//SPAWN PARTICLE
			bombParticle = new BombParticle{ m_PlayerID };
			AddChild(bombParticle);
			bombParticle->GetChild<GameObject>()->GetTransform()->Translate(blockInfo[m_BlockId - i * m_Cols].position);

			//CHECK PLAYER DEAD
			if (player1Id == (m_BlockId - i * m_Cols)) {
				dynamic_cast<BomberMan3DCharacter*>(PlayerLocator::GetPlayerOne())->LoseLife();
			}
			if (player2Id == (m_BlockId - i * m_Cols)) {
				dynamic_cast<BomberMan3DCharacter*>(PlayerLocator::GetPlayerTwo())->LoseLife();
			}
			continue;

		}
		if (blockInfo[m_BlockId - i * m_Cols].hasBlock && blockInfo[m_BlockId - i * m_Cols].isBreakable) { //if block has a box destroy and brake
			BomberMan3DLevel::SetHasBlock(m_BlockId - i * m_Cols, false);
			blocks[m_BlockId - i * m_Cols]->GetTransform()->Translate(1000, 1000, 1000);

			//SPAWN PARTICLE
			bombParticle = new BombParticle{m_PlayerID};
			AddChild(bombParticle);
			bombParticle->GetChild<GameObject>()->GetTransform()->Translate(blockInfo[m_BlockId - i * m_Cols].position);

			//CHECK PLAYER DEAD
			if (player1Id == (m_BlockId - i * m_Cols)) {
				dynamic_cast<BomberMan3DCharacter*>(PlayerLocator::GetPlayerOne())->LoseLife();
			}
			if (player2Id == (m_BlockId - i * m_Cols)) {
				dynamic_cast<BomberMan3DCharacter*>(PlayerLocator::GetPlayerTwo())->LoseLife();
			}

			int random = rand() % 6;
			if(random < 3) {
				//SPAWN RANDOM POWERUP (Extra range, faster placing, faster moving, extra life)
				auto powerup = new Powerup{ m_BlockId - i * m_Cols,random };
				AddChild(powerup);
				powerup->GetChild<GameObject>()->GetTransform()->Translate(DirectX::XMFLOAT3{ 0,0,(-5 * i) + .5f });
			}

			break;
		}
	}
}

void Bomb::Initialize(const GameContext& gameContext) {
//Bomb test
UNREFERENCED_PARAMETER(gameContext);

//SOUNDS
m_pFMODSystem = SoundManager::GetInstance()->GetSystem();

m_pFMODSystem->createSound("./Resources/Sounds/Explosion.wav", FMOD_2D, 0, &m_pExplodeSound);
m_pFMODSystem->createSound("./Resources/Sounds/DropBomb.wav", FMOD_2D, 0, &m_pDropSound);


m_pExplodeSound->setMode(FMOD_LOOP_OFF);

m_pChannel->setVolume(0.5f);

m_pFMODSystem->playSound(m_pDropSound, 0, false, &m_pChannel);
//

GameObject* obj = nullptr;
if (m_PlayerID == 1)
{
	obj = new GameObject{};
	auto model = new ModelComponent{ L"Resources/Meshes/BombMesh.ovm" };
	obj->AddComponent(model);
	model->SetMaterial(3);
	model->GetTransform()->Scale(0.06f, 0.06f, 0.06f);
}
else {
	obj = new GameObject{};
	auto model = new ModelComponent{ L"Resources/Meshes/BombMesh.ovm" };
	obj->AddComponent(model);
	model->SetMaterial(4);
	model->GetTransform()->Scale(0.06f, 0.06f, 0.06f);
}
AddChild(obj);

}

void Bomb::PostInitialize(const GameContext&) {
	
}

void Bomb::Update(const GameContext& gameContext) {
	UNREFERENCED_PARAMETER(gameContext);
	if (!m_HasExploded)
	{
		m_AccuTime += gameContext.pGameTime->GetElapsed();
		if (m_AccuTime > m_ExplodeTime) {
			GetChild<GameObject>()->GetTransform()->Translate(1000, 0, 0);
			Explode();
			m_pFMODSystem->playSound(m_pExplodeSound, 0, false, &m_pChannel);
			m_HasExploded = true;
			m_AccuTime = 0.0f;
		}
	}
}
