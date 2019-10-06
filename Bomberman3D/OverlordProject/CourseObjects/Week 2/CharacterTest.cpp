#include "stdafx.h"
#include "CharacterTest.h"
#include "GameObject.h"
#include "Components.h"
#include "ContentManager.h"
#include "PhysxManager.h"
#include "MaterialManager.h"
#include "../OverlordProject/Materials/ColorMaterial.h"
#include "../OverlordProject/Materials/DiffuseMaterial.h"
#include "PhysxProxy.h"
#include "SpherePrefab.h"
#include "ControllerComponent.h"
#include "Character.h"

CharacterTest::CharacterTest()
	:GameScene{L"CharacterTest"}
{
}

void CharacterTest::Initialize()
{
	const auto gameContext = GetGameContext();

	//GROUND PLANE
	//************
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	GetPhysxProxy()->EnablePhysxDebugRendering(true);

	auto pBouncyMaterial = physX->createMaterial(0, 0, 0);
	auto pGround = new GameObject();
	pGround->AddComponent(new RigidBodyComponent(true));

	std::shared_ptr<physx::PxGeometry> geom(new physx::PxPlaneGeometry());
	pGround->AddComponent(new ColliderComponent(geom, *pBouncyMaterial, physx::PxTransform(physx::PxQuat(DirectX::XM_PIDIV2, physx::PxVec3(0, 0, 1)))));
	AddChild(pGround);

	////Controller Test
	//auto m_pSphere = new SpherePrefab{ 1 };
	//auto controllerComp = new ControllerComponent{ pBouncyMaterial };
	//m_pSphere->AddComponent(controllerComp);
	//AddChild(m_pSphere);

	//Character test
	auto level = new GameObject();
	ModelComponent* model = new ModelComponent{ L"Resources/Meshes/SimpleLevel.ovm" };
	level->AddComponent(model);

	ColorMaterial* material = new ColorMaterial{};

	gameContext.pMaterialManager->AddMaterial(material, 0);
	model->SetMaterial(0);

	level->AddComponent(new RigidBodyComponent(true));

	std::shared_ptr<physx::PxGeometry> geometry = std::make_shared<physx::PxTriangleMeshGeometry>(ContentManager::Load<physx::PxTriangleMesh>(L"Resources/Meshes/SimpleLevel.ovpt"));


	ColliderComponent* collider = new ColliderComponent{ geometry, *pBouncyMaterial };
	level->AddComponent(collider);

	AddChild(level);

	auto character = new Character{};
	AddChild(character);

	character->GetTransform()->Translate(0, 5.f, 0);
}

void CharacterTest::Update()
{
}

void CharacterTest::Draw()
{
}
