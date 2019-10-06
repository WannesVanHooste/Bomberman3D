#include "stdafx.h"
#include "Timer.h"
#include "TextRenderer.h"
#include "ContentManager.h"
#include "SceneManager.h"
#include "SpriteFont.h"
#include "PlayerLocator.h"

void Timer::Initialize(const GameContext& ) {}
void Timer::PostInitialize(const GameContext& ) {}
void Timer::Update(const GameContext& gameContext) {
	m_Timer -= gameContext.pGameTime->GetElapsed();

	if(m_Timer <= 0) {
		SceneManager::GetInstance()->SetActiveGameScene(L"MainMenu");
	}
	int minutes = int(m_Timer) / 60;
	int seconds = int(m_Timer) - (minutes * 60);
	std::wstring time = std::to_wstring(minutes) + L" : " + std::to_wstring(seconds);
	if(seconds < 10) {
		time = std::to_wstring(minutes) + L" : 0" + std::to_wstring(seconds);
	}
	auto sprite = ContentManager::Load<SpriteFont>(L"./Resources/SpriteFonts/Consolas_32.fnt");
	if (dynamic_cast<BomberMan3DCharacter*>(PlayerLocator::GetPlayerOne())->GetDead())
	{
		TextRenderer::GetInstance()->DrawText(sprite, L"Player 2 won", DirectX::XMFLOAT2(550, 45), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Black));
	}
	else if (dynamic_cast<BomberMan3DCharacter*>(PlayerLocator::GetPlayerTwo())->GetDead())
	{
		TextRenderer::GetInstance()->DrawText(sprite, L"Player 1 won", DirectX::XMFLOAT2(550, 45), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Black));
	}
	else {
		TextRenderer::GetInstance()->DrawText(sprite, time, DirectX::XMFLOAT2(600, 45), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Black));
	}
}

