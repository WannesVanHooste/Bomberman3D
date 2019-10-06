#pragma once
#include "GameScene.h"

class TerrainTestScene final : public GameScene
{
public:
	TerrainTestScene();
	virtual ~TerrainTestScene() = default;

	TerrainTestScene(const TerrainTestScene& other) = delete;
	TerrainTestScene(TerrainTestScene&& other) noexcept = delete;
	TerrainTestScene& operator=(const TerrainTestScene& other) = delete;
	TerrainTestScene& operator=(TerrainTestScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
};

