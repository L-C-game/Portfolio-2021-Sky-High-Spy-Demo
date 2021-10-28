#include <chrono>
#include "Meteor.h"
#include "Asteroid.h"
#include "AsteroidPart.h"
#include "Gem.h"
#include "Particle.h"
#include "Player.h"

Point2f Player::CalcOffset(float angle)
{
	Point2f offset = { radius * cos(angle), radius * sin(angle) };
	return offset;
}

Vector2f Player::CalcVelocity(float angle)
{
	float Vx = MAX_S_AGENT8 * cos(angle);
	float Vy = MAX_S_AGENT8 * sin(angle);
	Vector2f playerVel = { Vx, Vy };
	return playerVel;
}

void Player::Spawn(GameState& state)
{
	if (!GameObject::s_vUpdateList.size() == 0)
	{
		std::vector<GameObject*> asteroidList = GameObject::GetTypeList(OBJ_ASTEROID);
		if (GameObject::GetTypeList(OBJ_ASTEROID).size() != 0)
		{
			for (GameObject* g : asteroidList)
			{
				Asteroid* a = dynamic_cast<Asteroid*>(g);

				if (a != nullptr && a->GetActive())
				{
					//Player constructor
					if (GameObject::GetObjectCount(GameObject::OBJ_PLAYER) < 1)
					{
						Point2f pos = a->GetPosition();
						Vector2f currentVel = a->GetVelocity();
						// Construct player
						GameObject* p = new Player(pos, a);
						p->SetVelocity(currentVel);
					}
				}
			}
		}
	}
}

void Player::Update(GameState& state)
{
	//Switch
	switch (GetPlayerState())
	{
	case STATE_ATTACHED:
		AttachedUpdate(state);
		break;
	case STATE_FLYING:
		FlyingUpdate(state);
		break;
	case STATE_SHIELD:
		FlyingUpdate(state);
		break;
	case STATE_SPEED:
		FlyingUpdate(state);
		break;
	case STATE_DEAD:
		DeadUpdate(state);
		break;
	}
}

void Player::FlyingUpdate(GameState& state)
{
	//Flies in direction they are facing
	// Slight control of trajectory using left and right arrow keys

	SetPosition(GetPosition() + GetVelocity());
	float angle = GetRotation();
	if (PlayBuffer::Instance().KeyDown(VK_LEFT))
	{
		angle -= 1 / (36 * pi);
		if (angle < -pi)
		{
			angle = pi;
		}
		Vector2f playerVel = CalcVelocity(angle);
		SetVelocity(playerVel);
	}
	else if (PlayBuffer::Instance().KeyDown(VK_RIGHT))
	{
		angle += 1 / (36 * pi);
		if (angle > pi)
		{
			angle = -pi;
		}
		Vector2f playerVel = CalcVelocity(angle);
		SetVelocity(playerVel);
	}
	SetRotation(angle);

	Point2f currentPos = GetPosition();
	// if goes off screen, destroy
	// need to add restart
	if (currentPos.x > DISPLAY_WIDTH + S_SCREEN_LIMIT || currentPos.x < -S_SCREEN_LIMIT ||
		currentPos.y < 0 || currentPos.y > DISPLAY_HEIGHT + S_SCREEN_LIMIT)
	{
		SetPlayerState(STATE_DEAD);
	}

	if (!GameObject::s_vUpdateList.size() == 0)
	{
		// if collides with meteor, state changes to dead
		if (GameObject::GetTypeList(OBJ_METEOR).size() != 0)
		{
			std::vector<GameObject*> meteorList = GameObject::GetTypeList(OBJ_METEOR);
			for (GameObject* g : meteorList)
			{
				Meteor* m = dynamic_cast<Meteor*>(g);
				if (GameObject::CheckCollisions(this, m))
				{
					if (GetPlayerState() != STATE_SHIELD)
					{
						PlaySpeaker::Instance().StartSound("combust", false);
						SetPlayerState(STATE_DEAD);
					}
					if (GetPlayerState() == STATE_SHIELD)
					{
						// need iframes
						if (GetHasTimerRun() == false)
						{
							endTime = std::chrono::steady_clock::now()
								+ std::chrono::seconds(1);
							PlaySpeaker::Instance().StartSound("clang", false);
							SetHasTimerRun(true);
						}
					}
				}
			}
		}

		if (GetHasTimerRun() == true && std::chrono::steady_clock::now() > endTime)
		{
			SetHasTimerRun(false);
			SetPlayerState(STATE_FLYING);
		}

		// Attach to the new asteroid
		if (GameObject::GetTypeList(OBJ_ASTEROID).size() != 0)
		{
			std::vector<GameObject*> asteroidList = GameObject::GetTypeList(OBJ_ASTEROID);
			for (GameObject* g : asteroidList)
			{
				Asteroid* a = dynamic_cast<Asteroid*>(g);
				if (GameObject::CheckCollisions(this, a))
				{
					//Point2f offset = { radius * cos(angle), radius * sin(angle) };
					Point2f pos = a->GetPosition();
					Vector2f currentVel = a->GetVelocity();
					SetPosition(pos);
					SetVelocity(currentVel);
					currentAst = a;
					a->SetDrawOrder(1);
					MAX_S_AGENT8 = sqrt(49);
					SetPlayerState(STATE_ATTACHED);

				}
			}
		}
		// Pickup gem
		if (GameObject::GetTypeList(OBJ_GEM).size() != 0)
		{
			std::vector<GameObject*> gemList = GameObject::GetTypeList(OBJ_GEM);
			for (GameObject* g : gemList)
			{
				Gem* gem = dynamic_cast<Gem*>(g);
				if (GameObject::CheckCollisions(this, gem))
				{
					switch (gem->GetGemState())
					{
					case gem->STATE_BASE:
						state.score++;
						// Spawn sparkles
						Particle::Spawn(gem, Particle::SPARKLE);
						gem->SetActive(false);
						break;
					case gem->STATE_FIVE:
						state.score += 5;
						// Spawn sparkles
						Particle::Spawn(gem, Particle::SPARKLE);
						gem->SetActive(false);
						break;
					case gem->STATE_SHIELD:
						if (GetPlayerState() == STATE_FLYING)
						{
							SetPlayerState(STATE_SHIELD);
						}
						state.score++;
						// Spawn sparkles
						Particle::Spawn(gem, Particle::SPARKLE);
						gem->SetActive(false);
						break;
					case gem->STATE_SPEED:
						if (GetPlayerState() == STATE_FLYING)
						{
							MAX_S_AGENT8 *= 1.5;
							Vector2f playerVel = CalcVelocity(GetRotation());
							SetVelocity(playerVel);
							SetPlayerState(STATE_SPEED);
						}
						state.score++;
						// Spawn sparkles
						Particle::Spawn(gem, Particle::SPARKLE);
						gem->SetActive(false);
						break;
					}
					PlaySpeaker::Instance().StartSound("reward", false);
				}
			}
		}
	}

}

void Player::AttachedUpdate(GameState& state)
{
	// Reference to the asteroid agent 8 is attached to
	float angle = GetRotation();

	// input
	if (PlayBuffer::Instance().KeyDown(VK_LEFT))
	{
		angle -= 1/(6*pi);
		if (angle < -pi)
		{
			angle = pi;
		}
	}
	else if (PlayBuffer::Instance().KeyDown(VK_RIGHT))
	{
		angle += 1/(6*pi);
		if (angle > pi)
		{
			angle = -pi;
		}
	}

	SetRotation(angle);
	Point2f playerPos = GetPosition();
	SetPosition(playerPos + GetVelocity());
	GameObject::ScreenWrapper(GetPosition());

	if (PlayBuffer::Instance().KeyDown(VK_SPACE))
	{
		SetPosition(GetPosition() + CalcOffset(angle));
		Vector2f playerVel = CalcVelocity(angle);
		SetVelocity(playerVel);
		SetPlayerState(STATE_FLYING);
		// Set asteroid to broken, needs to break and spawn a gem
		AsteroidPart::Spawn(currentAst);
		Gem::Spawn(currentAst);
		Particle::Spawn(currentAst, Particle::PARTICLE);
		currentAst->SetActive(false);
	}
}

void Player::DeadUpdate(GameState& state)
{
	// travel in direction it is facing
	SetPosition(GetPosition() + GetVelocity());
	Point2f currentPos = GetPosition();

	// destroy when the player leaves the screen
	if (currentPos.x > DISPLAY_WIDTH + S_SCREEN_LIMIT || currentPos.x < -S_SCREEN_LIMIT ||
		currentPos.y < 0 || currentPos.y > DISPLAY_HEIGHT + S_SCREEN_LIMIT)
	{
		// Reset the game
		SetIsDead(true);
	}
}

void Player::Draw(GameState& state) const
{
	//Switch goes here
	switch (GetPlayerState())
	{
	case STATE_ATTACHED:
		AttachedDraw(state);
		break;
	case STATE_FLYING:
		FlyingDraw(state);
		break;
	case STATE_SHIELD:
		FlyingDraw(state);
		ShieldDraw(state);
		break;
	case STATE_SPEED:
		SpeedDraw(state);
		FlyingDraw(state);
		break;
	case STATE_DEAD:
		DeadDraw(state);
		break;
	}
}

void Player::FlyingDraw(GameState& state) const
{
	const int playerID = PlayBlitter::Instance().GetSpriteId("agent8_fly");
	PlayBlitter::Instance().CentreSpriteOrigin(playerID);
	PlayBlitter::Instance().DrawRotated(playerID, GetPosition(), 2 * state.time, GetRotation() + pi / 2);
}

void Player::ShieldDraw(GameState& state) const
{
	const int playerID = PlayBlitter::Instance().GetSpriteId("shield_ring");
	PlayBlitter::Instance().CentreSpriteOrigin(playerID);
	PlayBlitter::Instance().DrawRotated(playerID, GetPosition(), 2 * state.time, GetRotation() + pi / 2);
}

void Player::SpeedDraw(GameState& state) const
{
	const int playerID = PlayBlitter::Instance().GetSpriteId("agent8_spd");
	PlayBlitter::Instance().CentreSpriteOrigin(playerID);
	PlayBlitter::Instance().DrawRotated(playerID, GetPosition(), 2 * state.time, GetRotation() + pi / 2);
}

void Player::AttachedDraw(GameState& state) const
{
	int playerID = PlayBlitter::Instance().GetSpriteId("agent8_left_7");
	int frame = 0;

	if (PlayBuffer::Instance().KeyDown(VK_LEFT))
	{
		playerID = PlayBlitter::Instance().GetSpriteId("agent8_left_7");
		frame = 2 * state.time;
	}
	else if (PlayBuffer::Instance().KeyDown(VK_RIGHT))
	{
		playerID = PlayBlitter::Instance().GetSpriteId("agent8_right_7");
		frame = 2 * state.time;
	}

	PlayBlitter::Instance().SetSpriteOrigin(playerID, attachedCentre);
	PlayBlitter::Instance().DrawRotated(playerID, GetPosition(), frame, GetRotation() + pi / 2);
}

void Player::DeadDraw(GameState& state) const
{
	const int playerID = PlayBlitter::Instance().GetSpriteId("agent8_dead_2");
	PlayBlitter::Instance().CentreSpriteOrigin(playerID);
	PlayBlitter::Instance().DrawRotated(playerID, GetPosition(), 2 * state.time, GetRotation() + pi / 2);
}