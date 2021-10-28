#include "Gem.h"

void Gem::Draw(GameState& state) const
{
	switch (GetGemState())
	{
	case STATE_BASE:
		GameObject::DrawSpecificCentred(state, "Gem");
		break;
	case STATE_FIVE:
		GameObject::DrawSpecificCentred(state, "Five");
		break;
	case STATE_SHIELD:
		GameObject::DrawSpecificCentred(state, "Shield");
		break;
	case STATE_SPEED:
		GameObject::DrawSpecificCentred(state, "speedUp");
		break;
	}
}

void Gem::Spawn(GameObject* a)
{
	Point2f pos = a->GetPosition();
	GameObject* gem = new Gem(pos);
}

void Gem::Update(GameState& state)
{
	
}



