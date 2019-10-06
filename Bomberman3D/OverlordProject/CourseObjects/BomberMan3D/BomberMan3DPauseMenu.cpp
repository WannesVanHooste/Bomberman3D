#include "stdafx.h"
#include "BomberMan3DPauseMenu.h"
#include "PlayerLocator.h"
#include "OverlordGame.h"
#include "SceneManager.h"
#include "TransformComponent.h"

BomberMan3DPauseMenu::BomberMan3DPauseMenu()
	:GameScene(L"PauseScene")
{
	m_pButtonManager = nullptr;
}

void BomberMan3DPauseMenu::Initialize() {
	auto activateButton = InputAction(14, InputTriggerState::Released, VK_SPACE, -1, XINPUT_GAMEPAD_A);
	GetGameContext().pInput->AddInputAction(activateButton);

	auto nextButton = InputAction(15, InputTriggerState::Released, 'S', -1, XINPUT_GAMEPAD_DPAD_DOWN);
	GetGameContext().pInput->AddInputAction(nextButton);

	auto previousButton = InputAction(16, InputTriggerState::Released, 'W', -1, XINPUT_GAMEPAD_DPAD_UP);
	GetGameContext().pInput->AddInputAction(previousButton);

	m_pButtonManager = new ButtonManager{};
	auto resume = new Button{ []() {SceneManager::GetInstance()->SetActiveGameScene(L"BomberMan3DScene"); }, L"./Resources/Textures/PauseResume.png" };
	AddChild(resume);

	auto restart = new Button{ []() {PlayerLocator::GetMainScene()->RestartScene(); SceneManager::GetInstance()->SetActiveGameScene(L"BomberMan3DScene"); }, L"./Resources/Textures/PauseRestart.png" };
	AddChild(restart);

	auto toMenu = new Button{ []() {SceneManager::GetInstance()->SetActiveGameScene(L"MainMenu"); }, L"./Resources/Textures/PauseToMenu.png" };
	AddChild(toMenu);

	auto exit = new Button{ []() {SceneManager::GetInstance()->GetGame()->SetGameEnd(true); }, L"./Resources/Textures/PauseExit.png" };
	AddChild(exit);

	m_pButtonManager->AddButton(resume);
	m_pButtonManager->AddButton(restart);
	m_pButtonManager->AddButton(toMenu);
	m_pButtonManager->AddButton(exit);

	AddChild(m_pButtonManager);
}
void BomberMan3DPauseMenu::Update() {
	if (GetGameContext().pInput->IsActionTriggered(14))
	{
		m_pButtonManager->GetActiveButton()->Activate();
	}
	if (GetGameContext().pInput->IsActionTriggered(15))
	{
		m_pButtonManager->NextButton();
	}
	if (GetGameContext().pInput->IsActionTriggered(16))
	{
		m_pButtonManager->PreviousButton();
	}
}
void BomberMan3DPauseMenu::Draw() {}

