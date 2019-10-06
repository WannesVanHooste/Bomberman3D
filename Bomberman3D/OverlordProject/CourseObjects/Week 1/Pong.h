#pragma once
#include "GameScene.h"

class Pong : public GameScene
{
public:
	Pong();
	virtual ~Pong() = default;


	Pong(const Pong& other) = delete;
	Pong(Pong&& other) noexcept = delete;
	Pong& operator=(const Pong& other) = delete;
	Pong& operator=(Pong&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;

private:
	GameObject* m_pProjectile = nullptr;
	GameObject* m_pCube1 = nullptr;
	GameObject* m_pCube2 = nullptr;
};

