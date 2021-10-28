#pragma once
#include "Play.h"
#include "GameObject.h"


class Particle : public GameObject
{
protected:
	bool m_hasTimerRun{ false };
	int m_particleState{ 0 };
	int m_particleType{ 0 };

public:
	// Constructor and destructor
	Particle(Point2f pos, int type) : GameObject(pos)
	{
		SetType(OBJ_PARTICLE);
		SetUpdateOrder(1);
		SetDrawOrder(6);

		float randNum = GameObject::RandomNumGen(1, 2);
		switch (type)
		{
		case PARTICLE:
			// Set overall type, particle
			if (randNum == 1)
			{
				SetParticleState(WHITE_DUST);
			}
			if (randNum == 2)
			{
				SetParticleState(GREY_DUST);
			}
			break;
		case SPARKLE:
			if (randNum == 1)
			{
				SetParticleState(WHITE);
			}
			else if (randNum == 2)
			{
				SetParticleState(YELLOW);
			}

		}
	}

	~Particle()
	{

	}

	enum ParticleType
	{
		PARTICLE = 0,
		SPARKLE 
	};

	enum ParticleState
	{
		WHITE_DUST = 0,
		GREY_DUST,
		WHITE,
		YELLOW
	};

	std::chrono::steady_clock::time_point endTime;

	// Function definitions
	void Update(GameState& state) override;
	void Draw(GameState& state) const override;
	static void Spawn(GameObject* a, int type);


	// Getters and setters
	void SetHasTimerRun(bool hasTimerRun) { m_hasTimerRun = hasTimerRun; }
	bool GetHasTimerRun() const { return m_hasTimerRun; }
	void SetParticleState(int particleState) { m_particleState = particleState; }
	int GetParticleState() const { return m_particleState; };
};

