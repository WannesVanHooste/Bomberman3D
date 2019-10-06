#include "stdafx.h"
#include "SkyBoxMaterial.h"
#include "ContentManager.h"

ID3DX11EffectShaderResourceVariable* SkyBoxMaterial::m_pDiffuseSRVvariable{};


SkyBoxMaterial::SkyBoxMaterial()
	:Material{ L"./Resources/Effects/SkyBox.fx" }
{}

SkyBoxMaterial::~SkyBoxMaterial()
{
}

void SkyBoxMaterial::SetCubeTexture(const std::wstring& assetFile)
{
	m_pCubeTexture = ContentManager::Load<TextureData>(assetFile);
}

void SkyBoxMaterial::LoadEffectVariables() {
	ID3DX11Effect* effect = ContentManager::Load<ID3DX11Effect>(L"./Resources/Effects/SkyBox.fx");
	m_pDiffuseSRVvariable = effect->GetVariableByName("m_CubeMap")->AsShaderResource();
}

void SkyBoxMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent) {
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);

	m_pDiffuseSRVvariable->SetResource(m_pCubeTexture->GetShaderResourceView());
}
