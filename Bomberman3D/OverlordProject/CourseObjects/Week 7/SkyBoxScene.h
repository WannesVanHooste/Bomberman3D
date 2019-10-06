#pragma once
#include "GameScene.h"

class ModelComponent;

class SkyBoxScene final : public GameScene
{
public:
	SkyBoxScene();
	virtual ~SkyBoxScene() = default;

	SkyBoxScene(const SkyBoxScene& other) = delete;
	SkyBoxScene(SkyBoxScene&& other) noexcept = delete;
	SkyBoxScene& operator=(const SkyBoxScene& other) = delete;
	SkyBoxScene& operator=(SkyBoxScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
};

