#include "stdafx.h"
#include "PostProcessingMaterial.h"
#include "RenderTarget.h"
#include "OverlordGame.h"
#include "ContentManager.h"
#include "SpriteRenderer.h"

PostProcessingMaterial::PostProcessingMaterial(std::wstring effectFile, unsigned int renderIndex,
	std::wstring technique)
	: m_IsInitialized(false),
	m_pInputLayout(nullptr),
	m_pInputLayoutSize(0),
	m_effectFile(std::move(effectFile)),
	m_InputLayoutID(0),
	m_RenderIndex(renderIndex),
	m_pRenderTarget(nullptr),
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_NumVertices(0),
	m_NumIndices(0),
	m_VertexBufferStride(sizeof(VertexPosTex)),
	  m_pEffect(nullptr),
	  m_pTechnique(nullptr),
	  m_TechniqueName(std::move(technique))
{
}

PostProcessingMaterial::~PostProcessingMaterial()
{
	//TODO: delete and/or release necessary objects and/or resource
	SafeDelete(m_pRenderTarget);
	SafeRelease(m_pInputLayout);
	SafeRelease(m_pVertexBuffer);
	//m_pVertexBuffer->Release();
	//m_pIndexBuffer->Release();
}

void PostProcessingMaterial::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	if (!m_IsInitialized)
	{
		//TODO: complete
		//1. LoadEffect (LoadEffect(...))
		LoadEffect(gameContext, m_effectFile);
		//2. CreateInputLaytout (CreateInputLayout(...))
		//   CreateVertexBuffer (CreateVertexBuffer(...)) > As a TriangleStrip (FullScreen Quad)
		EffectHelper::BuildInputLayout(gameContext.pDevice, m_pTechnique, &m_pInputLayout, m_pInputLayoutDescriptions, m_pInputLayoutSize, m_InputLayoutID);
		CreateVertexBuffer(gameContext);
		CreateIndexBuffer(gameContext);
		//3. Create RenderTarget (m_pRenderTarget)
		//		Take a look at the class, figure out how to initialize/create a RenderTarget Object
		//		GameSettings > OverlordGame::GetGameSettings()
		m_pRenderTarget = new RenderTarget{ gameContext.pDevice };
		RENDERTARGET_DESC desc{};
		desc.Width = OverlordGame::GetGameSettings().Window.Width;
		desc.Height = OverlordGame::GetGameSettings().Window.Height;
		desc.EnableDepthBuffer = true;
		desc.EnableColorBuffer = true;
		desc.EnableColorSRV = true;
		desc.EnableDepthSRV = true;
		desc.ColorBufferSupplied = false;
		desc.DepthBufferSupplied = false;
		desc.GenerateMipMaps_Color = true;
		
		m_pRenderTarget->Create(desc);

		m_IsInitialized = true;
	}
}

bool PostProcessingMaterial::LoadEffect(const GameContext& gameContext, const std::wstring& effectFile)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(effectFile);

	//TODO: complete
	//Load Effect through ContentManager
	m_pEffect = ContentManager::Load<ID3DX11Effect>(effectFile);
	//Check if m_TechniqueName (default constructor parameter) is set
	// If SET > Use this Technique (+ check if valid)
	// If !SET > Use Technique with index 0
	if(!m_TechniqueName.empty()) {
		m_pTechnique = m_pEffect->GetTechniqueByName(LPCSTR(m_TechniqueName.c_str()));
		if (!m_pTechnique->IsValid()) {
			Logger::LogWarning(L"technique not found");
			m_pTechnique = nullptr;
		}
	}
	else {
		m_pTechnique = m_pEffect->GetTechniqueByIndex(0);
	}
	if (!m_pTechnique->IsValid()) {
		Logger::LogWarning(L"technique not found");
		m_pTechnique = nullptr;
		return false;
	}

	//Call LoadEffectVariables
	LoadEffectVariables();
	return true;
}

void PostProcessingMaterial::Draw(const GameContext& gameContext, RenderTarget* previousRendertarget)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(previousRendertarget);

	//TODO: complete
	//1. Clear the object's RenderTarget (m_pRenderTarget) [Check RenderTarget Class]
	m_pRenderTarget->Clear(gameContext, DirectX::Colors::Black);
	//2. Call UpdateEffectVariables(...)
	UpdateEffectVariables(previousRendertarget);
	//3. Set InputLayout
	gameContext.pDeviceContext->IASetInputLayout(m_pInputLayout);
	//4. Set VertexBuffer
	UINT offset{0};
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_VertexBufferStride, &offset);

	gameContext.pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//5. Set PrimitiveTopology (TRIANGLELIST)
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//6. Draw 
	D3DX11_TECHNIQUE_DESC techDesc{};
	m_pTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pTechnique->GetPassByIndex(p)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
	}
	// Generate Mips
	gameContext.pDeviceContext->GenerateMips(m_pRenderTarget->GetShaderResourceView());
}

void PostProcessingMaterial::CreateVertexBuffer(const GameContext& gameContext)
{
	m_NumVertices = 4;
	VertexPosTex vertices[]{
	{{-1.f,1.0f,0.0f},{0,0}},
	{{-1.f,-1.0f,0.0f },{0,1}},
	{{1.0f,1.0f,0.0f},{1,0}},
	{{1.0f,-1.0f,0.0f},{1,1}} };

	//TODO: complete

	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(VertexPosTex) * m_NumVertices;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = vertices;
	gameContext.pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);

	//D3D11_BUFFER_DESC buffDesc;
	//buffDesc.Usage = D3D11_USAGE_IMMUTABLE;
	//buffDesc.ByteWidth = sizeof(VertexPosTex) * m_NumVertices;
	//buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//buffDesc.CPUAccessFlags = 0;
	//buffDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	//
	//D3D11_SUBRESOURCE_DATA* data = new D3D11_SUBRESOURCE_DATA{nullptr, 0, 0};
	//gameContext.pDevice->CreateBuffer(&buffDesc, data , &m_pVertexBuffer);
}

void PostProcessingMaterial::CreateIndexBuffer(const GameContext& gameContext)
{
	m_NumIndices = 6;

	//TODO: complete
	// Create index buffer

	DWORD indices[6] = { 0, 2, 1 ,2,3,1};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(DWORD) * m_NumIndices;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = indices;
	gameContext.pDevice->CreateBuffer(&bd, &InitData, &m_pIndexBuffer);

	//D3D11_BUFFER_DESC buffDesc;
	//buffDesc.Usage = D3D11_USAGE_DYNAMIC;
	//buffDesc.ByteWidth = sizeof(VertexPosTex) * m_NumIndices;
	//buffDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//buffDesc.MiscFlags = 0;
	//gameContext.pDevice->CreateBuffer(&buffDesc, nullptr, &m_pIndexBuffer);

}
