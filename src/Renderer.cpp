/**
 * @file Renderer.cpp
 * @brief Implementation of the Renderer2048 class for game visualization.
 *
 * This file implements all rendering functionality for the 2048 game including:
 * - Window initialization and asset loading
 * - Main game loop execution
 * - Input processing and game state management
 * - Drawing the game board background
 * - Rendering tiles with slide and fade animations
 *
 * @author Tarundeep Singh and Saurabh Madan
 * @date November 2025
 * @version 1.0
 *
 * @par Dependencies:
 * - Raylib: Graphics library for rendering and input
 * - Game2048: Core game logic class
 * - cmath: For std::min and std::max in animations
 * - iostream: For error output
 */

#include "Renderer2048.hpp"
#include <iostream>
#include <cmath>

//==============================================================================
// Color Definitions
//==============================================================================

/**
 * @brief Background color for the game window.
 * A warm beige/tan color matching the classic 2048 aesthetic.
 */
static const Color BG_COL = {187, 173, 160, 255};

/**
 * @brief Color for empty grid cells.
 * A slightly lighter shade than the background for visibility.
 */
static const Color EMPTY_BLOCK_COL = {204, 192, 180, 255};

//==============================================================================
// Rendering Constants
//==============================================================================

/**
 * @brief Window size as a percentage of the smaller screen dimension.
 * Set to 40% to ensure the window fits comfortably on any screen.
 */
constexpr float SCREEN_WINDOW_PERC = 0.4f;

/**
 * @brief Tile size as a percentage of cell size.
 * Remaining space (10%) is used for padding between tiles.
 */
constexpr float CELL_BLOCK_PERC = 0.9f;

/**
 * @brief Corner roundness factor for rounded rectangles.
 * Higher values create more rounded corners (0.0 to 1.0).
 */
constexpr float ROUNDED_CORNERS = 0.1f;

/**
 * @brief Multiplier for slide animation speed.
 * Higher values result in faster tile movement.
 */
constexpr float SLIDE_SPEED_FACTOR = 3.5f;

/**
 * @brief Multiplier for fade animation speed.
 * Controls the rate of opacity changes for spawn/merge effects.
 */
constexpr float FADE_SPEED_FACTOR = 0.004f;

/**
 * @brief Target frames per second for the game loop.
 */
constexpr int FPS = 60;

/**
 * @brief Relative path to the font file for tile numbers.
 */
static const char *FONT_PATH = "assets/font.ttf";

/**
 * @brief Relative path to the window icon image.
 */
static const char *ICON_PATH = "assets/2048.png";

//==============================================================================
// Constructor and Destructor
//==============================================================================

/**
 * @brief Constructs the renderer and initializes the game window.
 *
 * @param gameRef Reference to the Game2048 instance for game state access.
 */
Renderer2048::Renderer2048(Game2048 &gameRef)
    : game(gameRef)
{
    initWindow();
}

/**
 * @brief Destructor that releases all Raylib resources.
 *
 * Unloads the font texture, frees the icon image memory,
 * and closes the Raylib window.
 */
Renderer2048::~Renderer2048()
{
    UnloadFont(font);
    UnloadImage(icon);
    CloseWindow();
}

//==============================================================================
// Window Initialization
//==============================================================================

/**
 * @brief Initializes the game window, loads assets, and calculates layout.
 *
 * This function performs the following setup:
 * 1. Creates a temporary window to detect monitor dimensions
 * 2. Resizes and centers the window based on screen size
 * 3. Calculates tile size and padding for the game grid
 * 4. Computes animation speeds based on window dimensions
 * 5. Loads font and icon assets
 * 6. Configures rendering options (texture filter, FPS)
 *
 * @throws Exits with code 1 if font or icon assets fail to load.
 */
void Renderer2048::initWindow()
{
    // Create temporary window to access monitor information
    InitWindow(0, 0, "2048");

    // Get monitor dimensions for responsive sizing
    int monitor = GetCurrentMonitor();
    int screenW = GetMonitorWidth(monitor);
    int screenH = GetMonitorHeight(monitor);

    // Calculate window size as percentage of smaller dimension
    int minDim = (screenW < screenH) ? screenW : screenH;
    windowSize = static_cast<int>(minDim * SCREEN_WINDOW_PERC);

    // Center window on screen
    int winX = (screenW - windowSize) / 2;
    int winY = (screenH - windowSize) / 2;

    SetWindowSize(windowSize, windowSize);
    SetWindowPosition(winX, winY);

    // Calculate grid cell and tile dimensions
    float cellSize = static_cast<float>(windowSize) / SIZE;
    blockSize = CELL_BLOCK_PERC * cellSize;
    padding = ((cellSize - blockSize) * SIZE) / (SIZE + 1);

    // Calculate animation speeds based on window size
    slideSpeed = windowSize * SLIDE_SPEED_FACTOR;
    fadeSpeed = windowSize * FADE_SPEED_FACTOR;

    // Load font and icon assets
    font = LoadFontEx(FONT_PATH, 128, nullptr, 0);
    icon = LoadImage(ICON_PATH);

    // Validate asset loading
    if (!IsFontValid(font) || !IsImageValid(icon))
    {
        std::cerr << "Error: Could not load font or icon.\n";
        exit(1);
    }

    // Configure window and rendering options
    SetWindowIcon(icon);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    SetTargetFPS(FPS);
}

//==============================================================================
// Main Game Loop
//==============================================================================

/**
 * @brief Runs the main game loop until the window is closed.
 *
 * Each frame:
 * 1. Calculates delta time for frame-independent animations
 * 2. Processes keyboard input and updates game state
 * 3. Renders the current frame
 *
 * The loop continues until WindowShouldClose() returns true
 * (window closed via X button, Alt+F4, or similar).
 */
void Renderer2048::run()
{
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        processInput();
        draw(dt);
    }
}

//==============================================================================
// Input Processing
//==============================================================================

/**
 * @brief Processes keyboard input and manages game state transitions.
 *
 * Implements the game state machine:
 *
 * @par INPUT State:
 * - Arrow keys: Set direction and transition to MERGE state
 * - Spacebar: Transition to RESET state
 *
 * @par MERGE State:
 * - Executes merge operation in chosen direction
 * - Records whether any tiles moved for spawn decision
 * - Transitions to SLIDE state
 *
 * @par SLIDE State:
 * - Waits for slide animations to complete (isSliding=false)
 * - Spawns new tile if move was valid
 * - Transitions to INPUT state
 *
 * @par RESET State:
 * - Resets game board to initial state
 * - Transitions to INPUT state
 */
void Renderer2048::processInput()
{
    if (game.state == GameState::INPUT)
    {
        // Handle directional input
        if (IsKeyPressed(KEY_LEFT))
        {
            game.dir = Direction::LEFT;
            game.state = GameState::MERGE;
        }
        else if (IsKeyPressed(KEY_RIGHT))
        {
            game.dir = Direction::RIGHT;
            game.state = GameState::MERGE;
        }
        else if (IsKeyPressed(KEY_UP))
        {
            game.dir = Direction::UP;
            game.state = GameState::MERGE;
        }
        else if (IsKeyPressed(KEY_DOWN))
        {
            game.dir = Direction::DOWN;
            game.state = GameState::MERGE;
        }
        else if (IsKeyPressed(KEY_SPACE))
        {
            game.state = GameState::RESET;
        }
    }
    else if (game.state == GameState::MERGE)
    {
        // Execute merge and record result
        game.newBlock = game.merge(game.dir);
        game.state = GameState::SLIDE;
    }
    else if (game.state == GameState::SLIDE)
    {
        // Wait for animations to complete
        if (!game.isSliding)
        {
            if (game.newBlock)
                game.spawnBlock();
            game.state = GameState::INPUT;
        }
    }
    else if (game.state == GameState::RESET)
    {
        game.reset();
        game.state = GameState::INPUT;
    }
}

//==============================================================================
// Drawing Functions
//==============================================================================

/**
 * @brief Renders a complete frame of the game.
 *
 * @param dt Delta time since last frame in seconds.
 */
void Renderer2048::draw(float dt)
{
    BeginDrawing();
    ClearBackground(BG_COL);

    drawEmpty();
    drawBlocks(dt);

    EndDrawing();
}

/**
 * @brief Draws the empty 4×4 grid background.
 *
 * Renders rounded rectangle placeholders for all 16 grid positions.
 * These provide visual structure and show where tiles can be placed.
 */
void Renderer2048::drawEmpty()
{
    for (int r = 0; r < SIZE; r++)
    {
        for (int c = 0; c < SIZE; c++)
        {
            float x = c * blockSize + padding * (c + 1);
            float y = r * blockSize + padding * (r + 1);

            Rectangle rect{x, y, blockSize, blockSize};
            DrawRectangleRounded(rect, ROUNDED_CORNERS, 10, EMPTY_BLOCK_COL);
        }
    }
}

//==============================================================================
// Color Helper Functions
//==============================================================================

/**
 * @brief Returns the background color for a tile based on its value.
 *
 * Each power of 2 has a distinct color following the classic 2048 palette:
 * - Low values (2, 4): Light beige tones
 * - Medium values (8-64): Orange to red gradient
 * - High values (128-2048): Yellow/gold gradient
 * - Beyond 2048: Dark gray
 *
 * @param num The tile's numeric value.
 * @return Color for the tile background.
 */
static inline Color getBlockColor(int num)
{
    switch (num)
    {
    case 2:
        return {238, 228, 218, 255};
    case 4:
        return {237, 224, 200, 255};
    case 8:
        return {242, 177, 121, 255};
    case 16:
        return {245, 149, 99, 255};
    case 32:
        return {246, 124, 95, 255};
    case 64:
        return {246, 94, 59, 255};
    case 128:
        return {237, 207, 114, 255};
    case 256:
        return {237, 204, 97, 255};
    case 512:
        return {237, 200, 80, 255};
    case 1024:
        return {237, 197, 63, 255};
    case 2048:
        return {237, 194, 46, 255};
    default:
        return {60, 58, 50, 255};  // Dark color for values beyond 2048
    }
}

/**
 * @brief Returns the text color for a tile based on its value.
 *
 * Low value tiles (2, 4) use dark text for contrast against
 * their light backgrounds. Higher values use white text.
 *
 * @param num The tile's numeric value.
 * @return Color for the tile's number text.
 */
static inline Color getTextColor(int num)
{
    return (num <= 4) ? Color{119, 110, 101, 255} : RAYWHITE;
}

//==============================================================================
// Tile Rendering with Animation
//==============================================================================

/**
 * @brief Draws all tiles with slide and fade animations.
 *
 * This function iterates through the game board and renders each
 * non-empty tile. It handles three types of animations:
 *
 * @par Fade-in Animation:
 * New tiles spawn with alpha=0 and gradually increase to alpha=1,
 * creating a smooth appearance effect.
 *
 * @par Slide Animation:
 * Tiles smoothly move from their current position to their target
 * position (determined by grid coordinates). The isSliding flag
 * is set while any tile is still moving.
 *
 * @par Merge Fade-out Animation:
 * When a tile absorbs another, a "ghost" of the absorbed tile
 * fades out at its last position for visual feedback.
 *
 * @param dt Delta time since last frame for animation calculations.
 *
 * @par Position Calculation:
 * Target position is calculated from grid coordinates as:
 * x = col * blockSize + padding * (col + 1)
 * y = row * blockSize + padding * (row + 1)
 *
 * @par Font Sizing:
 * Font size decreases for larger numbers to fit within the tile.
 * Base size is blockSize/2.2, reduced by 8% per digit beyond 2.
 */
void Renderer2048::drawBlocks(float dt)
{
    game.isSliding = false;

    for (int r = 0; r < SIZE; r++)
    {
        for (int c = 0; c < SIZE; c++)
        {
            Block *b = game.get(r, c);
            if (b->num == 0)
                continue;

            // Initialize screen position on first draw
            if (!b->init)
            {
                b->x = c * blockSize + padding * (c + 1);
                b->y = r * blockSize + padding * (r + 1);
                b->init = true;
            }

            Color blockCol = getBlockColor(b->num);

            // Fade-in animation for new tiles
            if (b->alpha < 1.0f)
            {
                b->alpha += fadeSpeed * dt;
                if (b->alpha > 1)
                    b->alpha = 1;
                blockCol.a = static_cast<unsigned char>(blockCol.a * b->alpha);
            }

            // Merged tile fade-out animation
            if (b->merged)
            {
                b->merged_alpha -= fadeSpeed * dt;
                if (b->merged_alpha <= 0)
                {
                    b->merged_alpha = 0;
                    b->merged = false;
                }

                Color mergedCol = getBlockColor(b->merged_num);
                mergedCol.a = static_cast<unsigned char>(mergedCol.a * b->merged_alpha);

                Rectangle rect{b->merged_x, b->merged_y, blockSize, blockSize};
                DrawRectangleRounded(rect, ROUNDED_CORNERS, 20, mergedCol);
            }

            // Calculate target position from grid coordinates
            float tx = c * blockSize + padding * (c + 1);
            float ty = r * blockSize + padding * (r + 1);

            // Slide animation - move toward target position
            if (b->x != tx)
            {
                game.isSliding = true;
                if (b->x < tx)
                    b->x = std::min(b->x + slideSpeed * dt, tx);
                else
                    b->x = std::max(b->x - slideSpeed * dt, tx);
            }

            if (b->y != ty)
            {
                game.isSliding = true;
                if (b->y < ty)
                    b->y = std::min(b->y + slideSpeed * dt, ty);
                else
                    b->y = std::max(b->y - slideSpeed * dt, ty);
            }

            // Draw the tile background
            Rectangle rect{b->x, b->y, blockSize, blockSize};
            DrawRectangleRounded(rect, ROUNDED_CORNERS, 20, blockCol);

            // Draw the tile number with dynamic font sizing
            std::string text = std::to_string(b->num);

            float baseSize = blockSize / 2.2f;
            float fontSize = baseSize - (text.length() - 2) * (blockSize * 0.08f);

            Vector2 size = MeasureTextEx(font, text.c_str(), fontSize, 0);
            Vector2 pos{b->x + (blockSize - size.x) / 2.0f,
                        b->y + (blockSize - size.y) / 2.0f};

            DrawTextEx(font, text.c_str(), pos, fontSize, 0, getTextColor(b->num));
        }
    }
}
