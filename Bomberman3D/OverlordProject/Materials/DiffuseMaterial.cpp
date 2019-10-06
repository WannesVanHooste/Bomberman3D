#include "stdafx.h"
#include "DiffuseMaterial.h"
#include "ContentManager.h"

ID3DX11EffectShaderResourceVariable* DiffuseMaterial::m_pDiffuseSRVvariable{};

DiffuseMaterial::DiffuseMaterial()
	:Material{L"./Resources/Effects/PosNormTex3D.fx"}
{
}


DiffuseMaterial::~DiffuseMaterial()
{
}

void DiffuseMaterial::SetDiffuseTexture(const std::wstring & assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}

void DiffuseMaterial::LoadEffectVariables()
{
	ID3DX11Effect* effect = ContentManager::Load<ID3DX11Effect>(L"./Resources/Effects/PosNormTex3D.fx");
	m_pDiffuseSRVvariable = effect->GetVariableByName("gDiffuseMap")->AsShaderResource();

}

void DiffuseMaterial::UpdateEffectVariables(const GameContext & gameContext, ModelComponent * pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);

	m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());
}
