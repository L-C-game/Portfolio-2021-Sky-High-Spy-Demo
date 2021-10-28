#pragma once
#include "Play.h"
#include "GameObject.h"

class Meteor : public GameObject
{
public:
	// Constructor and destructor
	Meteor(Point2f pos) : GameObject(pos)
	{
		SetType(OBJ_METEOR);
		SetUpdateOrder(2);
		SetDrawOrder(3);
	}

	~Meteor()
	{

	}

	// Function definitions
	void Update(GameState& state) override;
	void Draw(GameState& state) const override;
	static void Spawn(GameState& state);

private:

};