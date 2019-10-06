#include "stdafx.h"
#include "SolidBlock.h"
#include "ContentManager.h"
#include "PhysxManager.h"
#include "RigidBodyComponent.h"
#include "ModelComponent.h"
#include "ColliderComponent.h"
#include "TransformComponent.h"

SolidBlock::SolidBlock()
{

}

void SolidBlock::ResetBlock() {
	GetTransform()->Translate(1000, 0, 0);
}

void SolidBlock::Initialize(const GameContext&) {
	auto physX = PhysxManager::GetInstance()->GetPhysics();

	m_pModel = new ModelComponent{ L"Resources/Meshes/Crate.ovm" };
	AddComponent(m_pModel);
	m_pModel->SetMaterial(2);
	AddComponent(new RigidBodyComponent());
	auto pBouncyMaterial = physX->createMaterial(0, 0, 0);
	std::shared_ptr<physx::PxGeometry> geometry = std::make_shared<physx::PxConvexMeshGeometry>(ContentManager::Load<physx::PxConvexMesh>(L"Resources/Meshes/Crate.ovpc"), physx::PxMeshScale{physx::PxVec3{0.35f,0.35f,1.2f},physx::PxQuat{1}});
	m_pCollider = new ColliderComponent{ geometry, *pBouncyMaterial };
	AddComponent(m_pCollider);
}

void SolidBlock::PostInitialize(const GameContext&) {}

void SolidBlock::Update(const GameContext&) {}

