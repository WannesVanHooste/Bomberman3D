//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "ShadowMapMaterial.h"
#include "ContentManager.h"

ShadowMapMaterial::~ShadowMapMaterial()
{
	//TODO: make sure you don't have memory leaks and/or resource leaks :) -> Figure out if you need to do something here

}

void ShadowMapMaterial::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	if (!m_IsInitialized)
	{
		//TODO: initialize the effect, techniques, shader variables, input layouts (hint use EffectHelper::BuildInputLayout), etc.
		m_pShadowEffect = ContentManager::Load<ID3DX11Effect>(L"./Resources/Effects/ShadowMapGenerator.fx");
		m_pShadowTechs[ShadowGenType::Static] = m_pShadowEffect->GetTechniqueByName("GenerateShadows");
		m_pShadowTechs[ShadowGenType::Skinned] = m_pShadowEffect->GetTechniqueByName("GenerateShadows_Skinned");
		m_pWorldMatrixVariable = m_pShadowEffect->GetVariableByName("gWorld")->AsMatrix();
		m_pBoneTransforms = m_pShadowEffect->GetVariableByName("gBones")->AsMatrix();
		m_pLightVPMatrixVariable = m_pShadowEffect->GetVariableByName("gLightViewProj")->AsMatrix();

		if (!EffectHelper::BuildInputLayout(gameContext.pDevice, m_pShadowTechs[ShadowGenType::Static],
			&m_pInputLayouts[ShadowGenType::Static], m_InputLayoutDescriptions[ShadowGenType::Static],
			m_InputLayoutSizes[ShadowGenType::Static], m_InputLayoutIds[ShadowGenType::Static]))
			Logger::LogError(L"inputlayout ShadowGenType::Static not initialized");
		if (!EffectHelper::BuildInputLayout(gameContext.pDevice, m_pShadowTechs[ShadowGenType::Skinned],
			&m_pInputLayouts[ShadowGenType::Skinned], m_InputLayoutDescriptions[ShadowGenType::Skinned],
			m_InputLayoutSizes[ShadowGenType::Skinned], m_InputLayoutIds[ShadowGenType::Skinned]))
			Logger::LogError(L"inputlayout 1 not initialized");

	}
}

void ShadowMapMaterial::SetLightVP(DirectX::XMFLOAT4X4 lightVP) const
{
	//UNREFERENCED_PARAMETER(lightVP);
	//TODO: set the correct shader variable
	m_pLightVPMatrixVariable->SetMatrix(&lightVP._11);
}

void ShadowMapMaterial::SetWorld(DirectX::XMFLOAT4X4 world) const
{
	//UNREFERENCED_PARAMETER(world);
	//TODO: set the correct shader variable
	m_pWorldMatrixVariable->SetMatrix(&world._11);
}

void ShadowMapMaterial::SetBones(const float* pData, int count) const
{
	//UNREFERENCED_PARAMETER(pData);
	//UNREFERENCED_PARAMETER(count);
	//TODO: set the correct shader variable
	m_pBoneTransforms->SetMatrixArray(pData, 0, count);
}
