#include "stdafx.h"

#include "TerrainComponent.h"
 #include <utility>
#include "ContentManager.h"
#include "EffectHelper.h"
#include "TransformComponent.h"
#include "TextureData.h"
#include "PhysxManager.h"
#include "PhysxProxy.h"
#include "GameObject.h"
#include "GameScene.h"

TerrainComponent::TerrainComponent(std::wstring heightMapFile, std::wstring textureFile,
                                   unsigned int nrOfRows, unsigned int nrOfColumns, float width, float depth,
                                   float maxHeight) :
	m_HeightMapFile(std::move(heightMapFile)),
	m_TextureFile(std::move(textureFile)),
	m_NrOfRows(nrOfRows),
	m_NrOfColumns(nrOfColumns),
	m_NrOfVertices(nrOfRows * nrOfColumns),
	m_Width(width),
	m_Depth(depth),
	m_MaxHeight(maxHeight)
{}

TerrainComponent::~TerrainComponent()
{
	SafeRelease(m_pInputLayout);
	SafeRelease(m_pVertexBuffer);
	SafeRelease(m_pIndexBuffer);
}

void TerrainComponent::Initialize(const GameContext& gameContext)
{
	//Load Effect
	m_pEffect = ContentManager::Load<ID3DX11Effect>(L"Resources/Effects/PosNormTex3D.fx");
	m_pTechnique = m_pEffect->GetTechniqueByIndex(0);

	//TODO: Error handling
	m_pMatWorldViewProjVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	m_pMatWorldVariable = m_pEffect->GetVariableByName("gWorld")->AsMatrix();
	m_pDiffuseVariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();

	UINT ilSize = 0;
	if (!EffectHelper::BuildInputLayout(gameContext.pDevice, m_pTechnique, &m_pInputLayout, ilSize))
		Logger::LogError(L"TerrainComponent::Initialize >> BuildInputLayout failed!");

	//Texture
	m_pTextureData = ContentManager::Load<TextureData>(m_TextureFile);

	//Load Height Map
	ReadHeightMap();

	//Create Vertices & Triangles
	CalculateVerticesAndIndices();

	if (!m_VecVertices.empty())
	{
		//Build Vertexbuffer
		BuildVertexBuffer(gameContext);

		//Build Indexbuffer
		BuildIndexBuffer(gameContext);

		//Create PhysX Heightfield
		CreatePxHeightField();
	}
}

void TerrainComponent::Update(const GameContext& )
{}

void TerrainComponent::Draw(const GameContext& gameContext)
{
	if (m_VecVertices.empty())
		return;

	DirectX::XMMATRIX world = XMLoadFloat4x4(&GetTransform()->GetWorld());
	const DirectX::XMMATRIX viewProj = XMLoadFloat4x4(&gameContext.pCamera->GetViewProjection());
	DirectX::XMMATRIX wvp = XMMatrixMultiply(world, viewProj);
	m_pMatWorldVariable->SetMatrix(reinterpret_cast<float*>(&world));
	m_pMatWorldViewProjVariable->SetMatrix(reinterpret_cast<float*>(&wvp));
	m_pDiffuseVariable->SetResource(m_pTextureData->GetShaderResourceView());

	// Set vertex buffer
	const unsigned int stride = sizeof(VertexPosNormTex);
	unsigned int offset = 0;
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// Set index buffer
	gameContext.pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the input layout
	gameContext.pDeviceContext->IASetInputLayout(m_pInputLayout);

	// Set primitive topology
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Render a cube
	D3DX11_TECHNIQUE_DESC techDesc;
	m_pTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pTechnique->GetPassByIndex(p)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
	}
}

//Exercise - Heightmap
void TerrainComponent::ReadHeightMap()
{
	//TODO: complete
	m_VecHeightValues.resize(m_NrOfVertices, 0);

	std::ifstream file;
	file.open(m_HeightMapFile.c_str(), std::ios_base::binary);
	if(!file) {
		Logger::LogFormat(LogLevel::Error, L"Loading terrain failed", m_HeightMapFile.c_str());
		return;
	}

	file.read(reinterpret_cast<char*>(&m_VecHeightValues[0]), static_cast<std::streamsize>(m_NrOfVertices * sizeof(unsigned short)));
	file.close();
}

//Exercise - Flat Grid
void TerrainComponent::CalculateVerticesAndIndices()
{
	using namespace DirectX;

	//TODO: complete
	//**VERTICES
	//Reserve spots in the buffer
	for(unsigned int i{}; i < m_NrOfVertices; ++i) {
		m_VecVertices.push_back(VertexPosNormTex{});
	}

	//Calculate the Initial Position (Terrain centered on the origin)
	//Reset the cellXPos Position for each Column
	float cellWidth = m_Width / m_NrOfColumns;
	float cellDepth = m_Depth / m_NrOfRows;
	float cellXPos = -m_Width / 2.0f;
	float cellZPos = m_Depth / 2.0f;
	
	for(unsigned int row = 0; row < m_NrOfRows; ++row) {
		cellXPos = -m_Width / 2.0f;
		for(unsigned int col = 0; col < m_NrOfColumns; ++col) {
			int vertexId = row * m_NrOfColumns + col;
			m_VecVertices[vertexId].Position.x = cellXPos;
			m_VecVertices[vertexId].Position.y = (m_VecHeightValues[vertexId] / 65535.0f) * m_MaxHeight;
			m_VecVertices[vertexId].Position.z = cellZPos;
			m_VecVertices[vertexId].TexCoord.x = float(col) / (m_NrOfColumns - 1);
			m_VecVertices[vertexId].TexCoord.y = float(row) / (m_NrOfRows - 1);
			cellXPos += cellWidth;
		}

		cellZPos -= cellDepth;
	}

	int nrQuadsRow = m_NrOfRows - 1;
	int nrQuadsColumn = m_NrOfColumns - 1;

	for(int row = 0; row < nrQuadsRow; ++row) {
		for(int col = 0; col < nrQuadsColumn; ++col) {
			int a = row * m_NrOfColumns + col;
			int b = a + 1;
			int c = a + m_NrOfColumns;
			int d = c + 1;
			AddQuad(a, b, c, d);
		}
	}
	//1. Position -- Partially Exercise - Heightmap --
	//2. Normal
	//3. TexCoord -- Exercise - UV --
	
	//Move the cellXPos Position (Down)
	//Move the cellZPos Position (Right)

	//Exercise - Normals
	//For each face...
	//Get the positions of 6 vertices
	//first triangle
	//second triangle

	//iterate through the vertices and calculate a normal for each one of them using the average of the 6 adjacent faces

	//from left front to right back
	//if col==0 is on left edge, there are 
	//no vertices on the left, fill in a illegal index

	//if col==m_NumColumns-1 is on right edge, there are 
	//no vertices on the right, fill in a illegal index

	//if index<0 or out of range: front or back edge 
	//it may not be used to calculate the average

	//calculate average by normalizing
	std::vector<XMFLOAT3> faceNormals{};
	for(unsigned int i{0}; i < m_VecIndices.size(); i+= 6) {
		XMVECTOR a{}, b{}, c{}, d{}, e{}, f{};
		a = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i)).Position);
		b = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i + 1)).Position);
		c = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i + 2)).Position);
		d = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i + 3)).Position);
		e = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i + 4)).Position);
		f = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i + 5)).Position);

		XMVECTOR v1{}, v2{}, normal{};
		//first triangle
		v1 = c - a;
		v2 = b - a;
		normal = XMVector3Cross(v1, v2);
		normal = XMVector3Normalize(normal);
		XMFLOAT3 normalFloat3{};
		XMStoreFloat3(&normalFloat3, normal);
		faceNormals.push_back(normalFloat3);

		//second triangle
		v1 = e - f;
		v2 = d - f;
		normal = XMVector3Cross(v1, v2);
		normal = XMVector3Normalize(normal);
		XMStoreFloat3(&normalFloat3, normal);
		faceNormals.push_back(normalFloat3);
	}

	int numFacesPerRow = (m_NrOfColumns - 1) * 2;
	int index[6];
	for(unsigned int row = 0; row < m_NrOfRows; ++row) {
		for(unsigned int col = 0; col < m_NrOfColumns; ++col) {
			int centerIndex = numFacesPerRow * row + col * 2;
			index[0] = centerIndex - 1;
			index[1] = centerIndex;
			index[2] = centerIndex + 1;
			index[3] = centerIndex - numFacesPerRow;
			index[4] = centerIndex - (numFacesPerRow - 1);
			index[5] = centerIndex - (numFacesPerRow - 2);

			if(col == 0) {
				index[0] = -1;
				index[3] = -1;
				index[4] = -1;
			}
			if(col == (m_NrOfColumns - 1)) {
				index[1] = -1;
				index[2] = -1;
				index[5] = -1;
			}
			XMVECTOR sum{};
			for (int i = 0; i < 6; ++i) {
				if(index[i] >= 0 && (index[i] < int(faceNormals.size()))) {
					sum += XMLoadFloat3(&faceNormals.at(index[i]));
				}
			}
			XMVector3Normalize(sum);
			XMFLOAT3 normal{};
			XMStoreFloat3(&normal, sum);
			int vertexId = row * m_NrOfColumns + col;
			m_VecVertices[vertexId].Normal = normal;
		}
	}

}

//Exercise - Flat Grid
void TerrainComponent::AddTriangle(const unsigned int a, const unsigned int b, const unsigned c)
{
	UNREFERENCED_PARAMETER(a);
	UNREFERENCED_PARAMETER(b);
	UNREFERENCED_PARAMETER(c);
	//TODO: complete
	m_VecIndices.push_back(a);
	m_VecIndices.push_back(b);
	m_VecIndices.push_back(c);
}

//Exercise - Flat Grid
void TerrainComponent::AddQuad(const unsigned int a, const unsigned int b, const unsigned c, const unsigned d)
{
	UNREFERENCED_PARAMETER(a);
	UNREFERENCED_PARAMETER(b);
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(d);
	//TODO: complete
	AddTriangle(a, d, c);
	AddTriangle(a, b, d);
}

//Exercise - PhysX
void TerrainComponent::CreatePxHeightField()
{
	//TODO: Implement PhysX HeightField
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	auto proxy = GetGameObject()->GetScene()->GetPhysxProxy();
	auto physxScene = proxy->GetPhysxScene();

	//After implementing the physX representation - Fix the lagginess from the PhysX Debugger! (If any)
	std::vector<physx::PxHeightFieldSample> hfSamples{};
	hfSamples.resize(m_NrOfVertices);
	for (UINT i = 0; i < m_VecHeightValues.size(); ++i)
	{
		hfSamples[i].height = physx::PxI16(((m_VecHeightValues[i] / 65535.0f) * m_MaxHeight));
	}

	physx::PxHeightFieldDesc hfDesc;
	hfDesc.format = physx::PxHeightFieldFormat::eS16_TM;
	hfDesc.nbColumns = m_NrOfColumns;
	hfDesc.nbRows = m_NrOfRows;
	hfDesc.samples.data = hfSamples.data();
	hfDesc.samples.stride = sizeof(physx::PxHeightFieldSample);


	physx::PxHeightField* aHeightField = physX->createHeightField(hfDesc);

	physx::PxHeightFieldGeometry hfGeom(aHeightField, physx::PxMeshGeometryFlags(), 1, m_Width / m_NrOfColumns, m_Depth / m_NrOfRows);
	auto bouncyMaterial = physX->createMaterial(.0f, .0f, .0f);
	physx::PxRigidActor* m_pActor = physX->createRigidStatic(physx::PxTransform(-m_Width / 2.0f, 0, m_Depth / 2.0f, physx::PxQuat(physx::PxPiDivTwo, physx::PxVec3(0, 1, 0))));
	physx::PxShape* pShape = m_pActor->createShape(hfGeom, *bouncyMaterial);
	m_pActor->userData = this;
	pShape->setSimulationFilterData(physx::PxFilterData(CollisionGroupFlag::Group0, 0, 0, 0));
	physxScene->addActor(*m_pActor);

}

void TerrainComponent::BuildVertexBuffer(const GameContext& gameContext)
{
	//Vertexbuffer
	D3D11_BUFFER_DESC bd = {};
	D3D11_SUBRESOURCE_DATA initData = {nullptr};
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(VertexPosNormTex) * m_NrOfVertices;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = m_VecVertices.data();
	const HRESULT hr = gameContext.pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);
	Logger::LogHResult(hr, L"Failed to Create Vertexbuffer");
}

void TerrainComponent::BuildIndexBuffer(const GameContext& gameContext)
{
	D3D11_BUFFER_DESC bd = {};
	D3D11_SUBRESOURCE_DATA initData = {nullptr};
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(DWORD) * m_VecIndices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = m_VecIndices.data();
	const HRESULT hr = gameContext.pDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
	Logger::LogHResult(hr, L"Failed to Create Indexbuffer");
	m_NumIndices = m_VecIndices.size();
}