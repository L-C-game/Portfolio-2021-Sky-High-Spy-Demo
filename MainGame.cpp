#include "MainGame.h"
#include "GameObject.h"
#include "Asteroid.h"
#include "Meteor.h"
#include "Player.h"
#define PLAY_IMPLEMENTATION
#include "Play.h"

GameState state;

// The entry point for a Windows program
int WINAPI WinMain( PLAY_WINARGS_IN )
{
	std::srand(std::time(NULL));

	PlayBuffer& buff = PlayBuffer::Instance(DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE, 0);
	PlayBlitter& blit = PlayBlitter::Instance("Data\\Sprites\\");
	PlaySpeaker& speak = PlaySpeaker::Instance("Data\\Audio\\");

	// Do things here that only need to be performed once at the start of your application

	// Set up the window display using the relevant constants
	blit.SetDisplayBuffer(buff.GetDisplayBuffer(), DISPLAY_WIDTH, DISPLAY_HEIGHT);
	// Load the background image from the file
	blit.LoadBackground("Data\\Backgrounds\\Background.png");
	
	//Return
	// Display the window title
	return buff.HandleWindows( PLAY_WINARGS_OUT, L"Sky High Spy!" );
}

// Called by the PlayBuffer once for each frame of the game (60 times a second!)
bool MainGameUpdate( float elapsedTime )
{
	PlayBuffer& buff = PlayBuffer::Instance();
	PlayBlitter& blit = PlayBlitter::Instance();
	
	// To keep track of the elapsed time
	state.time += elapsedTime;

	blit.DrawBackground();
	// Calling the methods
	GameObject::DrawAll(state);

	Meteor::Spawn(state);
	Asteroid::Spawn(state);
	Player::Spawn(state);

	switch (GetMainGameState())
	{
	case START_STATE:
		StartStateUpdate();
		break;
	case ACTIVE_STATE:
		PlayStateUpdate();
		break;
	case GAMEOVER_STATE:
		GameOverStateUpdate();
		break;
	}
	

	buff.Present();
	// Allows the player to easily exit the game
	return buff.KeyDown(VK_ESCAPE);
}

//Different updates depending on the current game state
void StartStateUpdate()
{
	PlayBuffer& buff = PlayBuffer::Instance();
	PlayBlitter& blit = PlayBlitter::Instance();
	state.score = 0;

	//PlaySpeaker::Instance().StopSound("music");

	if (buff.KeyDown(VK_RETURN))
	{
		PlaySpeaker::Instance().StartSound("music", true);
		SetMainGameState(ACTIVE_STATE);
	}

	blit.DrawStringCentred(blit.GetSpriteId("105px"), { DISPLAY_WIDTH / 2, 50 }, "Welcome to Sky High Spy!");
	blit.DrawStringCentred(blit.GetSpriteId("64px"), { DISPLAY_WIDTH / 2, 2 * DISPLAY_HEIGHT / 6 }, "Use the direction buttons to aim Agent8,");
	blit.DrawStringCentred(blit.GetSpriteId("64px"), { DISPLAY_WIDTH / 2, 3 * DISPLAY_HEIGHT / 6 }, "Press SPACE to jump between Asteroids,");
	blit.DrawStringCentred(blit.GetSpriteId("64px"), { DISPLAY_WIDTH / 2, 4 * DISPLAY_HEIGHT / 6 }, "Press ENTER/RETURN to start the game,");
	blit.DrawStringCentred(blit.GetSpriteId("64px"), { DISPLAY_WIDTH / 2, 5 * DISPLAY_HEIGHT / 6 }, "Press ESCAPE to quit the game.");
}

void PlayStateUpdate()
{
	GameObject::UpdateAll(state);

	PlayBlitter::Instance().DrawStringCentred(PlayBlitter::Instance().GetSpriteId("64px"), { DISPLAY_WIDTH / 2, 50 }, "SCORE: " + std::to_string(state.score));
	
	std::vector <GameObject*> pList = GameObject::GetTypeList(GameObject::OBJ_PLAYER);
	for (GameObject* p : pList)
	{
		Player* player = dynamic_cast<Player*>(p);
				
		if (player != nullptr && player->GetIsDead())
		{
			SetMainGameState(GAMEOVER_STATE);
		}
	}
}

void GameOverStateUpdate()
{
	PlayBlitter& blit = PlayBlitter::Instance();

	if (PlayBuffer::Instance().KeyDown(VK_RETURN))
	{
		std::vector <GameObject*> oList = GameObject::GetTypeList(GameObject::OBJ_ALL);
		for (GameObject* o : oList)
		{
			o->SetActive(false);
		}
		state.score = 0;
		SetMainGameState(ACTIVE_STATE);
	}
	blit.DrawStringCentred(blit.GetSpriteId("151px"), { DISPLAY_WIDTH / 2, 50 }, "GAME OVER:");
	blit.DrawStringCentred(blit.GetSpriteId("64"), { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 }, "Press ENTER/RETURN to retry!");
	blit.DrawStringCentred(blit.GetSpriteId("64"), { DISPLAY_WIDTH / 2, 3 * DISPLAY_HEIGHT / 4 }, "Press ESCAPE to Quit.");

}

// Gets called once when the player quits the game 
void MainGameExit( void )
{
	PlayBuffer::Destroy();
	PlayBlitter::Destroy();
	PlaySpeaker::Destroy();
	GameObject::DestroyAll();
}


bool HasCollided(Point2f pos1, Point2f pos2)
{
	Vector2f d = pos2 - pos1;
	float dist = sqrt((d.x * d.x) + (d.y * d.y));
	if (dist < S_SCREEN_LIMIT)
	{
		return true;
	}
	else
	{
		return false;
	}
}



