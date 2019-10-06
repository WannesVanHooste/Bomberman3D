#include "stdafx.h"
#include "ButtonManager.h"

void ButtonManager::Initialize(const GameContext& ) {
}
void ButtonManager::PostInitialize(const GameContext& ) {}
void ButtonManager::Update(const GameContext& ) {
}

void ButtonManager::AddButton(Button* button)
{
	m_pButtons.push_back(button);
	m_pActiveButton = m_pButtons[0];
	m_pActiveButton->Focus(true);
}

void ButtonManager::NextButton() {
	for (unsigned int i = 0; i < m_pButtons.size(); ++i)
	{
		if (m_pButtons[i] == m_pActiveButton)
		{
			auto nextButton = ++i % m_pButtons.size();
			m_pActiveButton = m_pButtons[nextButton];
			m_pActiveButton->Focus(true);
			break;
		}
	}
	for (unsigned int i = 0; i < m_pButtons.size(); ++i) {
		if (m_pButtons[i] != m_pActiveButton) {
			m_pButtons[i]->Focus(false);
		}
	}
}

void ButtonManager::PreviousButton() {
	for (unsigned int i = 0; i < m_pButtons.size(); ++i)
	{
		if (m_pButtons[i] == m_pActiveButton)
		{
			if (i == 0) {
				i = unsigned int(m_pButtons.size() - 1);
			}
			else {
				i -= 1;
			}
			m_pActiveButton = m_pButtons[i];
			m_pActiveButton->Focus(true);
			break;
		}
	}
	for (unsigned int i = 0; i < m_pButtons.size(); ++i) {
		if (m_pButtons[i] != m_pActiveButton) {
			m_pButtons[i]->Focus(false);
		}
	}
}

Button* ButtonManager::GetActiveButton() {
	return m_pActiveButton;
}
