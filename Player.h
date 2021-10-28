#pragma once
#include "Play.h"
#include "GameObject.h"

class Player : public GameObject
{
public:
	// Origins relevant to the different states
	Point2f attachedCentre = {65, 110};
	Point2f deadCentre = { 65, 40 };
	float radius = 70;
	GameObject* currentAst;
	GameObject* refPlayer;
	double MAX_S_AGENT8 = sqrt(49);
	std::chrono::steady_clock::time_point endTime;

	enum PlayerState
	{
		STATE_ATTACHED = 0,
		STATE_FLYING,
		STATE_SHIELD,
		STATE_SPEED,
		STATE_DEAD
	};

	// Constructor and destructor
	Player(Point2f pos, GameObject* a) : GameObject(pos)
	{
		SetType(OBJ_PLAYER);
		SetUpdateOrder(0);
		SetDrawOrder(0);
		currentAst = a;

		//Point2f playerPos;
		//playerPos.x = pos.x + cos(GetRotation()) * radius;
		//playerPos.y = pos.y + sin(GetRotation()) * radius;
		//SetPosition(playerPos);
	}

	~Player()
	{

	}

	// Function definitions
	static void Spawn(GameState& state);
	void Update(GameState& state) override;
	void FlyingUpdate(GameState& state);
	void AttachedUpdate(GameState& state);
	void DeadUpdate(GameState& state);
	void Draw(GameState& state) const override;
	void FlyingDraw(GameState& state) const;
	void AttachedDraw(GameState& state) const;
	void ShieldDraw(GameState& state) const;
	void SpeedDraw(GameState& state) const;
	void DeadDraw(GameState& state) const;

	//calculate velocity
	Vector2f CalcVelocity(float angle);
	Point2f CalcOffset(float angle);

	void SetPlayerState(int pState) { m_playerState = pState; }
	int GetPlayerState() const { return m_playerState; }
	void SetIsDead(bool isDead) { m_isDead = isDead; }
	bool GetIsDead() const { return m_isDead; }
	void SetHasTimerRun(bool hasTimerRun) { m_hasTimerRun = hasTimerRun; }
	bool GetHasTimerRun() const { return m_hasTimerRun; }

private:
	int m_playerState{ 0 };
	bool m_isDead{ false };
	bool m_hasTimerRun{ false };
};