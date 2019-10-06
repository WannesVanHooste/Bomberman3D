#pragma once
#include "GameScene.h"
#include "EffectHelper.h"

class Material;

class TestUber final : public GameScene
{
public:
	TestUber();
	virtual ~TestUber() = default;

	TestUber(const TestUber& other) = delete;
	TestUber(TestUber&& other) noexcept = delete;
	TestUber& operator=(const TestUber& other) = delete;
	TestUber& operator=(TestUber&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;

private:
};

