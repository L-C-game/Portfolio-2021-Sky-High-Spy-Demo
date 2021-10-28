#include <chrono>
#include "Particle.h"

void Particle::Draw(GameState& state) const
{
	PlayBlitter& blit = PlayBlitter::Instance();

	switch (Particle::GetParticleState())
	{
	case WHITE_DUST:
		GameObject::DrawSpecificCentred(state, "particle");
		break;
	case GREY_DUST:
		GameObject::DrawSpecificCentred(state, "particle2");
		break;
	case WHITE:
		GameObject::DrawSpecificCentred(state, "white_sparkle");
		break;
	case YELLOW:
		GameObject::DrawSpecificCentred(state, "yellow_sparkle");
		break;
	}
}

void Particle::Spawn(GameObject* a, int type)
{
	for (int n = 1; n < 14; n++)
	{
		Point2f pos = a->GetPosition();
		float angle = 4 * GameObject::RandomAngle(pos);
		GameObject* pPart = new Particle(pos, type);
		pPart->SetRotation(angle);
		float Vx = MAX_SPEED * cos(angle);
		float Vy = MAX_SPEED * sin(angle);
		Vector2f currentVel = { Vx, Vy };
		pPart->SetVelocity(currentVel);
	}
}

void Particle::Update(GameState& state)
{
	Point2f currentPos = GetPosition();
	Vector2f currentVel = GetVelocity();

	Point2f idealPos = { currentPos.x + currentVel.x, currentPos.y + currentVel.y };
	SetPosition(idealPos);

	if (GetHasTimerRun() == false)
	{
		endTime = std::chrono::steady_clock::now()
			+ std::chrono::milliseconds(800);
		SetHasTimerRun(true);
	}

	if (GetHasTimerRun() == true && std::chrono::steady_clock::now() > endTime)
	{
		SetHasTimerRun(false);
		SetActive(false);
	}
}