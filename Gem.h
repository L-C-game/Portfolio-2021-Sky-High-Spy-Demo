#pragma once
#include "Play.h"
#include "GameObject.h"

class Gem : public GameObject
{
private:
	int m_gemState{ 0 };

public:
	// Constructor and destructor
	Gem(Point2f pos) : GameObject(pos)
	{
		SetType(OBJ_GEM);
		SetUpdateOrder(1);
		SetDrawOrder(5);
		//Random chance other gems
		// each powerup has a 1/4 chance of spawing (high for testing purposes)
		// This time using the playbuffer random functions
		float randNum = GameObject::RandomNumGen(1, 4);

		if (randNum == 1)
		{
			SetGemState(STATE_BASE);
		}
		else if (randNum == 2)
		{
			SetGemState(STATE_FIVE);
		}
		else if (randNum == 3)
		{
			SetGemState(STATE_SHIELD);
		}
		else if (randNum == 4)
		{
			SetGemState(STATE_SPEED);
		}

	}

	~Gem()
	{

	}

	// Gem/Powerup states
	// The base gem does nothing
	// The blue is worth 5 points
	// The green gives you a shield that can survive 1 hit
	// Want 1 timer based powerup, e.g. speed up etc
	enum GemState
	{
		STATE_BASE = 0,
		STATE_FIVE,
		STATE_SHIELD,
		STATE_SPEED
	};

	// Function definitions
	void Update(GameState& state) override;
	void Draw(GameState& state) const override;
	static void Spawn(GameObject* a);

	void SetGemState(int gemState) { m_gemState = gemState; }
	int GetGemState() const { return m_gemState; };
};