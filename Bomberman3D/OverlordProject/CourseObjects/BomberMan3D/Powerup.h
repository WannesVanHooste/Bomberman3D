#pragma once
#include <GameObject.h>
class Powerup :
	public GameObject
{
public:
	Powerup(int blockID, int powerupId);
	~Powerup() = default;

	Powerup(const Powerup& other) = delete;
	Powerup(Powerup&& other) noexcept = delete;
	Powerup& operator=(const Powerup& other) = delete;
	Powerup& operator=(Powerup&& other) noexcept = delete;

	void Initialize(const GameContext& gameContext) override;
	void PostInitialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;

private:
	FMOD::System *m_pFMODSystem;
	FMOD::Channel *m_pChannel;
	FMOD::Sound* m_pPickupSound;
	int m_BlockID;
	int m_PowerupId;
	bool m_IsUsed = false;
};

