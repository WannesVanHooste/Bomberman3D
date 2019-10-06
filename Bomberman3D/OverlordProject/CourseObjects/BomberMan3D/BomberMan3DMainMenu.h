#pragma once
#include <GameScene.h>
#include "ButtonManager.h"

class BomberMan3DMainMenu :
	public GameScene
{
public:
	BomberMan3DMainMenu();
	virtual ~BomberMan3DMainMenu() = default;
	BomberMan3DMainMenu(const BomberMan3DMainMenu& other) = delete;
	BomberMan3DMainMenu(BomberMan3DMainMenu&& other) noexcept = delete;
	BomberMan3DMainMenu& operator=(const BomberMan3DMainMenu& other) = delete;
	BomberMan3DMainMenu& operator=(BomberMan3DMainMenu&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
private:
	ButtonManager* m_pButtonManager;
	FMOD::System *m_pFMODSystem;
	FMOD::Channel *m_pChannel;
	FMOD::Sound* m_pButtonSound;
};

