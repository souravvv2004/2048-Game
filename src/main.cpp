/**
 * @file main.cpp
 * @brief Entry point for the 2048 puzzle game application.
 *
 * This file contains the main() function which initializes the game
 * components and starts the game loop. The application uses a simple
 * architecture with separation between game logic (Game2048) and
 * rendering (Renderer2048).
 *
 * @author Tarundeep Singh and Saurabh Madan
 * @date November 2025
 * @version 1.0
 *
 * @par Build Instructions:
 * @code
 *     g++ -std=c++17 src/*.cpp -Iinclude -o game -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
 * @endcode
 *
 * @par Controls:
 * - Arrow Keys: Slide tiles in the corresponding direction
 * - Spacebar: Reset the game
 * - Close Window: Exit the application
 *
 * @see Game2048 For game logic implementation
 * @see Renderer2048 For rendering and input handling
 */

#include "Renderer2048.hpp"
#include "Game2048.hpp"

/**
 * @brief Application entry point.
 *
 * Creates the game logic instance (Game2048) and renderer instance
 * (Renderer2048), then starts the main game loop. The game loop
 * continues until the window is closed by the user.
 *
 * @return int Exit status code (0 for success).
 *
 * @par Execution Flow:
 * 1. Game2048 constructor: Seeds RNG, initializes empty board, spawns 2 tiles
 * 2. Renderer2048 constructor: Creates window, loads assets, calculates layout
 * 3. run(): Enters main loop (input → update → render) until window closes
 * 4. Destructors: Clean up Raylib resources automatically
 */
int main()
{
    Game2048 game;
    Renderer2048 renderer(game);
    renderer.run();
    return 0;
}
