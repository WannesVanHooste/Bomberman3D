#pragma once
#include "GameScene.h"

class TextureData;
class SpriteFont;

class FontTestScene final : public GameScene
{
public:
	FontTestScene();
	virtual ~FontTestScene() = default;

	FontTestScene(const FontTestScene& other) = delete;
	FontTestScene(FontTestScene&& other) noexcept = delete;
	FontTestScene& operator=(const FontTestScene& other) = delete;
	FontTestScene& operator=(FontTestScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;

private:
	float m_FpsInterval;
	float m_Counter;
	SpriteFont* m_pFont;
};

