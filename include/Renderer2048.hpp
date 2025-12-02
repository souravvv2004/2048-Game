/**
 * @file Renderer2048.hpp
 * @brief Rendering and window management for the 2048 game.
 *
 * This file contains the Renderer2048 class which handles all visual aspects
 * of the 2048 game including window initialization, drawing the game board,
 * rendering tiles with animations, and processing user input.
 *
 * @author Tarundeep Singh and Saurabh Madan
 * @date November 2025
 * @version 1.0
 *
 * @par Dependencies:
 * - Raylib: Graphics library for window management, drawing, and input
 * - Game2048: Core game logic class for game state and data
 *
 * @par Asset Requirements:
 * - assets/font.ttf: TrueType font for rendering tile numbers
 * - assets/2048.png: Window icon image
 */

#ifndef RENDERER2048_HPP
#define RENDERER2048_HPP

#include "Game2048.hpp"
#include <raylib.h>

/**
 * @class Renderer2048
 * @brief Handles all rendering and input processing for the 2048 game.
 *
 * This class is responsible for:
 * - Initializing and managing the game window
 * - Loading and managing assets (fonts, icons)
 * - Rendering the game board background
 * - Drawing tiles with slide and fade animations
 * - Processing keyboard input for game controls
 * - Running the main game loop
 *
 * @par Architecture:
 * The renderer is designed to be decoupled from game logic. It holds a
 * reference to a Game2048 instance and queries it for game state while
 * managing all visual presentation independently.
 *
 * @par Animation System:
 * The renderer implements two types of animations:
 * 1. Slide Animation: Tiles smoothly move from old positions to new positions
 * 2. Fade Animation: New tiles fade in, merged tiles fade out
 *
 * @par Example Usage:
 * @code
 *     Game2048 game;
 *     Renderer2048 renderer(game);
 *     renderer.run();  // Starts the game loop
 * @endcode
 */
class Renderer2048
{
public:
    /**
     * @brief Constructs the renderer and initializes the game window.
     *
     * Sets up the Raylib window, loads required assets (font and icon),
     * calculates rendering parameters based on window size, and prepares
     * for game rendering.
     *
     * @param game Reference to the Game2048 instance containing game logic.
     *
     * @throws std::runtime_error if font or icon assets cannot be loaded.
     */
    Renderer2048(Game2048 &game);

    /**
     * @brief Destructor that cleans up Raylib resources.
     *
     * Unloads the font and icon, then closes the Raylib window.
     * Called automatically when the renderer goes out of scope.
     */
    ~Renderer2048();

    /**
     * @brief Starts the main game loop.
     *
     * Enters an infinite loop that continues until the window is closed.
     * Each iteration:
     * 1. Calculates delta time for smooth animations
     * 2. Processes user input and updates game state
     * 3. Renders the current frame
     */
    void run();

private:
    /**
     * @brief Reference to the game logic instance.
     *
     * The renderer reads game state from this object and modifies
     * state based on user input. All game logic is delegated to
     * the Game2048 class.
     */
    Game2048 &game;

    /**
     * @brief Size of the game window in pixels (square window).
     *
     * Calculated as a percentage of the smaller screen dimension
     * to ensure the window fits on any monitor.
     */
    int windowSize = 0;

    /**
     * @brief Size of each tile in pixels (square tiles).
     *
     * Calculated from windowSize with consideration for padding
     * between tiles. Used for drawing tile rectangles.
     */
    float blockSize = 0;

    /**
     * @brief Padding space between tiles in pixels.
     *
     * Creates visual separation between tiles on the game board.
     * Calculated based on window size and tile size.
     */
    float padding = 0;

    /**
     * @brief Animation speed for tile sliding in pixels per second.
     *
     * Controls how fast tiles move to their new positions during
     * the slide animation. Calculated from window size.
     */
    float slideSpeed = 0;

    /**
     * @brief Animation speed for fade effects in alpha per second.
     *
     * Controls how fast tiles fade in/out during spawn and merge
     * animations. Calculated from window size.
     */
    float fadeSpeed = 0;

    /**
     * @brief Font used for rendering tile numbers.
     *
     * Loaded from assets/font.ttf during initialization.
     * Must be unloaded in destructor.
     */
    Font font;

    /**
     * @brief Window icon image.
     *
     * Loaded from assets/2048.png and set as the window icon.
     * Must be unloaded in destructor.
     */
    Image icon;

    /**
     * @brief Initializes the Raylib window and loads assets.
     *
     * Creates a centered window sized proportionally to the screen,
     * loads font and icon assets, calculates rendering parameters,
     * and sets the target frame rate.
     */
    void initWindow();

    /**
     * @brief Renders a complete frame of the game.
     *
     * Clears the screen, draws the empty grid background, then
     * draws all tiles with their current animation states.
     *
     * @param dt Delta time since last frame in seconds.
     */
    void draw(float dt);

    /**
     * @brief Draws the empty 4×4 grid background.
     *
     * Renders the placeholder rectangles for all 16 grid positions.
     * These form the background that tiles slide over.
     */
    void drawEmpty();

    /**
     * @brief Draws all tiles with animations.
     *
     * Iterates through all blocks on the game board and renders
     * each non-empty tile. Handles slide animations (position updates),
     * fade-in animations (for new tiles), and merge fade-out animations.
     * Updates the isSliding flag based on whether any animations are
     * still in progress.
     *
     * @param dt Delta time since last frame for animation calculations.
     */
    void drawBlocks(float dt);

    /**
     * @brief Processes keyboard input and updates game state.
     *
     * Handles the game state machine:
     * - INPUT state: Listens for arrow keys and spacebar
     * - MERGE state: Triggers merge operation in chosen direction
     * - SLIDE state: Waits for slide animation to complete
     * - RESET state: Resets the game board
     */
    void processInput();
};

#endif // RENDERER2048_HPP
