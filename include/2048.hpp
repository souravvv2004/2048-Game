#ifndef GAME_2048_HPP
#define GAME_2048_HPP

#include <array>

// Grid size (4×4)
constexpr int SIZE = 4;

// 2048 block struct.
struct Block
{
  bool init = false; // Block position initialized?
  int num = 0;       // Block number (2, 4, 8, ...)

  float x = 0.0f, y = 0.0f; // Position on screen
  float alpha = 1.0f;       // Color alpha

  bool merged = false; // Did this block merge?
  int merged_num = 0;  // Number after merge

  float merged_x = 0.0f,
        merged_y = 0.0f,
        merged_alpha = 1.0f; // Merged block screen data
};

// Direction for merging.
enum class Direction
{
  LEFT,
  RIGHT,
  UP,
  DOWN
};

// Function declarations
void init_2048();                   // Initialize game board
void spawn_block();                 // Add new 2 or 4
bool merge(Direction dir);          // Merge blocks
void reset_2048();                  // Reset board
Block *get_block(int row, int col); // Access a block

#endif

gameloop();
end_renderer();

Summary: This file is the frontend — drawing the grid, handling animations, input, and the main loop.