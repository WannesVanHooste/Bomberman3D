#include "stdafx.h"
#include "TestUber.h"
#include "GameObject.h"

#include "Components.h"
#include "PhysxProxy.h"
#include "PhysxManager.h"
#include "ContentManager.h"
#include "../../Materials/UberMaterial.h"
#include "ModelComponent.h"


TestUber::TestUber()
	:GameScene(L"TestUber")
{
}


void TestUber::Initialize()
{
	const auto gameContext = GetGameContext();

	GetPhysxProxy()->EnablePhysxDebugRendering(true);
	gameContext.pGameTime->ForceElapsedUpperbound(true);

	// Create PhysX ground plane
	auto physX = PhysxManager::GetInstance()->GetPhysics();

	auto bouncyMaterial = physX->createMaterial(0, 0, 1);
	auto ground = new GameObject();
	ground->AddComponent(new RigidBodyComponent(true));

	std::shared_ptr<physx::PxGeometry> geom(new physx::PxPlaneGeometry());
	ground->AddComponent(new ColliderComponent(geom, *bouncyMaterial, physx::PxTransform(physx::PxQuat(DirectX::XM_PIDIV2, physx::PxVec3(0, 0, 1)))));
	AddChild(ground);


	//Material Test
	auto myMaterial = new UberMaterial();
	myMaterial->EnableDiffuseTexture(true);
	myMaterial->EnableEnvironmentMapping(true);
	myMaterial->EnableSpecularPhong(true);
	myMaterial->SetDiffuseTexture(L"./Resources/Textures/Chair_Dark.dds");
	myMaterial->SetEnvironmentCube(L"./Resources/Textures/Sunol_Cubemap.dds");
	myMaterial->SetReflectionStrength(0.8f);
	myMaterial->SetRefractionIndex(0.3f);
	myMaterial->SetRefractionStrength(0);
	gameContext.pMaterialManager->AddMaterial(myMaterial, 0);

	auto model = new ModelComponent(L"./Resources/Meshes/Teapot.ovm");
	model->SetMaterial(0);

	auto teapot = new GameObject();
	teapot->AddComponent(model);
	AddChild(teapot);
	teapot->GetTransform()->Translate(0, 0, 20);
}

void TestUber::Update()
{
	const auto gameContext = GetGameContext();
}

void TestUber::Draw()
{}
