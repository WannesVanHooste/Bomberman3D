#include "stdafx.h"
#include "SoftwareSkinningScene_2.h"
#include "PhysxProxy.h"
#include "GameObject.h"
#include "../OverlordProject/Materials/ColorMaterial.h"
#include "BoneObject.h"
#include "Components.h"

SoftwareSkinningScene_2::SoftwareSkinningScene_2() : GameScene(L"SoftwareSkinningScene_2") {
	m_pBone0 = nullptr;
	m_pBone1 = nullptr;
	m_BoneRotation = 0;
	m_RotationSign = 1;
}

void SoftwareSkinningScene_2::Initialize()
{
	GetPhysxProxy()->EnablePhysxDebugRendering(true);
	GetGameContext().pGameTime->ForceElapsedUpperbound(true);


	ColorMaterial* material = new ColorMaterial{true};
	GetGameContext().pMaterialManager->AddMaterial(material, 0);

	auto root = new GameObject{};
	m_pBone0 = new BoneObject{ 0, 0, 15.0f };
	m_pBone1 = new BoneObject{ 1, 0, 15.0f };
	root->AddChild(m_pBone0);
	m_pBone0->AddBone(m_pBone1);

	root->GetTransform()->Rotate(0, -90, 0);

	AddChild(root);

	auto empty = new GameObject{};
	m_pMeshDrawer = new MeshDrawComponent{ 24 };
	empty->AddComponent(m_pMeshDrawer);

	AddChild(empty);

	m_pBone0->CalculateBindPose();
	CreateMesh(15);

}

void SoftwareSkinningScene_2::Update()
{
	const auto gameContext = GetGameContext();

	auto bindposvar1 = m_pBone0->GetBindPose();
	DirectX::XMMATRIX bindPose1 = DirectX::XMLoadFloat4x4(&bindposvar1);
	auto world1 = DirectX::XMLoadFloat4x4(&m_pBone0->GetTransform()->GetWorld());
	auto boneTransform1 = DirectX::XMMatrixMultiply(bindPose1, world1);

	auto bindposvar2 = m_pBone1->GetBindPose();
	auto bindPose2 = DirectX::XMLoadFloat4x4(&bindposvar2);
	auto world2 = DirectX::XMLoadFloat4x4(&m_pBone1->GetTransform()->GetWorld());
	auto boneTransform2 = DirectX::XMMatrixMultiply(bindPose2, world2);

	for(unsigned int i{}; i < m_SkinnedVertices.size(); ++i) {
		auto pos = DirectX::XMLoadFloat3(&m_SkinnedVertices[i].OriginalVertex.Position);
		auto normal = DirectX::XMLoadFloat3(&m_SkinnedVertices[i].OriginalVertex.Normal);
		if(i < 24) {
			DirectX::XMStoreFloat3(&m_SkinnedVertices[i].TransformedVertex.Position, DirectX::XMVector3TransformCoord(pos, boneTransform1));
			DirectX::XMStoreFloat3(&m_SkinnedVertices[i].TransformedVertex.Normal, DirectX::XMVector3TransformNormal(normal, boneTransform1));
		}
		else {
			DirectX::XMStoreFloat3(&m_SkinnedVertices[i].TransformedVertex.Position, DirectX::XMVector3TransformCoord(pos, boneTransform2));
			DirectX::XMStoreFloat3(&m_SkinnedVertices[i].TransformedVertex.Normal, DirectX::XMVector3TransformNormal(normal, boneTransform2));
		}
	}

	m_BoneRotation += m_RotationSign * 45 * gameContext.pGameTime->GetElapsed();

	if (m_RotationSign < 0 && m_BoneRotation <= -45) {
		m_RotationSign = 1;
	}
	if (m_RotationSign > 0 && m_BoneRotation >= 45) {
		m_RotationSign = -1;
	}

	m_pBone0->GetTransform()->Rotate(m_BoneRotation, 0, 0);
	m_pBone1->GetTransform()->Rotate(-m_BoneRotation * 2, 0, 0);
}

void SoftwareSkinningScene_2::Draw() {
	m_pMeshDrawer->RemoveTriangles();
	for (unsigned int i{}; i < m_SkinnedVertices.size(); i += 4)
	{
		QuadPosNormCol quad{ m_SkinnedVertices[i].TransformedVertex, m_SkinnedVertices[i + 1].TransformedVertex, m_SkinnedVertices[i + 2].TransformedVertex, m_SkinnedVertices[i + 3].TransformedVertex };
		m_pMeshDrawer->AddQuad(quad, true);
	}
	m_pMeshDrawer->UpdateBuffer();
}


void SoftwareSkinningScene_2::CreateMesh(float length) {
	auto pos = DirectX::XMFLOAT3(length / 2.f, 0, 0);
	auto offset = DirectX::XMFLOAT3(length / 2.f, 2.5f, 2.5f);
	auto col = DirectX::XMFLOAT4(1, 0, 0, 0.5f);

	//*****  //BOX1*  //***** 

	//FRONT
	auto norm = DirectX::XMFLOAT3(0, 0, -1);
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col));
	//BACK
	norm = DirectX::XMFLOAT3(0, 0, 1);
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col));
	//TOP
	norm = DirectX::XMFLOAT3(0, 1, 0);
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col));
	//BOTTOM
	norm = DirectX::XMFLOAT3(0, -1, 0);
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col));
	//LEFT
	norm = DirectX::XMFLOAT3(-1, 0, 0);
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col));
	//RIGHT
	norm = DirectX::XMFLOAT3(1, 0, 0);
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); 

//*****  //BOX2*  //***** 

	col = DirectX::XMFLOAT4(0, 1, 0, 0.5f);
	pos = DirectX::XMFLOAT3(22.5f, 0, 0);
	//FRONT
	norm = DirectX::XMFLOAT3(0, 0, -1);
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col));
	//BACK
	norm = DirectX::XMFLOAT3(0, 0, 1);
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col));
	//TOP
	norm = DirectX::XMFLOAT3(0, 1, 0);
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col));
	//BOTTOM
	norm = DirectX::XMFLOAT3(0, -1, 0);
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col));
	//LEFT
	norm = DirectX::XMFLOAT3(-1, 0, 0);
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col));
	//RIGHT
	norm = DirectX::XMFLOAT3(1, 0, 0);
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(DirectX::XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col));
}
