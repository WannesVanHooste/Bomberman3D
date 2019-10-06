#include "stdafx.h"
#include "BomberMan3DMainMenu.h"
#include "GameObject.h"
#include "SpriteComponent.h"
#include "Button.h"
#include "SceneManager.h"
#include "ButtonManager.h"
#include "PlayerLocator.h"
#include "TransformComponent.h"
#include "GameScene.h"
#include "OverlordGame.h"
#include "SoundManager.h"

BomberMan3DMainMenu::BomberMan3DMainMenu()
	:GameScene(L"MainMenu")
{
	m_pButtonManager = nullptr;
	m_pFMODSystem = nullptr;
}

void BomberMan3DMainMenu::Initialize() {

	auto activateButton = InputAction(10, InputTriggerState::Released, VK_SPACE, -1, XINPUT_GAMEPAD_A);
	GetGameContext().pInput->AddInputAction(activateButton);

	auto nextButton = InputAction(11, InputTriggerState::Released, 'S', -1, XINPUT_GAMEPAD_DPAD_DOWN);
	GetGameContext().pInput->AddInputAction(nextButton);

	auto previousButton = InputAction(12, InputTriggerState::Released, 'W', -1, XINPUT_GAMEPAD_DPAD_UP);
	GetGameContext().pInput->AddInputAction(previousButton);

	m_pButtonManager = new ButtonManager{};
	auto play = new Button{ []() {PlayerLocator::GetMainScene()->RestartScene(); SceneManager::GetInstance()->SetActiveGameScene(L"BomberMan3DScene"); }, L"./Resources/Textures/MainPlay.png"};
	AddChild(play);

	auto controls = new Button{ []() {SceneManager::GetInstance()->SetActiveGameScene(L"ControlsScene"); }, L"./Resources/Textures/MainControls.png" };
	AddChild(controls);

	auto exit = new Button{ []() {SceneManager::GetInstance()->GetGame()->SetGameEnd(true); }, L"./Resources/Textures/MainExit.png" };
	AddChild(exit);


	m_pButtonManager->AddButton(play);
	m_pButtonManager->AddButton(exit);
	m_pButtonManager->AddButton(controls);
	AddChild(m_pButtonManager);
	
}
void BomberMan3DMainMenu::Update() {
	if (GetGameContext().pInput->IsActionTriggered(10))
	{
		m_pButtonManager->GetActiveButton()->Activate();
	}
	if (GetGameContext().pInput->IsActionTriggered(12))
	{
		m_pButtonManager->NextButton();
	}
	if (GetGameContext().pInput->IsActionTriggered(11))
	{
		m_pButtonManager->PreviousButton();
	}
}
void BomberMan3DMainMenu::Draw() {
}
