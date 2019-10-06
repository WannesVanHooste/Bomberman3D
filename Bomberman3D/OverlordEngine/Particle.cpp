#include "stdafx.h"
#include "Particle.h"
#include "MathHelper.h"
// see https://stackoverflow.com/questions/21688529/binary-directxxmvector-does-not-define-this-operator-or-a-conversion
using namespace DirectX;

Particle::Particle(const ParticleEmitterSettings& emitterSettings):
	m_VertexInfo(ParticleVertex()),
	m_EmitterSettings(emitterSettings),
	m_IsActive(false),
	m_TotalEnergy(0),
	m_CurrentEnergy(0),
	m_SizeGrow(0),
	m_InitSize(0)
{}

void Particle::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	//TODO: See Lab9_2
	if(!m_IsActive) {
		return;
	}
	m_CurrentEnergy -= gameContext.pGameTime->GetElapsed();
	if(m_CurrentEnergy <= 0) {
		m_IsActive = false;
		return;
	}
	auto velocity = XMLoadFloat3(&m_EmitterSettings.Velocity);
	auto position = XMLoadFloat3(&m_VertexInfo.Position);
	velocity *= gameContext.pGameTime->GetElapsed();
	position += velocity;
	XMStoreFloat3(&m_VertexInfo.Position, position);

	m_VertexInfo.Color = m_EmitterSettings.Color;
	float particleLifePercent = m_CurrentEnergy / m_TotalEnergy;
	m_VertexInfo.Color.w = particleLifePercent * 2;

	m_VertexInfo.Size = m_InitSize + m_SizeGrow * (1 - particleLifePercent);
}

void Particle::Init(XMFLOAT3 initPosition)
{
	UNREFERENCED_PARAMETER(initPosition);
	//TODO: See Lab9_2
	m_IsActive = true;

	m_TotalEnergy = randF(m_EmitterSettings.MinEnergy, m_EmitterSettings.MaxEnergy);
	m_CurrentEnergy = randF(m_EmitterSettings.MinEnergy, m_EmitterSettings.MaxEnergy);

	XMFLOAT3 randomDirection{ 1, 0, 0 };
	XMVECTOR randomDirectionVector = XMLoadFloat3(&randomDirection);
	XMMATRIX randomMatrix = XMMatrixRotationRollPitchYaw(randF(-XM_PI, XM_PI), randF(-XM_PI, XM_PI), randF(-XM_PI, XM_PI));
	auto randomNormalizedVector = XMVector3TransformNormal(randomDirectionVector, randomMatrix);
	float randomDistance = randF(m_EmitterSettings.MinEmitterRange, m_EmitterSettings.MaxEmitterRange);
	randomNormalizedVector *= randomDistance;
	randomNormalizedVector += XMLoadFloat3(&initPosition);
	XMStoreFloat3(&m_VertexInfo.Position, randomNormalizedVector);

	m_VertexInfo.Size = randF(m_EmitterSettings.MinSize, m_EmitterSettings.MaxSize);
	m_InitSize = randF(m_EmitterSettings.MinSize, m_EmitterSettings.MaxSize);

	m_SizeGrow = randF(m_EmitterSettings.MinSizeGrow, m_EmitterSettings.MaxSizeGrow);

	m_VertexInfo.Rotation = randF(-XM_PI, XM_PI);
}
