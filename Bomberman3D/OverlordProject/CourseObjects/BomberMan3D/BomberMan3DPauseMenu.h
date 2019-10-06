#pragma once
#include <GameScene.h>
#include "ButtonManager.h"

class BomberMan3DPauseMenu :public GameScene
{
public:
	BomberMan3DPauseMenu();
	virtual ~BomberMan3DPauseMenu() = default;
	BomberMan3DPauseMenu(const BomberMan3DPauseMenu& other) = delete;
	BomberMan3DPauseMenu(BomberMan3DPauseMenu&& other) noexcept = delete;
	BomberMan3DPauseMenu& operator=(const BomberMan3DPauseMenu& other) = delete;
	BomberMan3DPauseMenu& operator=(BomberMan3DPauseMenu&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
private:
	ButtonManager* m_pButtonManager;
};

