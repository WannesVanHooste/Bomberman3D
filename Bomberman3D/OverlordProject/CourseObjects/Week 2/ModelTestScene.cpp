#include "stdafx.h"

#include "ModelTestScene.h"
#include "GameObject.h"
#include "Components.h"
#include "ContentManager.h"
#include "PhysxManager.h"
#include "MaterialManager.h"
#include "../OverlordProject/Materials/ColorMaterial.h"
#include "../OverlordProject/Materials/DiffuseMaterial.h"
#include "PhysxProxy.h"

ModelTestScene::ModelTestScene() :
GameScene(L"ModelTestScene"),
m_pChair(nullptr)
{}

void ModelTestScene::Initialize()
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

	//CHAIR OBJECT
	//************
	m_pChair = new GameObject();

	//1. Attach a modelcomponent (chair.ovm)
	ModelComponent* model = new ModelComponent{ L"Resources/Meshes/Chair.ovm" };
	m_pChair->AddComponent(model);
	//2. Create a ColorMaterial and add it to the material manager
	ColorMaterial* material = new ColorMaterial{};
	gameContext.pMaterialManager->AddMaterial(material, 0);
	//3. Assign the material to the previous created modelcomponent
	// Build and Run
	model->SetMaterial(0);

	//4. Create a DiffuseMaterial (using PosNormTex3D.fx)
	DiffuseMaterial* diffuseMaterial = new DiffuseMaterial{};
	//		Make sure you are able to set a texture (DiffuseMaterial::SetDiffuseTexture(const wstring& assetFile))
	diffuseMaterial->SetDiffuseTexture(L"Resources/Textures/Chair_Dark.dds");
	//		Load the correct shadervariable and set it during the material variable update
	gameContext.pMaterialManager->AddMaterial(diffuseMaterial, 1);
	model->SetMaterial(1);

	//5. Assign the material to the modelcomponent
	// Build and Run

	//6. Attach a rigidbody component (pure-dynamic)
    m_pChair->AddComponent(new RigidBodyComponent());
	//7. Attach a collider component (Use a PxConvexMeshGeometry [chair.ovpc])
	std::shared_ptr<physx::PxGeometry> geometry = std::make_shared<physx::PxConvexMeshGeometry>(ContentManager::Load<physx::PxConvexMesh>(L"Resources/Meshes/Chair.ovpc"));
	

	ColliderComponent* collider = new ColliderComponent{geometry, *pBouncyMaterial};

	m_pChair->AddComponent(collider);

	// Build and Run
	AddChild(m_pChair);
	m_pChair->GetTransform()->Translate( 0,5,0 );
	
}

void ModelTestScene::Update()
{}

void ModelTestScene::Draw()
{}
