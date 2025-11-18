#include "Renderer2048.hpp"
#include <iostream>
#include <cmath>

//==--------------------------------------------------
// Colors
//==--------------------------------------------------

static const Color BG_COL = {187, 173, 160, 255};
static const Color EMPTY_BLOCK_COL = {204, 192, 180, 255};

//==--------------------------------------------------
// Constants
//==--------------------------------------------------

constexpr float SCREEN_WINDOW_PERC = 0.4f;
constexpr float CELL_BLOCK_PERC = 0.9f;
constexpr float ROUNDED_CORNERS = 0.1f;
constexpr float SLIDE_SPEED_FACTOR = 3.5f;
constexpr float FADE_SPEED_FACTOR = 0.004f;
constexpr int FPS = 60;

// Asset paths
static const char *FONT_PATH = "assets/font.ttf";
static const char *ICON_PATH = "assets/2048.png";

Renderer2048::Renderer2048(Game2048 &gameRef)
    : game(gameRef)
{
    initWindow();
}

Renderer2048::~Renderer2048()
{
    UnloadFont(font);
    UnloadImage(icon);
    CloseWindow();
}

// --------------------------------------------------
// Objective: Initialize the game window and related parameters.
// input: None
// output: None
// Side Effects: Sets up the game window, font, icon, and rendering parameters.
// --------------------------------------------------
void Renderer2048::initWindow()
{
    InitWindow(0, 0, "2048");

    int monitor = GetCurrentMonitor();
    int screenW = GetMonitorWidth(monitor);
    int screenH = GetMonitorHeight(monitor);

    int minDim = (screenW < screenH) ? screenW : screenH;
    windowSize = static_cast<int>(minDim * SCREEN_WINDOW_PERC);

    int winX = (screenW - windowSize) / 2;
    int winY = (screenH - windowSize) / 2;

    SetWindowSize(windowSize, windowSize);
    SetWindowPosition(winX, winY);

    float cellSize = static_cast<float>(windowSize) / SIZE;
    blockSize = CELL_BLOCK_PERC * cellSize;
    padding = ((cellSize - blockSize) * SIZE) / (SIZE + 1);

    slideSpeed = windowSize * SLIDE_SPEED_FACTOR;
    fadeSpeed = windowSize * FADE_SPEED_FACTOR;

    font = LoadFontEx(FONT_PATH, 128, nullptr, 0);
    icon = LoadImage(ICON_PATH);

    if (!IsFontValid(font) || !IsImageValid(icon))
    {
        std::cerr << "Error: Could not load font or icon.\n";
        exit(1);
    }

    SetWindowIcon(icon);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    SetTargetFPS(FPS);
}
// --------------------------------------------------
// Main Game Loop
// Objective: Run the main game loop handling input and rendering.
// input: None
// output: None
// Side Effects: Continuously processes input and renders the game until the window is closed.
// --------------------------------------------------

void Renderer2048::run()
{
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        processInput();
        draw(dt);
    }
}

// --------------------------------------------------
// Input Processing
// Objective: Handle user input for game actions.
// input: None
// output: None
// Side Effects: Modifies the game state based on user input.
// --------------------------------------------------
void Renderer2048::processInput()
{

    // Approach: Handle key presses to control game state.

    if (game.state == GameState::INPUT)
    {
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
        game.newBlock = game.merge(game.dir);
        game.state = GameState::SLIDE;
    }
    else if (game.state == GameState::SLIDE)
    {
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

// --------------------------------------------------
// Drawing
// Objective: Render the game state to the window.
// input: float dt - delta time since last frame.
// output: None
// Side Effects: Draws the current game state including blocks and animations.
// --------------------------------------------------
void Renderer2048::draw(float dt)
{

    // Approach: Clear the screen and draw the game board and blocks.
    BeginDrawing();
    ClearBackground(BG_COL);

    drawEmpty();
    drawBlocks(dt);

    EndDrawing();
}

// --------------------------------------------------
// Drawing empty grid
// Objective: Draw the empty grid background.
// input: None
// output: None
// Side Effects: Renders the empty grid on the screen.
// --------------------------------------------------
void Renderer2048::drawEmpty()
{

    // Approach: Draw empty blocks in a grid layout.

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

// --------------------------------------------------
// Color Helpers
// Objective: Get colors for blocks based on their numbers.
// input: int num - block number.
// output: Color - corresponding color for the block.
// Side Effects: None
//--------------------------------------------------//

static inline Color getBlockColor(int num)
{

    // Approach: Return specific colors for known block numbers, default otherwise.

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
        return {60, 58, 50, 255};
    }
}

static inline Color getTextColor(int num)
{
    return (num <= 4) ? Color{119, 110, 101, 255} : RAYWHITE;
}

// --------------------------------------------------
// Drawing blocks with animation
// Objective: Draw blocks with slide and fade animations.
// input: float dt - delta time since last frame.
// output: None
// Side Effects: Renders blocks with animations based on their state.
// --------------------------------------------------
void Renderer2048::drawBlocks(float dt)
{
    game.isSliding = false;

    // Approach: Iterate through blocks and draw them with appropriate animations.

    for (int r = 0; r < SIZE; r++)
    {
        for (int c = 0; c < SIZE; c++)
        {

            Block *b = game.get(r, c);
            if (b->num == 0)
                continue;

            // Initialize screen pos on first draw
            if (!b->init)
            {
                b->x = c * blockSize + padding * (c + 1);
                b->y = r * blockSize + padding * (r + 1);
                b->init = true;
            }

            Color blockCol = getBlockColor(b->num);

            // Fade-in animation
            if (b->alpha < 1.0f)
            {
                b->alpha += fadeSpeed * dt;
                if (b->alpha > 1)
                    b->alpha = 1;
                blockCol.a = static_cast<unsigned char>(blockCol.a * b->alpha);
            }

            // Merged fade-out animation
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

            // Slide animation target
            float tx = c * blockSize + padding * (c + 1);
            float ty = r * blockSize + padding * (r + 1);

            // Slide movement
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

            Rectangle rect{b->x, b->y, blockSize, blockSize};
            DrawRectangleRounded(rect, ROUNDED_CORNERS, 20, blockCol);

            // Draw number
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
