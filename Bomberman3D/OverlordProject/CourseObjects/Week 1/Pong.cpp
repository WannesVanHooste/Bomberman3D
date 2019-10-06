#include "stdafx.h"
#include "Pong.h"
#include "PhysxManager.h"
#include "GameObject.h"
#include "Components.h"
#include "PhysxProxy.h"
#include "Prefabs.h"
#include "SpherePrefab.h"
#include "CubePrefab.h"

Pong::Pong()
	:GameScene{L"Pong"}
{
}

void Pong::Initialize()
{
	const auto gameContext = GetGameContext();
	GetPhysxProxy()->EnablePhysxDebugRendering(true);

	//INPUTS
	auto inputAction = InputAction(0, InputTriggerState::Down, 'Y');
	gameContext.pInput->AddInputAction(inputAction);

	auto inputAction1 = InputAction(1, InputTriggerState::Down, 'H');
	gameContext.pInput->AddInputAction(inputAction1);

	auto inputAction2 = InputAction(2, InputTriggerState::Down, 'O');
	gameContext.pInput->AddInputAction(inputAction2);

	auto inputAction3 = InputAction(3, InputTriggerState::Down, 'L');
	gameContext.pInput->AddInputAction(inputAction3);

	auto inputAction4 = InputAction(4, InputTriggerState::Down, VK_SPACE);
	gameContext.pInput->AddInputAction(inputAction4);

	auto inputAction5 = InputAction(5, InputTriggerState::Down, 'R');
	gameContext.pInput->AddInputAction(inputAction5);
	//Physx
	auto pPhysx = PhysxManager::GetInstance()->GetPhysics();
	auto pBouncyMaterial = pPhysx->createMaterial(0.5f, 0.5f, 1.0f);

	//GROUND
	auto pGround = new GameObject();
	pGround->AddComponent(new RigidBodyComponent(true));

	std::shared_ptr<physx::PxGeometry> pGeometry(new physx::PxPlaneGeometry());
	pGround->AddComponent(new ColliderComponent(pGeometry, *pBouncyMaterial, physx::PxTransform(physx::PxQuat(DirectX::XM_PIDIV2, physx::PxVec3(0, 0, 1)))));

	AddChild(pGround);

	//PROJECTILE
	m_pProjectile = new SpherePrefab(0.5f, 10);

	auto pRigidBody = new RigidBodyComponent();
//	pRigidBody->SetCollisionGroup(CollisionGroupFlag::Group0);
	//pRigidBody->SetCollisionIgnoreGroups(static_cast<CollisionGroupFlag>(CollisionGroupFlag::Group1 | CollisionGroupFlag::Group2));
	m_pProjectile->AddComponent(pRigidBody);

	std::shared_ptr<physx::PxGeometry> pSphereGeometry(new physx::PxSphereGeometry(0.5f));
	m_pProjectile->AddComponent(new ColliderComponent(pSphereGeometry, *pBouncyMaterial));

	AddChild(m_pProjectile);

	auto joint = physx::PxD6JointCreate(*pPhysx, nullptr, physx::PxTransform(physx::PxVec3(0,1,0)), m_pProjectile->GetComponent<RigidBodyComponent>()->GetPxRigidBody(), physx::PxTransform(physx::PxVec3(0, 0, 0)));
	
	joint->setMotion(physx::PxD6Axis::eX, physx::PxD6Motion::eFREE);
    joint->setMotion(physx::PxD6Axis::eZ, physx::PxD6Motion::eFREE);

	//CUBE 1
	m_pCube1 = new CubePrefab(5,1,1);
	m_pCube1->GetTransform()->Translate(0, 1, -10);

	auto pRigidBodyCube = new RigidBodyComponent();
	pRigidBodyCube->SetConstraint(RigidBodyConstraintFlag::RotX, true);
	pRigidBodyCube->SetConstraint(RigidBodyConstraintFlag::RotY, true);
	pRigidBodyCube->SetConstraint(RigidBodyConstraintFlag::RotZ, true);
	pRigidBodyCube->SetConstraint(RigidBodyConstraintFlag::TransY, true);
	pRigidBodyCube->SetConstraint(RigidBodyConstraintFlag::TransZ, true);
	pRigidBodyCube->SetCollisionGroup(CollisionGroupFlag::Group0);
	pRigidBodyCube->SetCollisionIgnoreGroups(static_cast<CollisionGroupFlag>(CollisionGroupFlag::Group1 | CollisionGroupFlag::Group2));
	m_pCube1->AddComponent(pRigidBodyCube);

	std::shared_ptr<physx::PxGeometry> pBoxGeometry(new physx::PxBoxGeometry(2.5f, 0.5f, 0.5f));
	m_pCube1->AddComponent(new ColliderComponent(pBoxGeometry, *pBouncyMaterial));

	AddChild(m_pCube1);

	//CUBE 2
	m_pCube2 = new CubePrefab(5, 1, 1);
	m_pCube2->GetTransform()->Translate(0, 1, 10);

	auto pRigidBodyCube1 = new RigidBodyComponent();
	pRigidBodyCube1->SetConstraint(RigidBodyConstraintFlag::RotX, true);
	pRigidBodyCube1->SetConstraint(RigidBodyConstraintFlag::RotY, true);
	pRigidBodyCube1->SetConstraint(RigidBodyConstraintFlag::RotZ, true);
	pRigidBodyCube1->SetConstraint(RigidBodyConstraintFlag::TransY, true);
	pRigidBodyCube1->SetConstraint(RigidBodyConstraintFlag::TransZ, true);
	pRigidBodyCube1->SetCollisionGroup(CollisionGroupFlag::Group0);
	pRigidBodyCube1->SetCollisionIgnoreGroups(static_cast<CollisionGroupFlag>(CollisionGroupFlag::Group1 | CollisionGroupFlag::Group2));
	m_pCube2->AddComponent(pRigidBodyCube1);

	std::shared_ptr<physx::PxGeometry> pBoxGeometry1(new physx::PxBoxGeometry(2.5f, 0.5f, 0.5f));
	m_pCube2->AddComponent(new ColliderComponent(pBoxGeometry1, *pBouncyMaterial));

	AddChild(m_pCube2);

	//STOP 1
	auto pStop1 = new CubePrefab(1, 1, 22);
	pStop1->GetTransform()->Translate(-10, 1, 0);

	auto pRigidBodyCube2 = new RigidBodyComponent(true);
	pRigidBodyCube2->SetCollisionGroup(CollisionGroupFlag::Group0);
	pRigidBodyCube2->SetCollisionIgnoreGroups(static_cast<CollisionGroupFlag>(CollisionGroupFlag::Group1 | CollisionGroupFlag::Group2));
	pStop1->AddComponent(pRigidBodyCube2);

	std::shared_ptr<physx::PxGeometry> pBoxGeometry2(new physx::PxBoxGeometry(0.5f, 0.5f, 11));
	pStop1->AddComponent(new ColliderComponent(pBoxGeometry2, *pBouncyMaterial));

	AddChild(pStop1);

	//STOP 2
	auto pStop2 = new CubePrefab(1, 1, 22);
	pStop2->GetTransform()->Translate(10, 1, 0);

	auto pRigidBodyCube3 = new RigidBodyComponent(true);
	pRigidBodyCube3->SetCollisionGroup(CollisionGroupFlag::Group0);
	pRigidBodyCube3->SetCollisionIgnoreGroups(static_cast<CollisionGroupFlag>(CollisionGroupFlag::Group1 | CollisionGroupFlag::Group2));
	pStop2->AddComponent(pRigidBodyCube3);

	pStop2->AddComponent(new ColliderComponent(pBoxGeometry2, *pBouncyMaterial));

	AddChild(pStop2);

	auto camera = new FixedCamera();

	AddChild(camera);
	camera->GetComponent<CameraComponent>()->GetTransform()->Translate(0, 30, 0);
	camera->GetComponent<CameraComponent>()->GetTransform()->Rotate(90, 0, 0);

	GameScene::SetActiveCamera(camera->GetComponent<CameraComponent>());
 }

void Pong::Update()
{
	const auto gameContext = GetGameContext();
	if (gameContext.pInput->IsActionTriggered(0))
	{
		auto pRigidBody = m_pCube1->GetComponent<RigidBodyComponent>();
		pRigidBody->AddForce(physx::PxVec3(30, 0, 0));
	}
	else if (gameContext.pInput->IsActionTriggered(1))
	{
		auto pRigidBody = m_pCube1->GetComponent<RigidBodyComponent>();
		pRigidBody->AddForce(physx::PxVec3(-30, 0, 0));
	}
	else if (gameContext.pInput->IsActionTriggered(2))
	{
		auto pRigidBody1 = m_pCube2->GetComponent<RigidBodyComponent>();
		pRigidBody1->AddForce(physx::PxVec3(30, 0, 0));
	}
	else if (gameContext.pInput->IsActionTriggered(3))
	{
		auto pRigidBody1 = m_pCube2->GetComponent<RigidBodyComponent>();
		pRigidBody1->AddForce(physx::PxVec3(-30, 0, 0));
	}
	else {
		auto pRigidBody1 = m_pCube2->GetComponent<RigidBodyComponent>();
		pRigidBody1->PutToSleep();

		auto pRigidBody2 = m_pCube1->GetComponent<RigidBodyComponent>();
		pRigidBody2->PutToSleep();
	}
	if (gameContext.pInput->IsActionTriggered(4)) 
	{
		auto pRigidBody = m_pProjectile->GetComponent<RigidBodyComponent>()->GetPxRigidBody();
		pRigidBody->addForce({ 50,0,300 }, physx::PxForceMode::eFORCE, true);
	}
	if (gameContext.pInput->IsActionTriggered(5))
	{
		m_pProjectile->GetTransform()->Translate(0, 0, 0);
		auto pRigidBody = m_pProjectile->GetComponent<RigidBodyComponent>();
		pRigidBody->PutToSleep();
	}
}

void Pong::Draw()
{
}
