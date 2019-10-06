#pragma once
#include "ButtonManager.h"
#include "BomberMan3DScene.h"

//class reworked and reused from Programming 4 exam project
class PlayerLocator
{
public:
	static GameObject* GetPlayerOne() { return m_Player1; }
	static GameObject* GetPlayerTwo() { return m_Player2; }
	static BomberMan3DScene* GetMainScene() { return m_pMainScene; }

	static void ProvidePlayerOne(GameObject* player1)
	{
		m_Player1 = player1;
	}
	static void ProvidePlayerTwo(GameObject* player2)
	{
		m_Player2 = player2;
	}
	static void ProvideMainScene(BomberMan3DScene* scene) {
		m_pMainScene = scene;
	}
private:
	static GameObject* m_Player1;
	static GameObject* m_Player2;
	static BomberMan3DScene* m_pMainScene;
};

