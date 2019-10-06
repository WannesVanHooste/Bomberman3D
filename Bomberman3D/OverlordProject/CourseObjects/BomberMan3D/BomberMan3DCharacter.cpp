#include "stdafx.h"
#include "BomberMan3DCharacter.h"
#include "Components.h"
#include "Prefabs.h"
#include "GameScene.h"
#include "PhysxManager.h"
#include "PhysxProxy.h"
#include "directxmath.h"
#include "Bomb.h"
#include "ModelAnimator.h"
#include "BomberMan3DLevel.h"

BomberMan3DCharacter::BomberMan3DCharacter(float radius, float height, float moveSpeed, int playerID, int cols, int rows)
:m_Radius(radius)
,m_Height(height)
,m_MoveSpeed(moveSpeed)
,m_pController(nullptr)
//Running
,m_MaxRunVelocity(50.0f)
,m_TerminalVelocity(20)
,m_RunAccelerationTime(0.3f)
,m_RunAcceleration(m_MaxRunVelocity / m_RunAccelerationTime)
,m_RunVelocity(0)
,m_Velocity(0, 0, 0)
,m_PlayerID(playerID)
,m_BombPlaceDelay(3.0f)
,m_AccuTime(0.0f)
,m_Cols(cols)
,m_Rows(rows)
{
	
}

int BomberMan3DCharacter::GetBlockId() {
	int col = int(GetTransform()->GetPosition().x + 1) / 5;
	int row = int(GetTransform()->GetPosition().z + 1) / 5;
	m_BlockId = (row * m_Cols) + col;
	return m_BlockId;
}

DirectX::XMFLOAT3 BomberMan3DCharacter::GetPositionOnGrid() {
	int col = int(GetTransform()->GetPosition().x + 1) / 5;
	int row = int(GetTransform()->GetPosition().z + 1) / 5;
	m_BlockId = (row * m_Cols) + col;
	return BomberMan3DLevel::GetBlockInfo()[row * 15 + col].position;
}

void BomberMan3DCharacter::LoseLife() {
	--m_Lives;
	if (m_PlayerID == 1)
	{
		GetTransform()->Translate(6.f, 0, 6.f);
	}
	else {
		GetTransform()->Translate(((m_Cols - 1) * 5.0f) - 6.0f, 0, ((m_Rows - 1) * 5.0f) - 6.0f);
	}
}

void BomberMan3DCharacter::PlayVictoryAnimation() {
	if(m_pModel->GetAnimator()->GetClipName() != L"Victory") {
		m_pModel->GetAnimator()->SetAnimation(2);
	}
}

void BomberMan3DCharacter::DropBomb(const GameContext& gameContext) {
	if (m_PlayerID == 1)
	{
		if (gameContext.pInput->IsActionTriggered(CharacterMovement::DROPBOMB))
		{
			if (m_AccuTime <= 0.0f)
			{
				Bomb* bomb = nullptr;
				GetBlockId();
				bomb = new Bomb{ m_Range,5.0f,1, m_BlockId,m_Cols,m_Rows };
				GetScene()->AddChild(bomb);
				bomb->GetTransform()->Translate(GetPositionOnGrid());
				m_AccuTime = m_BombPlaceDelay;
			}
		}
	}
	else {
		if (gameContext.pInput->IsActionTriggered(CharacterMovement::DROPBOMB2))
		{
			if (m_AccuTime <= 0.0f)
			{
				Bomb* bomb = nullptr;
				GetBlockId();
				bomb = new Bomb{ m_Range,5.0f,2, m_BlockId,m_Cols,m_Rows };
				GetScene()->AddChild(bomb);
				bomb->GetTransform()->Translate(GetPositionOnGrid());
				
				m_AccuTime = m_BombPlaceDelay;
			}
		}
	}
}

void BomberMan3DCharacter::Initialize(const GameContext& gameContext) {
	//TODO: Create controller
	auto physX = PhysxManager::GetInstance()->GetPhysics();

	auto pBouncyMaterial = physX->createMaterial(0, 0, 0);

	m_pController = new ControllerComponent{ pBouncyMaterial, m_Radius, m_Height,L"Character", physx::PxCapsuleClimbingMode::eLAST };
	this->AddComponent(m_pController);

	m_pModel = new ModelComponent(L"./Resources/Meshes/AjAnimations2.ovm");
	m_pModel->SetMaterial(0);
	auto obj = new GameObject();
	obj->AddComponent(m_pModel);
	AddChild(obj);
	m_pModel->GetTransform()->Scale(0.05f, 0.05f, 0.05f);
	m_pModel->GetTransform()->Translate(0, -8.0f, 0);


	//TODO: Register all Input Actions
	if (m_PlayerID == 1)
	{
		auto left = InputAction(CharacterMovement::LEFT, InputTriggerState::Down, 'A', -1, XINPUT_GAMEPAD_DPAD_LEFT);
		gameContext.pInput->AddInputAction(left);

		auto right = InputAction(CharacterMovement::RIGHT, InputTriggerState::Down, 'D', -1, XINPUT_GAMEPAD_DPAD_RIGHT);
		gameContext.pInput->AddInputAction(right);

		auto forward = InputAction(CharacterMovement::FORWARD, InputTriggerState::Down, 'W', -1, XINPUT_GAMEPAD_DPAD_UP);
		gameContext.pInput->AddInputAction(forward);

		auto back = InputAction(CharacterMovement::BACKWARD, InputTriggerState::Down, 'S', -1, XINPUT_GAMEPAD_DPAD_DOWN);
		gameContext.pInput->AddInputAction(back);

		auto DropBomb = InputAction(CharacterMovement::DROPBOMB, InputTriggerState::Down, VK_SPACE, -1, XINPUT_GAMEPAD_A);
		gameContext.pInput->AddInputAction(DropBomb);
	}
	else {
		auto left = InputAction(CharacterMovement::LEFT2, InputTriggerState::Down, VK_LEFT, -1, XINPUT_GAMEPAD_DPAD_LEFT, GamepadIndex::PlayerTwo);
		gameContext.pInput->AddInputAction(left);

		auto right = InputAction(CharacterMovement::RIGHT2, InputTriggerState::Down, VK_RIGHT, -1, XINPUT_GAMEPAD_DPAD_RIGHT, GamepadIndex::PlayerTwo);
		gameContext.pInput->AddInputAction(right);

		auto forward = InputAction(CharacterMovement::FORWARD2, InputTriggerState::Down, VK_UP, -1, XINPUT_GAMEPAD_DPAD_UP, GamepadIndex::PlayerTwo);
		gameContext.pInput->AddInputAction(forward);

		auto back = InputAction(CharacterMovement::BACKWARD2, InputTriggerState::Down, VK_DOWN, -1, XINPUT_GAMEPAD_DPAD_DOWN, GamepadIndex::PlayerTwo);
		gameContext.pInput->AddInputAction(back);

		auto DropBomb = InputAction(CharacterMovement::DROPBOMB2, InputTriggerState::Down, VK_SHIFT, -1 , XINPUT_GAMEPAD_A);
		gameContext.pInput->AddInputAction(DropBomb);
	}
}

void BomberMan3DCharacter::PostInitialize(const GameContext&) {
	m_pModel->GetAnimator()->SetAnimation(1);
	m_pModel->GetAnimator()->Play();
}
void BomberMan3DCharacter::Update(const GameContext& gameContext) {
	UNREFERENCED_PARAMETER(gameContext);
	//TODO: Update the character (Camera rotation, Character Movement, Character Gravity)
	using namespace DirectX;
	//takes care of moving camera with mouse
	//auto look = DirectX::XMFLOAT2(0, 0);
	float elapsedTime = gameContext.pGameTime->GetElapsed();

	/*const auto mouseMove = InputManager::GetMouseMovement();
	look.x = static_cast<float>(mouseMove.x);
	look.y = static_cast<float>(mouseMove.y);

	if (look.x == 0 && look.y == 0)
	{
		look = InputManager::GetThumbstickPosition(false);
	}*/

	m_AccuTime -= gameContext.pGameTime->GetElapsed();

	auto move = DirectX::XMFLOAT2(0, 0);
	if (m_PlayerID == 1)
	{
		move.y = gameContext.pInput->IsActionTriggered(CharacterMovement::FORWARD) ? 1.0f : 0.0f;
		if (move.y == 0) move.y = -(gameContext.pInput->IsActionTriggered(CharacterMovement::BACKWARD) ? 1.0f : 0.0f);

		move.x = gameContext.pInput->IsActionTriggered(CharacterMovement::RIGHT) ? 1.0f : 0.0f;
		if (move.x == 0) move.x = -(gameContext.pInput->IsActionTriggered(CharacterMovement::LEFT) ? 1.0f : 0.0f);
	}
	else {
		move.y = gameContext.pInput->IsActionTriggered(CharacterMovement::FORWARD2) ? 1.0f : 0.0f;
		if (move.y == 0) move.y = -(gameContext.pInput->IsActionTriggered(CharacterMovement::BACKWARD2) ? 1.0f : 0.0f);

		move.x = gameContext.pInput->IsActionTriggered(CharacterMovement::RIGHT2) ? 1.0f : 0.0f;
		if (move.x == 0) move.x = -(gameContext.pInput->IsActionTriggered(CharacterMovement::LEFT2) ? 1.0f : 0.0f);
	}

	//wll optimize later when movement with joystick
	if(move.x == 0 && move.y == 1) {
		GetTransform()->Rotate(0, 180, 0);
	}
	if (move.x == 0 && move.y == -1) {
		GetTransform()->Rotate(0, 0, 0);
	}
	if (move.x == 1 && move.y == 0) {
		GetTransform()->Rotate(0, -90, 0);
	}
	if (move.x == -1 && move.y == 0) {
		GetTransform()->Rotate(0, 90, 0);
	}
	if(move.x == 1 && move.y == 1) {
		GetTransform()->Rotate(0, -135, 0);
	}
	if (move.x == 1 && move.y == -1) {
		GetTransform()->Rotate(0, -45, 0);
	}
	if (move.x == -1 && move.y == 1) {
		GetTransform()->Rotate(0, 135, 0);
	}
	if (move.x == -1 && move.y == -1) {
		GetTransform()->Rotate(0, 45, 0);
	}

	if (move.x != 0 || move.y != 0) // if moving
	{
		if (m_RunVelocity < m_MaxRunVelocity)
		{
			m_RunVelocity += (m_RunAcceleration * elapsedTime);
		}
		XMFLOAT3 movement{ move.x, 0 , move.y };
		auto direction = XMLoadFloat3(&movement);
		XMFLOAT3 newDirection{};
		XMStoreFloat3(&newDirection, (direction * m_RunVelocity));
		m_Velocity = newDirection;

		//update animation to running
		if(m_pModel->GetAnimator()->GetClipName() != L"Running" && m_pModel->GetAnimator()->GetClipName() != L"Dead" && m_pModel->GetAnimator()->GetClipName() != L"Victory") {
			m_pModel->GetAnimator()->SetAnimation(0);
		}
	}
	else // if not moving
	{
		m_Velocity.x = 0;
		m_Velocity.z = 0;

		//update animations to idle
		if (m_pModel->GetAnimator()->GetClipName() != L"Idle" && m_pModel->GetAnimator()->GetClipName() != L"Dead" && m_pModel->GetAnimator()->GetClipName() != L"Victory") {
			m_pModel->GetAnimator()->SetAnimation(1);
		}
	}

	auto temp = XMLoadFloat3(&m_Velocity);
	temp *= elapsedTime;
	XMFLOAT3 temp2{};
	XMStoreFloat3(&temp2, temp);
	m_pController->Move(temp2);

	if(m_Lives <= 0) {
		m_IsDead = true;
		if (m_pModel->GetAnimator()->GetClipName() != L"Dead")
		{
			m_pModel->GetAnimator()->SetAnimation(3);
		}
	}
}

void BomberMan3DCharacter::FixCollisionOnRestart() {
	GetTransform()->Translate(1000, 0, 0);
}
