#include "stdafx.h"
#include "BoneObject.h"
#include "ModelComponent.h"
#include "TransformComponent.h"

BoneObject::BoneObject(int boneId, int materialId, float length)
:m_MaterialId(materialId)
,m_Length(length)
,m_BoneId(boneId){
	
}

void BoneObject::AddBone(BoneObject* pBone) {
	pBone->GetTransform()->Translate(0, 0, -m_Length);
	AddChild(pBone);
}

DirectX::XMFLOAT4X4 BoneObject::GetBindPose() {
	return m_BindPose;
}

void BoneObject::CalculateBindPose() {
	auto worldMatrix = DirectX::XMLoadFloat4x4(&this->GetTransform()->GetWorld());
	worldMatrix = DirectX::XMMatrixInverse(nullptr, worldMatrix);
	DirectX::XMStoreFloat4x4(&m_BindPose, worldMatrix);

	auto childs = GetChildren<BoneObject>();
	for (BoneObject* element : childs) {
		element->CalculateBindPose();
	}
}

void BoneObject::Initialize(const GameContext& gameContext) {
	auto boneObject = new GameObject();
	ModelComponent* model = new ModelComponent{ L"Resources/Meshes/Bone.ovm" };
    boneObject->AddComponent(model);

	//ColorMaterial* material = new ColorMaterial{};
	//gameContext.pMaterialManager->AddMaterial(material, m_MaterialId);
	model->SetMaterial(m_MaterialId);
	boneObject->GetTransform()->Scale(m_Length, m_Length, m_Length);
	AddChild(boneObject);

	UNREFERENCED_PARAMETER(gameContext);
}
