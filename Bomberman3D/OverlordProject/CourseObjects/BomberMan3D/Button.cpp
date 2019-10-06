#include "stdafx.h"
#include "Button.h"
//#include <unordered_map>
#include "SoundManager.h"
#include "TransformComponent.h"

Button::Button(void(*f)(), const std::wstring& normalTexture)
:m_Function(f)
,m_pFMODSystem{}
,m_pChannel{}
,m_pButtonSound{}
, m_NormalTexture(normalTexture)
{
}

void Button::Focus(bool isFocus) {
	if(isFocus) {
		m_pFMODSystem->playSound(m_pButtonSound, 0, false, &m_pChannel);
		GetTransform()->Translate(0, 0, 94.f);
	}
	else {
		GetTransform()->Translate(0, 0, 0);
	}
}

void Button::Activate() {
	m_pFMODSystem->playSound(m_pButtonSound, 0, false, &m_pChannel);
	m_Function();
}
void Button::Initialize(const GameContext& ) {
	//SOUNDS
	m_pFMODSystem = SoundManager::GetInstance()->GetSystem();

	m_pFMODSystem->createSound("./Resources/Sounds/Button.wav", FMOD_2D, 0, &m_pButtonSound);

	m_pButtonSound->setMode(FMOD_LOOP_OFF);

	m_pChannel->setVolume(0.5f);
	//

	auto sprite = new SpriteComponent{ m_NormalTexture };
	AddComponent(sprite);
}
void Button::PostInitialize(const GameContext& ) {
}
void Button::Update(const GameContext& ) {}
