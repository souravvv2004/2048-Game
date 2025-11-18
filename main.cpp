#include "Renderer2048.hpp"
#include "Game2048.hpp"

//================= Main Function =================//
// Objective: Entry point of the 2048 game application.
// input: None
// output: int - exit status code.
// Side Effects: Initializes and runs the game loop.
//====================================================//

int main()
{
    Game2048 game;
    Renderer2048 renderer(game);
    renderer.run();
    return 0;
}
