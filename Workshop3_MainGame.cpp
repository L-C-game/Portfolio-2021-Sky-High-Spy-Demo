// You'll need to #include MainGame.h, Laser.h and Saucer.h BEFORE Play.h

#define PLAY_IMPLEMENTATION
#include "Play.h"

constexpr int DISPLAY_WIDTH = 1280;
constexpr int DISPLAY_HEIGHT = 720;
constexpr int DISPLAY_SCALE = 1;

// This needs to become part of the Laser class declaration in "Laser.h"
struct Laser
{
    // These will all become private data members
    bool active{ true };
    Point2f pos{ 0, 0 };
    Vector2f velocity{ 0, 0 };
};

// This needs to become part of the Saucer class declaration in "Saucer.h"
struct Saucer
{
    // These will all become private data members
    bool active{ true };
    Point2f pos{ 0, 0 };
    Vector2f velocity{ 0, 0 };
    float rot{ 0 };
    bool dead{ false };
};

// GameState declaration needs to go into "MainGame.h"
struct GameState
{
    float time{ 0 };
    int score{ 0 };
    int difficulty{ 1 };
    Point2f playerPos{ 640, 650 };
    std::vector< Laser > lasers;
    std::vector< Saucer > saucers;
};

GameState gState; // GameState definition stays in MainGame.cpp

void UpdatePlayer(); // Function declarations should be in "MainGame.h"
void UpdateLasers(); // Becomes Laser::UpdateAll() a static method of the Laser class declared in "Laser.h"
void SpawnSaucers(); // Becomes Saucer::SpawnWave() a static method of the Saucer class declared in "Saucer.h" 
void UpdateSaucers(); // Becomes Saucer::UpdateAll() a static method of the Saucer class declared in "Saucer.h" 
bool HasCollided( Point2f pos1, Point2f pos2 ); // Function declarations should be in "MainGame.h"

// Stays in MainGame.cpp
int WINAPI WinMain( PLAY_WINARGS_IN )
{
    PlayBuffer& buff = PlayBuffer::Instance( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE, 0 );
    PlayBlitter& blit = PlayBlitter::Instance( "Data\\Sprites\\" );
    PlaySpeaker& speak = PlaySpeaker::Instance( "Data\\Audio\\" );

    blit.SetDisplayBuffer( buff.GetDisplayBuffer(), DISPLAY_WIDTH, DISPLAY_HEIGHT );
    blit.LoadBackground( "Data\\Backgrounds\\background.png" );
    blit.CentreAllSpriteOrigins();

    speak.StartSound( "music", true );

    return buff.HandleWindows( PLAY_WINARGS_OUT, L"PlayBuffer Framework" );
}

// Stays in MainGame.cpp
bool MainGameUpdate( float elapsedTime )
{
    PlayBuffer& buff = PlayBuffer::Instance();
    PlayBlitter& blit = PlayBlitter::Instance();

    gState.time += elapsedTime;

    blit.DrawBackground();
    UpdateLasers(); // Call Laser::UpdateAll() passing through a reference to the GameState
    UpdatePlayer();
    UpdateSaucers(); // Call Saucer::UpdateAll() passing through a reference to the GameState
    blit.DrawStringCentred( blit.GetSpriteId( "105px" ), { DISPLAY_WIDTH / 2, 50 }, "SCORE: " + std::to_string( gState.score ) );
    buff.Present();

    return PlayBuffer::Instance().KeyDown( VK_ESCAPE );
}

// Stays in MainGame.cpp
void MainGameExit( void )
{
    PlayBuffer::Destroy();
    PlayBlitter::Destroy();
    PlaySpeaker::Destroy();
}

void UpdatePlayer( void ) // Stays in MainGame.cpp
{
    PlayBuffer& buff = PlayBuffer::Instance();
    PlayBlitter& blit = PlayBlitter::Instance();
    PlaySpeaker& speak = PlaySpeaker::Instance();

    if( buff.KeyDown( VK_LEFT ) )
        gState.playerPos.x -= 5;

    if( buff.KeyDown( VK_RIGHT ) )
        gState.playerPos.x += 5;

    if( buff.KeyPressed( VK_SPACE ) )
    {
        // Creating the laser instance will need to work slightly differently
        Laser l;
        l.pos = { gState.playerPos.x, gState.playerPos.y - 50 };
        l.velocity = { 0, -20 };
        gState.lasers.push_back( l );

        if( gState.score >= 100 )
            gState.score -= 100;

        speak.StartSound( "laser", false );
    }

    if( gState.saucers.size() == 0 )
        SpawnSaucers(); // Call Saucer::SpawnWave() passing through a reference to the GameState

    float yWobble = sin( gState.time * PLAY_PI ) * 3;
    blit.Draw( blit.GetSpriteId( "Rocket" ), { gState.playerPos.x, gState.playerPos.y + yWobble }, 2 * gState.time );
}

void UpdateLasers( void ) // Becomes Laser::UpdateAll() which calls Laser::Update() for each Laser in the vector
{
    PlayBlitter& blit = PlayBlitter::Instance();
    PlaySpeaker& speak = PlaySpeaker::Instance();

    // Laser::UpdateAll() will need to take a reference to the GameState as a parameter
    for( Laser& l : gState.lasers )
    {
        // Start of Laser::Update()
        // Laser::Update() will need to take a reference to the GameState as a parameter
        for( Saucer& s : gState.saucers )
        {
            if( s.dead == false && HasCollided( l.pos, s.pos ) )
            {
                s.dead = true;
                gState.score += 1000;
                speak.StartSound( "clang", false );
            }
        }

        l.pos += l.velocity;

        if( l.pos.y < 0 )
            l.active = false;

        blit.Draw( blit.GetSpriteId( "Laser" ), l.pos, 0 );
        // End of Laser::Update()
    }

    for( int n = 0; n < gState.lasers.size(); n++ )
    {
        if( gState.lasers[n].active == false )
            gState.lasers.erase( gState.lasers.begin() + n );
    }

}

void SpawnSaucers() // Becomes Saucers::SpawnSaucers() 
{
    for( int n = 0; n < gState.difficulty + 5; n++ )
    {
        Saucer s;
        s.pos = { 1500 + ( n * 100 ), 125 };
        gState.saucers.push_back( s );
    }
    gState.difficulty++;
    PlaySpeaker::Instance().StartSound( "reward", false );
}

void UpdateSaucers( void ) // Becomes Saucer::UpdateAll() which calls Saucer::Update() for each Saucer in the vector
{
    PlayBlitter& blit = PlayBlitter::Instance();

    // Saucer::UpdateAll() will need to take a reference to the GameState as a parameter
    for( Saucer& s : gState.saucers )
    {
        // Start of Saucer::Update()
        // Saucer::Update() will need to take a reference to the GameState as a parameter
        if( !s.dead )
        {
            s.velocity.x = -5 * gState.difficulty;
            s.velocity.y = sin( s.pos.x / 100 ) * 3;

            if( s.pos.x < -50 )
                s.pos = { 1500, 125 };
        }
        else
        {
            s.velocity.x = -5 * gState.difficulty;
            s.velocity.y += 1;
            s.rot += 0.1f * gState.difficulty;

            if( s.pos.y > DISPLAY_HEIGHT )
                s.active = false;
        }

        s.pos += s.velocity;

        blit.DrawRotated( blit.GetSpriteId( "Saucer" ), s.pos, 0, s.rot );
        // End of Saucer::Update()
    }

    for( int n = 0; n < gState.saucers.size(); n++ )
    {
        if( gState.saucers[n].active == false )
            gState.saucers.erase( gState.saucers.begin() + n );
    }
}

bool HasCollided( Point2f pos1, Point2f pos2 ) // Stays in MainGame.cpp
{
    Vector2f separation = pos2 - pos1;
    float dist = sqrt( ( separation.x * separation.x ) + ( separation.y * separation.y ) );
    if( dist < 50 )
        return true;
    else
        return false;
}



