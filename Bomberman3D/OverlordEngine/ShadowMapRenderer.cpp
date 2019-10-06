#include "stdafx.h"
#include "ShadowMapRenderer.h"
#include "ContentManager.h"
#include "ShadowMapMaterial.h"
#include "RenderTarget.h"
#include "MeshFilter.h"
#include "SceneManager.h"
#include "OverlordGame.h"

ShadowMapRenderer::~ShadowMapRenderer()
{
	//TODO: make sure you don't have memory leaks and/or resource leaks :) -> Figure out if you need to do something here
	SafeDelete(m_pShadowRT);
	SafeDelete(m_pShadowMat);
}

void ShadowMapRenderer::Initialize(const GameContext& gameContext)
{
	if (m_IsInitialized)
		return;

	//UNREFERENCED_PARAMETER(gameContext);
	//TODO: create shadow generator material + initialize it
	m_pShadowMat = new ShadowMapMaterial{};
	m_pShadowMat->Initialize(gameContext);

	//TODO: create a rendertarget with the correct settings (hint: depth only) for the shadow generator using a RENDERTARGET_DESC
	m_pShadowRT = new RenderTarget{ gameContext.pDevice };
	RENDERTARGET_DESC desc{};
	desc.Width = OverlordGame::GetGameSettings().Window.Width;
	desc.Height = OverlordGame::GetGameSettings().Window.Height;
	desc.EnableDepthBuffer = true;
	desc.EnableColorBuffer = false;
	desc.EnableColorSRV = false;
	desc.EnableDepthSRV = true;
	m_pShadowRT->Create(desc);

	m_IsInitialized = true;
}

void ShadowMapRenderer::SetLight(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 direction)
{
	//UNREFERENCED_PARAMETER(position);
	//UNREFERENCED_PARAMETER(direction);
	//TODO: store the input parameters in the appropriate datamembers
	m_LightPosition = position;
	m_LightDirection = direction;
	//TODO: calculate the Light VP matrix (Directional Light only ;)) and store it in the appropriate datamember

	//reference from Pieter G.
	const auto windowSettings = OverlordGame::GetGameSettings().Window;
	DirectX::XMVECTOR LightPosition = DirectX::XMLoadFloat3(&position);
	DirectX::XMVECTOR LightDirection = DirectX::XMLoadFloat3(&direction);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	const float viewWidth = (m_Size > 0) ? m_Size * windowSettings.AspectRatio : windowSettings.Width;
	const float viewHeight = (m_Size > 0) ? m_Size : windowSettings.Height;

	//const DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(LightPosition, LightDirection, up);

	//DirectX::XMMATRIX projection;
	DirectX::XMMATRIX view = DirectX::XMMatrixLookToLH(LightPosition, LightDirection, up);
	DirectX::XMMATRIX projection = DirectX::XMMatrixOrthographicLH(viewWidth * 2.2f, viewHeight * 2.2f, 0.1f, 450.0f);

	DirectX::XMStoreFloat4x4(&m_LightVP, view * projection);
}

void ShadowMapRenderer::Begin(const GameContext& gameContext) const
{
	//Reset Texture Register 5 (Unbind)
	ID3D11ShaderResourceView *const pSRV[] = { nullptr };
	gameContext.pDeviceContext->PSSetShaderResources(1, 1, pSRV);

	//UNREFERENCED_PARAMETER(gameContext);
	//TODO: set the appropriate render target that our shadow generator will write to (hint: use the OverlordGame::SetRenderTarget function through SceneManager)
	
	SceneManager::GetInstance()->GetGame()->SetRenderTarget(m_pShadowRT);
	//TODO: clear this render target
	m_pShadowRT->Clear(gameContext, DirectX::Colors::CornflowerBlue);
	
	//TODO: set the shader variables of this shadow generator material
	//onnodig
}

void ShadowMapRenderer::End(const GameContext&) const
{
	//UNREFERENCED_PARAMETER(gameContext);
	//TODO: restore default render target (hint: passing nullptr to OverlordGame::SetRenderTarget will do the trick)
	SceneManager::GetInstance()->GetGame()->SetRenderTarget(nullptr);
}

void ShadowMapRenderer::Draw(const GameContext& gameContext, MeshFilter* pMeshFilter, DirectX::XMFLOAT4X4 world, const std::vector<DirectX::XMFLOAT4X4>& bones) const
{
	//UNREFERENCED_PARAMETER(gameContext);
	//UNREFERENCED_PARAMETER(pMeshFilter);
	//UNREFERENCED_PARAMETER(world);
	//UNREFERENCED_PARAMETER(bones);

	//TODO: update shader variables in material
	//m_pShadowMat->SetBones(reinterpret_cast<const float*>(&bones), bones.size());
	//m_pShadowMat->SetWorld(world);
	//m_pShadowMat->SetLightVP(m_LightVP);

	//TODO: update shader variables in material
	//TODO: set the correct inputlayout, buffers, topology (some variables are set based on the generation type Skinned or Static)
	ID3DX11EffectTechnique* shadowTechnique{ nullptr };
	VertexBufferData vertexBufferData{};
	UINT offset = 0;

	const ShadowMapMaterial::ShadowGenType type = (!pMeshFilter->m_HasAnimations) ? ShadowMapMaterial::ShadowGenType::Static : ShadowMapMaterial::ShadowGenType::Skinned;

	m_pShadowMat->SetBones(reinterpret_cast<const float*>(bones.data()), bones.size());
	m_pShadowMat->SetLightVP(m_LightVP);
	m_pShadowMat->SetWorld(world);

	shadowTechnique = m_pShadowMat->m_pShadowTechs[type];
	vertexBufferData = pMeshFilter->GetVertexBufferData(gameContext, m_pShadowMat->m_InputLayoutIds[type]);

	gameContext.pDeviceContext->IASetInputLayout(m_pShadowMat->m_pInputLayouts[type]);
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &vertexBufferData.pVertexBuffer, &vertexBufferData.VertexStride, &offset);
	gameContext.pDeviceContext->IASetIndexBuffer(pMeshFilter->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3DX11_TECHNIQUE_DESC techDesc{};
	shadowTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		shadowTechnique->GetPassByIndex(p)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->DrawIndexed(pMeshFilter->m_IndexCount, 0, 0);
	}
}

void ShadowMapRenderer::UpdateMeshFilter(const GameContext& gameContext, MeshFilter* pMeshFilter)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pMeshFilter);
	//TODO: based on the type (Skinned or Static) build the correct vertex buffers for the MeshFilter (Hint use MeshFilter::BuildVertexBuffer)
	ShadowMapMaterial::ShadowGenType type = (!pMeshFilter->m_HasAnimations) ? ShadowMapMaterial::ShadowGenType::Static : ShadowMapMaterial::ShadowGenType::Skinned;
	pMeshFilter->BuildVertexBuffer(gameContext, m_pShadowMat->m_InputLayoutIds[type], m_pShadowMat->m_InputLayoutSizes[type], m_pShadowMat->m_InputLayoutDescriptions[type]);
}

ID3D11ShaderResourceView* ShadowMapRenderer::GetShadowMap() const
{
	//TODO: return the depth shader resource view of the shadow generator render target
	return m_pShadowRT->GetDepthShaderResourceView();
}
