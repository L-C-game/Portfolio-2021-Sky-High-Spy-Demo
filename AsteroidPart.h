#pragma once
#include "Play.h"
#include "GameObject.h"

class AsteroidPart : public GameObject
{
public:
	// Constants
	const Point2f asteroidPartCentre{ 75, 80 };

	// Constructor and destructor
	AsteroidPart(Point2f pos) : GameObject(pos)
	{
		SetType(OBJ_ASTEROID_PART);
		SetUpdateOrder(4);
		SetDrawOrder(1);
	}

	~AsteroidPart()
	{

	}

	// Function definitions
	void Update(GameState& state) override;
	void Draw(GameState& state) const override;
	static void Spawn(GameObject* a);

	void SetFrame(int frame) { m_frame = frame; }
	int GetFrame() const { return m_frame; };

private:
	int m_frame = 1;
};