#include "stdafx.h"

#include "TerrainTestScene.h"
#include "TerrainComponent.h"
#include "GameObject.h"
#include <PhysxManager.h>
#include <PhysxProxy.h>
#include "Prefabs.h"
#include "Components.h"

TerrainTestScene::TerrainTestScene() :
GameScene(L"TerrainTestScene")
{}

void TerrainTestScene::Initialize()
{
	const auto gameContext = GetGameContext();

	auto physX = PhysxManager::GetInstance()->GetPhysics();
	auto bouncyMaterial = physX->createMaterial(0, 0, 1);
	GetPhysxProxy()->EnablePhysxDebugRendering(true);
	auto pTerrainCmp = new TerrainComponent(L"Resources/Terrain/Hawai_HeightMap_64x64x16.raw", L"Resources/Terrain/Hawai_TexMap.dds", 64, 64,100.0f,100.0f,20.0f);
	auto pObj = new GameObject();
	pObj->AddComponent(pTerrainCmp);
	AddChild(pObj);

	// ADD SPHERE
	auto m_pSphere = new SpherePrefab();
	m_pSphere->GetTransform()->Translate(0, 50, 0);

	// Sphere PhysX
	auto rigidbody = new RigidBodyComponent();
	m_pSphere->AddComponent(rigidbody);

	std::shared_ptr<physx::PxGeometry> spheregeom(new physx::PxSphereGeometry(1));
	m_pSphere->AddComponent(new ColliderComponent(spheregeom, *bouncyMaterial, physx::PxTransform(physx::PxQuat(DirectX::XM_PIDIV2, physx::PxVec3(0, 0, 1)))));

	AddChild(m_pSphere);
}

void TerrainTestScene::Update()
{}

void TerrainTestScene::Draw()
{}
