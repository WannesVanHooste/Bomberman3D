#include "stdafx.h"

#include "SpriteComponent.h"
 #include <utility>

#include "GameObject.h"
#include "TextureData.h"
#include "ContentManager.h"
#include "SpriteRenderer.h"
#include "TransformComponent.h"

SpriteComponent::SpriteComponent(std::wstring spriteAsset, DirectX::XMFLOAT2 pivot, DirectX::XMFLOAT4 color):
	m_pTexture(nullptr),
	m_SpriteAsset(std::move(spriteAsset)),
	m_Pivot(pivot),
	m_Color(color)
{
}

void SpriteComponent::Initialize(const GameContext& )
{
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::SetTexture(const std::wstring& spriteAsset)
{
	m_SpriteAsset = spriteAsset;
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::Update(const GameContext& )
{
}

void SpriteComponent::Draw(const GameContext& )
{
	if (!m_pTexture)
		return;

	//TODO: Here you need to draw the SpriteComponent using the Draw of the sprite renderer
	// The sprite renderer is a singleton
	// you will need to position, the rotation and the scale
	// You can use the QuaternionToEuler function to help you with the z rotation 
	auto position = GetGameObject()->GetTransform()->GetPosition();
	auto worldPosition = GetGameObject()->GetTransform()->GetWorldPosition();
	//auto rotation = QuaternionToEuler(GetGameObject()->GetTransform()->GetRotation());
	//auto scale = GetGameObject()->GetTransform()->GetScale();
	auto worldScale = GetGameObject()->GetTransform()->GetWorldScale();
	auto worldRotation = QuaternionToEuler(GetGameObject()->GetTransform()->GetWorldRotation());
	SpriteRenderer* renderer = SpriteRenderer::GetInstance();
	renderer->Draw(m_pTexture, DirectX::XMFLOAT2(worldPosition.x, worldPosition.y), m_Color, m_Pivot, DirectX::XMFLOAT2(worldScale.x, worldScale.y), worldRotation.z, position.z);
}
