#include "GameObject.h"

std::vector< GameObject* > GameObject::s_vUpdateList;
std::vector< GameObject* > GameObject::s_vDrawList;

// Game object constructor
// Added default
GameObject::GameObject() = default;
GameObject::GameObject( Point2f pos )
{
	m_pos = pos;

	s_vUpdateList.push_back( this );
	s_vDrawList.push_back( this );
}

// Game object destructor
GameObject::~GameObject()
{
	s_vDrawList.erase( std::find( s_vDrawList.begin(), s_vDrawList.end(), this ) );
	s_vUpdateList.erase( std::find( s_vUpdateList.begin(), s_vUpdateList.end(), this ) );
}

void GameObject::UpdateAll( GameState& state )
{
	std::sort( s_vUpdateList.begin(), s_vUpdateList.end(), GameObject::UpdateOrder );

	for( int n = 0; n < s_vUpdateList.size(); n++ )
	{
		s_vUpdateList[n]->Update( state );
		   
		if( !s_vUpdateList[n]->m_active )
    			delete s_vUpdateList[n--];
	}
}

void GameObject::DrawAll( GameState& state )
{
	std::sort( s_vDrawList.begin(), s_vDrawList.end(), GameObject::DrawOrder );

	for( int n = 0; n < s_vDrawList.size(); n++ )
		s_vDrawList[n]->Draw( state );
}

void GameObject::DrawSpecificCentred(GameState& state, const char* name) const
{
	const int spriteID = PlayBlitter::Instance().GetSpriteId(name);
	PlayBlitter::Instance().CentreSpriteOrigin(spriteID);
	PlayBlitter::Instance().DrawRotated(spriteID, GetPosition(), 2 * state.time, GetRotation());
}

float GameObject::RandomNumGen(int min, int max)
{
	return (std::rand() % ((max - min) + 1)) + min;
}

Point2f GameObject::RandomPos(GameState& state)
{
		int randEdge = GameObject::RandomNumGen(1,4);
		Point2f pos;
		int randXN = GameObject::RandomNumGen(0, DISPLAY_WIDTH);
		int randYE = GameObject::RandomNumGen(0, DISPLAY_HEIGHT);
		int randXS = GameObject::RandomNumGen(0, DISPLAY_WIDTH);
		int randYW = GameObject::RandomNumGen(0, DISPLAY_HEIGHT);
		switch (randEdge)
		{
			case NORTH:
				pos = { randXN, 0 };
				break;
			case EAST:
				pos = { DISPLAY_WIDTH, randYE };
				break;
			case SOUTH:
				pos = { randXS,	DISPLAY_HEIGHT};
				break;
			case WEST:
				pos = { 0, randYW };
				break;
		}
		return pos;
}

float GameObject::RandomAngle(Point2f pos)
{
	// Set the direction opposing the initial position
	// This ensures the objects appear with the screen limits
	float angle = 0;
	// Setting the direction based on polar coords
	if (pos.y == 0) // if North
	{
		// South
		angle = -pi / 2;
	}
	else if (pos.x == DISPLAY_WIDTH) // if East
	{
		// West
		angle = pi;
	}
	else if (pos.y == DISPLAY_HEIGHT) // if South
	{
		// North
		angle = pi / 2;
	}
	else if (pos.x == 0) // if West
	{
		// East
		angle = 0;
	}
	// Offset the direction angle by a random amount
	float randOffset = GameObject::RandomNumGen(-5, 11);
	randOffset *= (pi / 20);
	angle += randOffset;
	return angle;
}

bool GameObject::CheckCollisions(GameObject* a, GameObject* b)
{
	if (HasCollided(a->GetPosition(), b->GetPosition()))
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

void GameObject::ScreenWrapper(Point2f idealPos)
{
	Point2f pos = idealPos;
	if (idealPos.x > DISPLAY_WIDTH + S_SCREEN_LIMIT)
	{
		pos = { -S_SCREEN_LIMIT, idealPos.y };
	}
	else if (idealPos.x < -S_SCREEN_LIMIT)
	{
		pos = { DISPLAY_WIDTH + S_SCREEN_LIMIT, idealPos.y };
	}
	else if (idealPos.y < 0)
	{
		pos = { idealPos.x, DISPLAY_HEIGHT + S_SCREEN_LIMIT };
	}
	else if (idealPos.y > DISPLAY_HEIGHT + S_SCREEN_LIMIT)
	{
		pos = { idealPos.x, 0 };
	}

	SetPosition(pos);
}

// Can give the no. objects
int GameObject::GetObjectCount( GameObject::Type type )
{
	int count = 0;

	for( GameObject* p : s_vUpdateList )
	{
		if( p->m_type == type || type == OBJ_ALL )
			count++;
	}

	return count;
}

int GameObject::GetObjectList( GameObject::Type type, std::vector< GameObject* >& vList )
{
	vList.clear();

	for( GameObject* p : s_vUpdateList )
	{
		if( p->m_type == type || type == OBJ_ALL )
			vList.push_back( p );
	}

	return vList.size();
}

std::vector< GameObject* > GameObject::GetTypeList(GameObject::Type type)
{
	std::vector< GameObject* > typeList;
	for (GameObject* p : s_vUpdateList)
	{
		if (p->m_type == type || type == OBJ_ALL)
			typeList.push_back(p);
	}

	return typeList;
}

// Can remove the 
void GameObject::DestroyAll()
{
	for( int n = 0; n < s_vDrawList.size(); n++ )
		delete s_vUpdateList[n--];
}

