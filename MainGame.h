#pragma once
#include "Play.h"
#include <vector>


    static int m_mainGameState{ 0 };

    static void SetMainGameState(int pState) { m_mainGameState = pState; }
    static int GetMainGameState() { return m_mainGameState; }

    void StartStateUpdate();
    void PlayStateUpdate();
    void GameOverStateUpdate();

    bool HasCollided(Point2f pos1, Point2f pos2);

    struct GameState
    {
        float time{ 0 };
        int score{ 0 };
        int difficulty{ 0 };
    };

    static enum MainGameState
    {
        START_STATE = 0,
        ACTIVE_STATE,
        GAMEOVER_STATE,
    };

    const int DISPLAY_WIDTH = 1280;
    const int DISPLAY_HEIGHT = 720;
    const int DISPLAY_SCALE = 1;

    const double pi = 3.14159265358979323846;
    const double MAX_SPEED = sqrt(9);

    static constexpr int S_SCREEN_LIMIT = 50;