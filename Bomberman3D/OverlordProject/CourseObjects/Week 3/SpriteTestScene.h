#pragma once
#include "GameScene.h"

class TextureData;

class SpriteTestScene final : public GameScene
{
public:
	SpriteTestScene();
	virtual ~SpriteTestScene() = default;

	SpriteTestScene(const SpriteTestScene& other) = delete;
	SpriteTestScene(SpriteTestScene&& other) noexcept = delete;
	SpriteTestScene& operator=(const SpriteTestScene& other) = delete;
	SpriteTestScene& operator=(SpriteTestScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;

private:
	float m_FpsInterval;
	float m_Counter;
	TextureData* m_pTexture;
	GameObject* m_pObj;
};

