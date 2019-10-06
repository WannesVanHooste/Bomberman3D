//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "DiffuseMaterial_Shadow.h"
#include "GeneralStructs.h"
#include "Logger.h"
#include "ContentManager.h"
#include "TextureData.h"
#include "Components.h"

ID3DX11EffectShaderResourceVariable* DiffuseMaterial_Shadow::m_pDiffuseSRVvariable = nullptr;
ID3DX11EffectShaderResourceVariable* DiffuseMaterial_Shadow::m_pShadowSRVvariable = nullptr;
ID3DX11EffectVectorVariable* DiffuseMaterial_Shadow::m_pLightDirectionVariable = nullptr;
ID3DX11EffectMatrixVariable* DiffuseMaterial_Shadow::m_pLightWVPvariable = nullptr;

DiffuseMaterial_Shadow::DiffuseMaterial_Shadow() : Material(L"./Resources/Effects/Shadow/PosNormTex3D_Shadow.fx"),
	m_pDiffuseTexture(nullptr)
{}

void DiffuseMaterial_Shadow::SetDiffuseTexture(const std::wstring& assetFile)
{
	UNREFERENCED_PARAMETER(assetFile);
	//TODO: store the diffuse texture in the appropriate member
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}

void DiffuseMaterial_Shadow::SetLightDirection(DirectX::XMFLOAT3 dir)
{
	UNREFERENCED_PARAMETER(dir);
	//TODO: store the light direction in the appropriate member
	m_LightDirection = dir;
}

void DiffuseMaterial_Shadow::LoadEffectVariables()
{
	//TODO: load all the necessary shader variables
	m_pDiffuseSRVvariable = GetEffect()->GetVariableByName("gDiffuseMap")->AsShaderResource();
	m_pShadowSRVvariable = GetEffect()->GetVariableByName("gShadowMap")->AsShaderResource();
	m_pLightDirectionVariable = GetEffect()->GetVariableByName("gLightDirection")->AsVector();
	m_pLightWVPvariable = GetEffect()->GetVariableByName("gWorldViewProj_Light")->AsMatrix();

}

void DiffuseMaterial_Shadow::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	//UNREFERENCED_PARAMETER(gameContext);
	//UNREFERENCED_PARAMETER(pModelComponent);
	////TODO: update all the necessary shader variables
	//m_pLightDirectionVariable->SetFloatVector(&m_LightDirection.x);
	////diffuse
	//m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());
	//m_pShadowSRVvariable->SetResource(gameContext.pShadowMapper->GetShadowMap());

	//DirectX::XMFLOAT4X4 vp = gameContext.pShadowMapper->GetLightVP();
	//DirectX::XMMATRIX  lightVP = DirectX::XMLoadFloat4x4(&vp);
	//DirectX::XMMATRIX  world = DirectX::XMLoadFloat4x4(&gameContext.pCamera->GetTransform()->GetWorld());
	//DirectX::XMMATRIX WVP = DirectX::XMMatrixMultiply(lightVP, world);

	//DirectX::XMFLOAT4X4 WVP4x4{};
	//DirectX::XMStoreFloat4x4(&WVP4x4, WVP);
	//m_pLightWVPvariable->SetMatrix(&WVP4x4._11);

	m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());
	//m_pBoneTransforms->SetMatrixArray(&pModelComponent->GetAnimator()->GetBoneTransforms()[0]._11, 0, pModelComponent->GetAnimator()->GetBoneTransforms().size());
	m_pShadowSRVvariable->SetResource(gameContext.pShadowMapper->GetShadowMap());
	m_pLightDirectionVariable->SetFloatVector(&m_LightDirection.x);

	DirectX::XMFLOAT4X4 LightVP = gameContext.pShadowMapper->GetLightVP();
	DirectX::XMFLOAT4X4 World = pModelComponent->GetTransform()->GetWorld();
	DirectX::XMFLOAT4X4 lightWVP4x4{};
	DirectX::XMStoreFloat4x4(&lightWVP4x4, DirectX::XMLoadFloat4x4(&World) * DirectX::XMLoadFloat4x4(&LightVP));
	m_pLightWVPvariable->SetMatrix(&lightWVP4x4._11);
}