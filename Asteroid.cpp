#include "Asteroid.h"

void Asteroid::Draw(GameState& state) const
{
	const int asteroidID = PlayBlitter::Instance().GetSpriteId("asteroid_2"); 
	PlayBlitter::Instance().SetSpriteOrigin(asteroidID, asteroidCentre);
	PlayBlitter::Instance().DrawRotated(asteroidID, GetPosition(), 2 * state.time, GetRotation() + pi/2);
}

void Asteroid::Spawn(GameState& state)
{
	if (GameObject::GetObjectCount(GameObject::OBJ_ASTEROID) < 6)
	{
		Point2f pos = GameObject::RandomPos(state);
		float angle = GameObject::RandomAngle(pos);
		GameObject* a = new Asteroid(pos);
		a->SetRotation(angle);
		float Vx = MAX_SPEED * cos(angle);
		float Vy = MAX_SPEED * sin(angle);
		Vector2f currentVel = { Vx, Vy };
		a->SetVelocity(currentVel);
	}
}

void Asteroid::Update(GameState& state)
{
	Point2f currentPos = GetPosition();
	Vector2f currentVel = GetVelocity();
	// Move until the asteroid hits the screen edge then despawn
	Point2f idealPos = { currentPos.x + currentVel.x, currentPos.y + currentVel.y };
	SetPosition(idealPos);
	GameObject::ScreenWrapper(idealPos);
}