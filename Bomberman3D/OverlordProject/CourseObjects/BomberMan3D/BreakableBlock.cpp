#include "stdafx.h"
#include "BreakableBlock.h"
#include "ContentManager.h"
#include "PhysxManager.h"
#include "RigidBodyComponent.h"
#include "ModelComponent.h"
#include "ColliderComponent.h"
#include "TransformComponent.h"

BreakableBlock::BreakableBlock() {
	
}

void BreakableBlock::ResetBlock() {
	GetTransform()->Translate(1000, 0, 0);
}

void BreakableBlock::Initialize(const GameContext&) {
	auto physX = PhysxManager::GetInstance()->GetPhysics();

	auto modelComp = new ModelComponent{ L"Resources/Meshes/Crate.ovm" };
	AddComponent(modelComp);
	modelComp->SetMaterial(1);
	AddComponent(new RigidBodyComponent());
	auto pBouncyMaterial = physX->createMaterial(0, 0, 0);
	std::shared_ptr<physx::PxGeometry> geometry = std::make_shared<physx::PxConvexMeshGeometry>(ContentManager::Load<physx::PxConvexMesh>(L"Resources/Meshes/Crate.ovpc"), physx::PxMeshScale{ physx::PxVec3{0.3f,0.3f,1.2f},physx::PxQuat{1} });
	ColliderComponent* collider = new ColliderComponent{ geometry, *pBouncyMaterial };
	AddComponent(collider);
}

void BreakableBlock::PostInitialize(const GameContext&) {}

void BreakableBlock::Update(const GameContext&) {}

