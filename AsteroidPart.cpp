#include "AsteroidPart.h"

// Function definitions
void AsteroidPart::Update(GameState& state)
{
	Point2f currentPos = GetPosition();
	Vector2f currentVel = GetVelocity();
	// Move until the asteroid hits the screen edge then despawn
	Point2f idealPos = { currentPos.x + currentVel.x, currentPos.y + currentVel.y };
	SetPosition(idealPos);
}

void AsteroidPart::Draw(GameState& state) const 
{
	const int asteroidPartID = PlayBlitter::Instance().GetSpriteId("asteroid_pieces_3");
	PlayBlitter::Instance().SetSpriteOrigin(asteroidPartID, asteroidPartCentre);
	PlayBlitter::Instance().DrawRotated(asteroidPartID, GetPosition(), GetFrame(), GetRotation() + pi/2); //Here adding pi/2 just 
}

void AsteroidPart::Spawn(GameObject* a) 
{
	for (int n = 1; n < 4; n++)
	{
		// If we start at 0 2/3pi off
		Point2f pos = a->GetPosition();
		float astAngle = a->GetRotation();
		GameObject* astPart = new AsteroidPart(pos);
		astPart->SetRotation(astAngle);
		float angle = (astAngle + (-n * ((2 * pi) / 3)));
		float Vx = MAX_SPEED * cos(angle);
		float Vy = MAX_SPEED * sin(angle);
		Vector2f currentVel = { Vx, Vy };
		astPart->SetVelocity(currentVel);
		AsteroidPart* astGib = dynamic_cast<AsteroidPart*>(astPart);
		if (astGib != nullptr)
		{
			astGib->SetFrame(n);
		}
	}

	PlaySpeaker::Instance().StartSound("explode", false);
}