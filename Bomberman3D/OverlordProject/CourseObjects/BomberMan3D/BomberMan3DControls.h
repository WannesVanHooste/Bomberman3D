#pragma once
#include <GameScene.h>
#include "Button.h"

class BomberMan3DControls :public GameScene
{
public:
	BomberMan3DControls();
	virtual ~BomberMan3DControls() = default;

	BomberMan3DControls(const BomberMan3DControls& other) = delete;
	BomberMan3DControls(BomberMan3DControls&& other) noexcept = delete;
	BomberMan3DControls& operator=(const BomberMan3DControls& other) = delete;
	BomberMan3DControls& operator=(BomberMan3DControls&& other) noexcept = delete;
protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;

private:
	Button* m_pBackButton;
};

