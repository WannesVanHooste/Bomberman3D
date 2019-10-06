#pragma once
#include "GameScene.h"

class CharacterTest : public GameScene
{
public:
	CharacterTest();
	virtual ~CharacterTest() = default;

	CharacterTest(const CharacterTest& other) = delete;
	CharacterTest(CharacterTest&& other) noexcept = delete;
	CharacterTest& operator=(const CharacterTest& other) = delete;
	CharacterTest& operator=(CharacterTest&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;

private:

};

