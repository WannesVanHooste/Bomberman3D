#include "stdafx.h"
#include "BomberMan3DControls.h"
#include "OverlordGame.h"
#include "Button.h"
#include "SceneManager.h"
#include "TransformComponent.h"

BomberMan3DControls::BomberMan3DControls()
	:GameScene{L"ControlsScene"}
{
}

void BomberMan3DControls::Initialize() {
	auto activateButton = InputAction(13, InputTriggerState::Released, VK_SPACE, -1, XINPUT_GAMEPAD_A);
	GetGameContext().pInput->AddInputAction(activateButton);

	m_pBackButton = new Button{ []() {SceneManager::GetInstance()->SetActiveGameScene(L"MainMenu"); }, L"./Resources/Textures/Controls.png"};
	AddChild(m_pBackButton);
}

void BomberMan3DControls::Update() {
	if (GetGameContext().pInput->IsActionTriggered(13))
	{
		m_pBackButton->Activate();
	}
}

void BomberMan3DControls::Draw() {
	
}
