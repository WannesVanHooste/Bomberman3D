
#include "stdafx.h"
#include "Character.h"
#include "Components.h"
#include "Prefabs.h"
#include "GameScene.h"
#include "PhysxManager.h"
#include "PhysxProxy.h"
#include "directxmath.h"


Character::Character(float radius, float height, float moveSpeed) : 
	m_Radius(radius),
	m_Height(height),
	m_MoveSpeed(moveSpeed),
	m_pCamera(nullptr),
	m_pController(nullptr),
	m_TotalPitch(0), 
	m_TotalYaw(0),
	m_RotationSpeed(90.f),
	//Running
	m_MaxRunVelocity(50.0f), 
	m_TerminalVelocity(20),
	m_Gravity(9.81f), 
	m_RunAccelerationTime(0.3f), 
	m_JumpAccelerationTime(0.8f), 
	m_RunAcceleration(m_MaxRunVelocity/m_RunAccelerationTime), 
	m_JumpAcceleration(m_Gravity/m_JumpAccelerationTime), 
	m_RunVelocity(0), 
	m_JumpVelocity(0),
	m_Velocity(0,0,0)
{}

void Character::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	//TODO: Create controller
	auto physX = PhysxManager::GetInstance()->GetPhysics();

	auto pBouncyMaterial = physX->createMaterial(0, 0, 0);

	m_pController = new ControllerComponent{pBouncyMaterial, m_Radius, m_Height};
	this->AddComponent(m_pController);

	//TODO: Add a fixed camera as child
	auto fixedCam = new FixedCamera{};
	m_pCamera = new CameraComponent{};
	fixedCam->AddComponent(m_pCamera);
	//m_pCamera = fixedCam->GetComponent<CameraComponent>();

	this->AddChild(fixedCam);
	//TODO: Register all Input Actions

	auto left = InputAction(CharacterMovement::LEFT, InputTriggerState::Down, 'A');
	gameContext.pInput->AddInputAction(left);

	auto right = InputAction(CharacterMovement::RIGHT, InputTriggerState::Down, 'D');
	gameContext.pInput->AddInputAction(right);

	auto forward = InputAction(CharacterMovement::FORWARD, InputTriggerState::Down, 'W');
	gameContext.pInput->AddInputAction(forward);

	auto back = InputAction(CharacterMovement::BACKWARD, InputTriggerState::Down, 'S');
	gameContext.pInput->AddInputAction(back);

	auto Jump = InputAction(CharacterMovement::JUMP, InputTriggerState::Down, VK_SPACE);
	gameContext.pInput->AddInputAction(Jump);
}

void Character::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	//TODO: Set the camera as active
	// We need to do this in the PostInitialize because child game objects only get initialized after the Initialize of the current object finishes
	GetScene()->SetActiveCamera(m_pCamera);
}

void Character::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	//TODO: Update the character (Camera rotation, Character Movement, Character Gravity)
	using namespace DirectX;

	//takes care of moving camera with mouse
	auto look = DirectX::XMFLOAT2(0, 0);

	float elapsedTime = gameContext.pGameTime->GetElapsed();

	const auto mouseMove = InputManager::GetMouseMovement();
	look.x = static_cast<float>(mouseMove.x);
	look.y = static_cast<float>(mouseMove.y);

	if (look.x == 0 && look.y == 0)
	{
		look = InputManager::GetThumbstickPosition(false);
	}

	m_TotalYaw += look.x * m_RotationSpeed * elapsedTime;
	m_TotalPitch += look.y * m_RotationSpeed * elapsedTime;

	GetTransform()->Rotate(m_TotalPitch, m_TotalYaw, 0);

	
	auto move = DirectX::XMFLOAT2(0, 0);

	move.y = gameContext.pInput->IsActionTriggered(CharacterMovement::FORWARD) ? 1.0f : 0.0f;
	if (move.y == 0) move.y = -(gameContext.pInput->IsActionTriggered(CharacterMovement::BACKWARD) ? 1.0f : 0.0f);

	move.x = gameContext.pInput->IsActionTriggered(CharacterMovement::RIGHT) ? 1.0f : 0.0f;
	if (move.x == 0) move.x = -(gameContext.pInput->IsActionTriggered(CharacterMovement::LEFT) ? 1.0f : 0.0f);
	
	auto forward = XMLoadFloat3(&m_pCamera->GetTransform()->GetForward());
	auto right = XMLoadFloat3(&m_pCamera->GetTransform()->GetRight());

	auto direction = forward * move.y;
	direction += right * move.x;

	if (move.x != 0 || move.y != 0)
	{
		if (m_RunVelocity < m_MaxRunVelocity)
		{
			m_RunVelocity += (m_RunAcceleration * elapsedTime);
		}

		XMFLOAT3 newDirection{};
		XMStoreFloat3(&newDirection, (direction * m_RunVelocity));
		m_Velocity = newDirection;

		//m_Velocity.y = m_JumpVelocity;
	}
	else
	{
		m_Velocity.x = 0;
		m_Velocity.z = 0;
	}
	if (m_pController->GetCollisionFlags() != physx::PxControllerCollisionFlag::eCOLLISION_DOWN)
	{
		if (m_JumpAcceleration < m_TerminalVelocity)
		{
			m_JumpVelocity -= (m_JumpAcceleration * elapsedTime);
		}
	}
	else if (gameContext.pInput->IsActionTriggered(CharacterMovement::JUMP))
	{
		m_JumpVelocity = 0;
		m_Velocity.y = 200;
	}
	else {
		m_Velocity.y = 0;
	}
	m_Velocity.y += m_JumpVelocity;
	
	auto temp = XMLoadFloat3(&m_Velocity);
	temp *= elapsedTime;
	XMFLOAT3 temp2{};
	XMStoreFloat3(&temp2, temp);
	m_pController->Move(temp2);
}