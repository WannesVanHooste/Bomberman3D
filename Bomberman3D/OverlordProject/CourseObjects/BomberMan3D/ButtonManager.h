#pragma once
#include <GameObject.h>
#include "Button.h"

//class reworked and reused from Programming 4 exam project
class ButtonManager :
	public GameObject
{
public:
	ButtonManager() = default;

	ButtonManager(const ButtonManager& other) = delete;
	ButtonManager(ButtonManager&& other) noexcept = delete;
	ButtonManager& operator=(const ButtonManager& other) = delete;
	ButtonManager& operator=(ButtonManager&& other) noexcept = delete;

	void Initialize(const GameContext& gameContext) override;
	void PostInitialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;

	void AddButton(Button* button);
	void NextButton();
	void PreviousButton();
	Button* GetActiveButton();
	void SetActiveButtonByIndex(int i) { m_pActiveButton = m_pButtons[i]; }
private:

	std::vector<Button*> m_pButtons;
	std::vector<SpriteComponent*> m_pSprites;
	Button* m_pActiveButton;
	SpriteComponent* m_pCurrentSprite;

};

