#include "stdafx.h"
#include "BombParticle.h"
#include "GameScene.h"
#include "ParticleEmitterComponent.h"
#include "ModelComponent.h"
#include "TransformComponent.h"

BombParticle::BombParticle(int playerId)
	:m_PlayerId(playerId)
{
}

void BombParticle::Initialize(const GameContext&) {
	std::wstring filePath{};
	if(m_PlayerId == 1) {
		filePath = L"./Resources/Textures/BlueFireBall.png";
	}
	else {
		filePath = L"./Resources/Textures/RedFireBall.png";
	}
	GameObject*	obj = new GameObject{};
		auto particle = new ParticleEmitterComponent{ filePath, 60 };
particle->SetVelocity(DirectX::XMFLOAT3(-10.0f, 2.0f, 0));
particle->SetMinSize(5.0f);
particle->SetMaxSize(7.0f);
particle->SetMinEnergy(1.0f);
particle->SetMaxEnergy(2.0f);
particle->SetMinSizeGrow(5.5f);
particle->SetMaxSizeGrow(7.5f);
particle->SetMinEmitterRange(0.2f);
particle->SetMaxEmitterRange(0.5f);
particle->SetColor(DirectX::XMFLOAT4(1.f, 1.f, 1.f, 0.6f));
obj->AddComponent(particle);
	AddChild(obj);
}

void BombParticle::PostInitialize(const GameContext&) {}

void BombParticle::Update(const GameContext& gameContext) {
	m_AccuSec += gameContext.pGameTime->GetElapsed();
	if(m_AccuSec >= m_LifeTime) {
		GetChild<GameObject>()->GetTransform()->Translate(1000, 0, 0);
		GetChild<GameObject>()->GetComponent<ParticleEmitterComponent>()->StopUpdate();
	}
}

