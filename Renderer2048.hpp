#ifndef RENDERER2048_HPP
#define RENDERER2048_HPP

#include "Game2048.hpp"
#include <raylib.h>

//================= Renderer2048 Methods =================//
// Renderer for 2048 game
// Approach: Handles drawing, input processing, and game loop.
// input: Game2048& game - reference to the game logic instance.
// output: None
// Side Effects: Manages the rendering and user interaction for the game.
//====================================================//

class Renderer2048
{
public:
    Renderer2048(Game2048 &game);
    ~Renderer2048();

    void run();

private:
    Game2048 &game;

    int windowSize = 0;
    float blockSize = 0;
    float padding = 0;
    float slideSpeed = 0;
    float fadeSpeed = 0;

    Font font;
    Image icon;

    void initWindow();
    void draw(float dt);
    void drawEmpty();
    void drawBlocks(float dt);
    void processInput();
};

#endif
