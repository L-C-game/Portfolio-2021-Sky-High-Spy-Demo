#pragma once
#include "Play.h"
#include "GameObject.h"

class Asteroid : public GameObject
{
public:
	Point2f asteroidCentre{ 75, 80 };

	// Constructor and destructor
	Asteroid(Point2f pos) : GameObject(pos)
	{
		SetType(OBJ_ASTEROID);
		SetUpdateOrder(3);
		SetDrawOrder(1);
	}

	~Asteroid()
	{

	}

	// Function definitions
	void Update(GameState& state) override;
	void Draw(GameState& state) const override;
	static void Spawn(GameState& state);

protected:
		
	
};
