#pragma once
#include "GameObject.h"
#include "SpriteComponent.h"

//class reworked and reused from Programming 4 exam project
class Button : public GameObject
{
public:
	Button(void(*f)(), const std::wstring& normalTexture);
	~Button() = default;

	Button(const Button& other) = delete;
	Button(Button&& other) noexcept = delete;
	Button& operator=(const Button& other) = delete;
	Button& operator=(Button&& other) noexcept = delete;

	void Focus(bool isFocus);
	void Activate();
	void Initialize(const GameContext& gameContext) override;
	void PostInitialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;


private:
	FMOD::System *m_pFMODSystem;
	FMOD::Channel *m_pChannel;
	FMOD::Sound* m_pButtonSound;

	void(*m_Function)();
	std::wstring m_NormalTexture;
};

