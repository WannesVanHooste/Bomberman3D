#include "stdafx.h"
#include "ComponentTestScene.h"
#include "PhysxManager.h"
#include "GameObject.h"
#include "Components.h"
#include "PhysxProxy.h"
#include "Prefabs.h"
#include "SpherePrefab.h"

ComponentTestScene::ComponentTestScene()
	:GameScene(L"ComponentTestScene")
{

}

void ComponentTestScene::Initialize()
{
	const auto gameContext = GetGameContext();
	GetPhysxProxy()->EnablePhysxDebugRendering(true);

	//Physx
	auto pPhysx = PhysxManager::GetInstance()->GetPhysics();
	auto pBouncyMaterial = pPhysx->createMaterial(0.5f, 0.5f, 1.0f);

	//GROUND
	auto pGround = new GameObject();
	pGround->AddComponent(new RigidBodyComponent(true));

	std::shared_ptr<physx::PxGeometry> pGeometry(new physx::PxPlaneGeometry());
	pGround->AddComponent(new ColliderComponent(pGeometry, *pBouncyMaterial, physx::PxTransform(physx::PxQuat(DirectX::XM_PIDIV2, physx::PxVec3(0,0,1)))));

	AddChild(pGround);

	//SPHERE 1
	m_pSphere = new SpherePrefab();
	m_pSphere->GetTransform()->Translate(0, 30.0f, 0);

	auto pRigidBody = new RigidBodyComponent();
	pRigidBody->SetCollisionGroup(CollisionGroupFlag::Group0);
	pRigidBody->SetCollisionIgnoreGroups(static_cast<CollisionGroupFlag>(CollisionGroupFlag::Group1 | CollisionGroupFlag::Group2));
	m_pSphere->AddComponent(pRigidBody);

	std::shared_ptr<physx::PxGeometry> pSphereGeometry(new physx::PxSphereGeometry(1.0f));
	m_pSphere->AddComponent(new ColliderComponent(pSphereGeometry, *pBouncyMaterial));

	AddChild(m_pSphere);

	//SPHERE 2
	auto pSphere = new SpherePrefab();
	pSphere->GetTransform()->Translate(0, 20.0f, 0);

	pRigidBody = new RigidBodyComponent();
	pRigidBody->SetCollisionGroup(CollisionGroupFlag::Group1);
	pSphere->AddComponent(pRigidBody);

	pSphere->AddComponent(new ColliderComponent(pSphereGeometry, *pBouncyMaterial));

	AddChild(pSphere);

	//SPHERE 3
	pSphere = new SpherePrefab();
	pSphere->GetTransform()->Translate(0, 10.0f, 0);

	pRigidBody = new RigidBodyComponent();
	pRigidBody->SetCollisionGroup(CollisionGroupFlag::Group2);
	pSphere->AddComponent(pRigidBody);

	pSphere->AddComponent(new ColliderComponent(pSphereGeometry, *pBouncyMaterial));

	AddChild(pSphere);

	auto inputAction = InputAction(0, InputTriggerState::Down, 'M');
	gameContext.pInput->AddInputAction(inputAction);
}

void ComponentTestScene::Update()
{
	const auto gameContext = GetGameContext();
	if (gameContext.pInput->IsActionTriggered(0))
	{
		auto pRigidBody = m_pSphere->GetComponent<RigidBodyComponent>();
		pRigidBody->AddForce(physx::PxVec3(0, 10,0));
		//m_pSphere->GetTransform()->Translate(0, 10, 0);
	}
}

void ComponentTestScene::Draw()
{

}