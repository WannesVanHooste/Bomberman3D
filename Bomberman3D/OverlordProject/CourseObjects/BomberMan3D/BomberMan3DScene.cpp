#include "stdafx.h"
#include "BomberMan3DScene.h"
#include "PhysxManager.h"
#include "PhysxProxy.h"
#include "RigidBodyComponent.h"
#include "GameObject.h"
#include "ColliderComponent.h"
#include "ContentManager.h"
#include "TransformComponent.h"
#include "FixedCamera.h"
#include "BomberMan3DCharacter.h"
#include "BomberMan3DLevel.h"
#include "Bomb.h"
#include "PlayerLocator.h"
#include "SceneManager.h"
#include "../../Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "../../Materials/Shadow/SkinnedDiffuseMaterial_Shadow.h"
#include "TextRenderer.h"
#include "SpriteFontLoader.h"
#include "Timer.h"
#include "ModelComponent.h"
#include <iomanip>
#include "PostInvert.h"
#include "PostChromaticAberration.h"

BomberMan3DScene::BomberMan3DScene()
	:GameScene(L"BomberMan3DScene")
{
	m_pGround = nullptr;
	m_pGroundPlane = nullptr;
	m_pLevel = nullptr;
	m_pPlayer1 = nullptr;
	m_pPlayer2 = nullptr;
	m_pTimer = nullptr;
	m_pTimerSprite = nullptr;
	PlayerLocator::ProvideMainScene(this);
	m_pSpriteFont = nullptr;
}

BomberMan3DScene::~BomberMan3DScene() {
	if(!m_InvertedColors) {
		SafeDelete(m_pPostInvert);
	}
	if(!m_ChromicAberration) {
		SafeDelete(m_pPostChromatic);
	}
}

void BomberMan3DScene::RestartScene() {

	m_pPlayer1->FixCollisionOnRestart();
	m_pPlayer2->FixCollisionOnRestart();
	for(auto element: GetChildren()) {
		auto bomb = dynamic_cast<Bomb*>(element);
		if(bomb != nullptr) {
			RemoveChild(bomb);
		}
	}
	RemoveChild(m_pPlayer1);
	RemoveChild(m_pPlayer2);
	RemoveChild(m_pTimer);
	RemoveChild(m_pTimerSprite);
	m_pLevel->ResetBlocks();
	RemoveChild(m_pLevel);
	BomberMan3DLevel::ClearBlocks();

	////LEVEL TEST
	int cols{ 15 };
	int rows{ 15 };
	m_pLevel = new BomberMan3DLevel{ cols,rows };
	AddChild(m_pLevel);

	//Controller Test 2
	m_pPlayer1 = new BomberMan3DCharacter{ 3.7f,7,60,1,15,15 };
	m_pPlayer2= new BomberMan3DCharacter{ 3.7f,7,60,2,15,15 };
	AddChild(m_pPlayer1);
	AddChild(m_pPlayer2);
	m_pPlayer1->GetTransform()->Translate(6.f, 5.0f, 6.f);
	m_pPlayer2->GetTransform()->Translate(((cols - 1) * 5.0f) - 6.0f, 5.0f, ((rows - 1) * 5.0f) - 6.0f);

	PlayerLocator::ProvidePlayerOne(m_pPlayer1);
	PlayerLocator::ProvidePlayerTwo(m_pPlayer2);

	//timer
	m_pTimer = new Timer{};
	AddChild(m_pTimer);

	m_pTimerSprite = new GameObject{};
	auto sprite = new SpriteComponent{ L"./Resources/Textures/InGame.png" };
	m_pTimerSprite->AddComponent(sprite);
	AddChild(m_pTimerSprite);
	m_pTimerSprite->GetTransform()->Translate(0, 0, 0.95f);

}

void BomberMan3DScene::Initialize()
{
	m_pSpriteFont = ContentManager::Load<SpriteFont>(L"./Resources/SpriteFonts/Consolas_32.fnt");

	m_pPostChromatic = new PostChromaticAberration{};
	m_pPostInvert = new PostInvert{};
	//input
	auto pause = InputAction(17, InputTriggerState::Released, 'P', -1, XINPUT_GAMEPAD_START);
	GetGameContext().pInput->AddInputAction(pause);

	auto chromatic = InputAction(18, InputTriggerState::Released, 'C', -1);
	GetGameContext().pInput->AddInputAction(chromatic);

	auto invert = InputAction(19, InputTriggerState::Released, 'I', -1);
	GetGameContext().pInput->AddInputAction(invert);

	const auto gameContext = GetGameContext();

	//gameContext.pShadowMapper->SetLight({ -95.6139526f,66.1346436f,-41.1850471f }, { 0.740129888f, -0.597205281f, 0.309117377f });
	gameContext.pShadowMapper->SetLight({ -68.8f,62.7f,58.8f }, { 0.699f, -0.692f, 0.178f });

	//GROUND PLANE
	//************
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	//GetPhysxProxy()->EnablePhysxDebugRendering(true);

	auto pBouncyMaterial = physX->createMaterial(0, 0, 0);
	m_pGround = new GameObject();
	m_pGround->AddComponent(new RigidBodyComponent(true));

	std::shared_ptr<physx::PxGeometry> geom(new physx::PxPlaneGeometry());
	m_pGround->AddComponent(new ColliderComponent(geom, *pBouncyMaterial, physx::PxTransform(physx::PxQuat(DirectX::XM_PIDIV2, physx::PxVec3(0, 0, 1)))));
	AddChild(m_pGround);

	//CAMERA TEST
	auto fixedCam = new FixedCamera{};
	auto camera = new CameraComponent{};
	fixedCam->AddComponent(camera);
	AddChild(fixedCam);
	camera->GetTransform()->Translate(37.5f, 110, 7.5f);
	camera->GetTransform()->Rotate(75, 0, 0);
	SetActiveCamera(camera);

	//PlayerLocator::ProvideActiveCamera(camera);
	//SET MATERIALS
	auto skinnedDiffuseMaterial = new SkinnedDiffuseMaterial_Shadow();
	skinnedDiffuseMaterial->SetDiffuseTexture(L"./Resources/Textures/Boy01_diffuse.jpg");
	skinnedDiffuseMaterial->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());
	gameContext.pMaterialManager->AddMaterial(skinnedDiffuseMaterial, 0);

	DiffuseMaterial_Shadow* diffuseMaterial = new DiffuseMaterial_Shadow{};
	diffuseMaterial->SetDiffuseTexture(L"Resources/Textures/Wooden Crate_Crate_BaseColor.png");
	diffuseMaterial->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());
	gameContext.pMaterialManager->AddMaterial(diffuseMaterial, 1);
	
	diffuseMaterial = new DiffuseMaterial_Shadow{};
	diffuseMaterial->SetDiffuseTexture(L"Resources/Textures/Wooden Crate_Crate_BaseColor2.png");
	diffuseMaterial->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());
	gameContext.pMaterialManager->AddMaterial(diffuseMaterial, 2);

	diffuseMaterial = new DiffuseMaterial_Shadow{};
	diffuseMaterial->SetDiffuseTexture(L"Resources/Textures/bomb_Diffuse1.png");
	diffuseMaterial->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());
	gameContext.pMaterialManager->AddMaterial(diffuseMaterial, 3);

	diffuseMaterial = new DiffuseMaterial_Shadow{};
	diffuseMaterial->SetDiffuseTexture(L"Resources/Textures/bomb_Diffuse2.png");
	diffuseMaterial->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());
	gameContext.pMaterialManager->AddMaterial(diffuseMaterial, 4);

	diffuseMaterial = new DiffuseMaterial_Shadow{};
	diffuseMaterial->SetDiffuseTexture(L"Resources/Textures/Plus.png");
	diffuseMaterial->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());
	gameContext.pMaterialManager->AddMaterial(diffuseMaterial, 5);

	diffuseMaterial = new DiffuseMaterial_Shadow{};
	diffuseMaterial->SetDiffuseTexture(L"Resources/Textures/GroundTexture2.png");
	diffuseMaterial->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());
	gameContext.pMaterialManager->AddMaterial(diffuseMaterial, 6);

	//LEVEL TEST
	int cols{ 15 };
	int rows{ 15 };
	m_pLevel = new BomberMan3DLevel{ cols,rows };
	AddChild(m_pLevel);

	//Controller Test 2
	m_pPlayer1 = new BomberMan3DCharacter{ 3.7f,7,60,1,15,15 };
	m_pPlayer2 = new BomberMan3DCharacter{ 3.7f,7,60,2,15,15 };
	AddChild(m_pPlayer1);
	AddChild(m_pPlayer2);
	m_pPlayer1->GetTransform()->Translate(6.f, 8.0f, 6.f);
	m_pPlayer2->GetTransform()->Translate(((cols - 1) * 5.0f) - 6.0f, 8.0f, ((rows - 1) * 5.0f) - 6.0f);

	PlayerLocator::ProvidePlayerOne(m_pPlayer1);
	PlayerLocator::ProvidePlayerTwo(m_pPlayer2);

	//timer
	m_pTimer = new Timer{};
	AddChild(m_pTimer);

	m_pTimerSprite = new GameObject{};
	auto sprite = new SpriteComponent{ L"./Resources/Textures/Timer.png"};
	m_pTimerSprite->AddComponent(sprite);
	AddChild(m_pTimerSprite);
	m_pTimerSprite->GetTransform()->Translate(540, 10, 0);


	//ground plane
	m_pGroundPlane = new GameObject{};
	auto mesh = new ModelComponent{ L"./Resources/Meshes/GroundPlane.ovm" };
	mesh->SetMaterial(6);
	m_pGroundPlane->AddComponent(mesh);
	AddChild(m_pGroundPlane);
	m_pGroundPlane->GetTransform()->Rotate(90, 0, 0);
	m_pGroundPlane->GetTransform()->Translate(0,-5, 0);
	m_pGroundPlane->GetTransform()->Scale(0.4f, 0.4f, 0.4f);

	auto firstBlock = new GameObject{};
	auto model = new ModelComponent{ L"Resources/Meshes/Crate.ovm" };

	firstBlock->AddComponent(model);
	model->SetMaterial(2);
	AddChild(firstBlock);
	firstBlock->GetTransform()->Rotate(90, 0, 0);

	auto collision = new GameObject{};
	collision->AddComponent(new RigidBodyComponent(true));
	std::shared_ptr<physx::PxGeometry> geometry = std::make_shared<physx::PxConvexMeshGeometry>(ContentManager::Load<physx::PxConvexMesh>(L"Resources/Meshes/Crate.ovpc"), physx::PxMeshScale{ physx::PxVec3{1.2f,1.2f,1.2f},physx::PxQuat{1} });
	auto collider = new ColliderComponent{ geometry, *pBouncyMaterial };
	collision->AddComponent(collider);
	AddChild(collision);
}

void BomberMan3DScene::Update()
{
	m_pPlayer1->DropBomb(GetGameContext());
	m_pPlayer2->DropBomb(GetGameContext());

	if(m_pPlayer1->GetDead() || m_pPlayer2->GetDead()) {
		if(m_pPlayer1->GetDead()) {
			m_pPlayer2->PlayVictoryAnimation();
			
		}
		else {
			m_pPlayer1->PlayVictoryAnimation();
		}
		m_AccuTime += GetGameContext().pGameTime->GetElapsed();
		if (m_AccuTime >= m_EndTime)
		{
			m_AccuTime = 0.0f;
			SceneManager::GetInstance()->SetActiveGameScene(L"MainMenu");
		}
	}

	if (GetGameContext().pInput->IsActionTriggered(17))
	{
		SceneManager::GetInstance()->SetActiveGameScene(L"PauseScene");
	}

	if (GetGameContext().pInput->IsActionTriggered(18))
	{
		if(m_ChromicAberration) {
			RemovePostProcessingEffect(m_pPostChromatic);
			m_ChromicAberration = false;
		}
		else {
			AddPostProcessingEffect(m_pPostChromatic);
			m_ChromicAberration = true;
		}
	}

	if (GetGameContext().pInput->IsActionTriggered(19))
	{
		if (m_InvertedColors) {
			RemovePostProcessingEffect(m_pPostInvert);
			m_InvertedColors = false;
		}
		else {
			AddPostProcessingEffect(m_pPostInvert);
			m_InvertedColors = true;
		}
	}
}

void BomberMan3DScene::Draw() {
	//player 1 hud
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, L"Lives: " + std::to_wstring(m_pPlayer1->GetLifes()), DirectX::XMFLOAT2(155, 88), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Black));
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, std::to_wstring(m_pPlayer1->GetRange()), DirectX::XMFLOAT2(63, 335), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Black));
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, std::to_wstring(int(m_pPlayer1->GetMoveSpeed())), DirectX::XMFLOAT2(55, 485), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Black));
	float bombDelay = m_pPlayer1->GetBombPlaceDelay() * 10.0f;
	int bigNumber = int(bombDelay) / 10;
	int smallNumber = int(bombDelay) - (bigNumber * 10);
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, std::to_wstring(bigNumber) + L"." + std::to_wstring(smallNumber), DirectX::XMFLOAT2(50, 635), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Black));

	//player 2 hud
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, L"Lives: " + std::to_wstring(m_pPlayer2->GetLifes()), DirectX::XMFLOAT2(1005, 88), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Black));
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, std::to_wstring(m_pPlayer2->GetRange()), DirectX::XMFLOAT2(1203, 335), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Black));
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, std::to_wstring(int(m_pPlayer2->GetMoveSpeed())), DirectX::XMFLOAT2(1195, 485), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Black));
	float bombDelay2 = m_pPlayer2->GetBombPlaceDelay() * 10.0f;
	int bigNumber2 = int(bombDelay2) / 10;
	int smallNumber2 = int(bombDelay2) - (bigNumber2 * 10);
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, std::to_wstring(bigNumber2) + L"." + std::to_wstring(smallNumber2), DirectX::XMFLOAT2(1190, 635), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Black));
}

