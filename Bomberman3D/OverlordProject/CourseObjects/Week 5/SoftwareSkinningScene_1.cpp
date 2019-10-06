#include "stdafx.h"
#include "SoftwareSkinningScene_1.h"
#include "PhysxProxy.h"
#include "GameObject.h"
#include "../OverlordProject/Materials/ColorMaterial.h"
#include "BoneObject.h"
#include "Components.h"

SoftwareSkinningScene_1::SoftwareSkinningScene_1() : GameScene(L"SoftwareSkinningScene_1") {
	m_pBone0 = nullptr;
	m_pBone1 = nullptr;
	m_BoneRotation = 0;
	m_RotationSign = 1;
}

void SoftwareSkinningScene_1::Initialize()
{
	GetPhysxProxy()->EnablePhysxDebugRendering(true);
	GetGameContext().pGameTime->ForceElapsedUpperbound(true);

	
	ColorMaterial* material = new ColorMaterial{};
	GetGameContext().pMaterialManager->AddMaterial(material, 0);
	
	auto root = new GameObject{};
	m_pBone0 = new BoneObject{ 0, 0, 15.0f };
	m_pBone1 = new BoneObject{ 1, 0, 15.0f };
	root->AddChild(m_pBone0);
	m_pBone0->AddBone(m_pBone1);

	root->GetTransform()->Rotate(0, -90, 0);

	AddChild(root);
}

void SoftwareSkinningScene_1::Update()
{
	const auto gameContext = GetGameContext();
	m_BoneRotation += m_RotationSign * 45 * gameContext.pGameTime->GetElapsed();

	if (m_RotationSign < 0 && m_BoneRotation <= -45) {
		m_RotationSign = 1;
	}
	if (m_RotationSign > 0 && m_BoneRotation >= 45) {
		m_RotationSign = -1;
	}

	m_pBone0->GetTransform()->Rotate(m_BoneRotation, 0, 0);
	m_pBone1->GetTransform()->Rotate(-m_BoneRotation * 2,0,0);
}

void SoftwareSkinningScene_1::Draw() {}

