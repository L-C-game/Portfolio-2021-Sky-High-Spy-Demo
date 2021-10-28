#include "Meteor.h"

void Meteor::Draw(GameState& state) const
{
	const int meteorID = PlayBlitter::Instance().GetSpriteId("meteor_2");
	Vector2f newOrigin = { 64,45 };
	PlayBlitter::Instance().SetSpriteOrigin(meteorID, newOrigin);
	PlayBlitter::Instance().DrawRotated(meteorID, GetPosition(), 2 * state.time, GetRotation() + pi / 2);
}

void Meteor::Spawn(GameState& state)
{
	if (GameObject::GetObjectCount(GameObject::OBJ_METEOR) < 2)
	{
		Point2f pos = GameObject::RandomPos(state);
		float angle = GameObject::RandomAngle(pos);
		GameObject* m = new Meteor(pos);
		m->SetRotation(angle);
		float Vx = MAX_SPEED * cos(angle);
		float Vy = MAX_SPEED * sin(angle);
		Vector2f currentVel = { Vx, Vy };
		m->SetVelocity(currentVel);
	}
}

void Meteor::Update(GameState& state)
{
	Point2f currentPos = GetPosition();
	Vector2f currentVel = GetVelocity();
	// Move until the meteor hits the screen edge then despawn
	Point2f idealPos = { currentPos.x + currentVel.x, currentPos.y + currentVel.y };
	SetPosition(idealPos);
	GameObject::ScreenWrapper(idealPos);
}