#pragma once
#include <GameObject.h>
class Timer :
	public GameObject
{
public:
	Timer() = default;
	~Timer() = default;

	Timer(const Timer& other) = delete;
	Timer(Timer&& other) noexcept = delete;
	Timer& operator=(const Timer& other) = delete;
	Timer& operator=(Timer&& other) noexcept = delete;

	void Initialize(const GameContext& gameContext) override;
	void PostInitialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;

private:
	float m_Timer = 300.0f;
};

