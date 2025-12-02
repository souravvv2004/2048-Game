/**
 * @file 2048.hpp
 * @brief Legacy/Deprecated header file for 2048 game types and declarations.
 * 
 * @deprecated This file is no longer used in the current implementation.
 *             Please use Block.hpp for the Block struct, and Game2048.hpp
 *             for the Direction enum, SIZE constant, and game logic.
 *             This file is kept for historical reference only.
 * 
 * @warning Do not include this file in new code. It contains duplicate
 *          definitions that will cause compilation errors if used alongside
 *          the current implementation files.
 * 
 * @note The current 2048 game implementation uses:
 *       - Block.hpp: Defines the Block structure for game tiles
 *       - Game2048.hpp: Defines Direction enum, GameState enum, SIZE constant,
 *                       and the Game2048 class for game logic
 *       - Renderer2048.hpp: Defines the Renderer2048 class for rendering
 */

#ifndef GAME_2048_HPP
#define GAME_2048_HPP

#include <array>

/**
 * @brief Grid size constant for the 4×4 game board.
 * @deprecated Use SIZE from Game2048.hpp instead.
 */
constexpr int SIZE = 4;

/**
 * @brief Block structure representing a tile in the 2048 game.
 * @deprecated Use Block from Block.hpp instead.
 * 
 * Each block contains:
 * - Position and state information for the main tile
 * - Merged tile information for animation purposes
 */
struct Block
{
  bool init = false;        ///< Whether block position has been initialized for rendering
  int num = 0;              ///< Block number value (2, 4, 8, 16, ...)
  float x = 0.0f;           ///< X position on screen in pixels
  float y = 0.0f;           ///< Y position on screen in pixels  
  float alpha = 1.0f;       ///< Color alpha for fade animations (0.0 to 1.0)
  bool merged = false;      ///< Flag indicating if this block merged this turn
  int merged_num = 0;       ///< Original number before merge (for fade-out animation)
  float merged_x = 0.0f;    ///< X position of merged block (for animation)
  float merged_y = 0.0f;    ///< Y position of merged block (for animation)
  float merged_alpha = 1.0f;///< Alpha of merged block for fade-out effect
};

/**
 * @brief Direction enumeration for block merging operations.
 * @deprecated Use Direction from Game2048.hpp instead.
 */
enum class Direction
{
  LEFT,   ///< Merge tiles to the left
  RIGHT,  ///< Merge tiles to the right
  UP,     ///< Merge tiles upward
  DOWN    ///< Merge tiles downward
};

/**
 * @brief Function declarations for legacy API.
 * @deprecated These functions are not implemented. Use the Game2048 class instead.
 * 
 * The current implementation uses the Game2048 class with methods:
 * - Game2048::reset() - Initialize/reset game board
 * - Game2048::spawnBlock() - Add new 2 or 4 tile
 * - Game2048::merge(Direction) - Merge blocks in direction
 * - Game2048::get(int, int) - Access a block
 */
void init_2048();                   ///< Initialize game board (not implemented)
void spawn_block();                 ///< Add new 2 or 4 tile (not implemented)
bool merge(Direction dir);          ///< Merge blocks in given direction (not implemented)
void reset_2048();                  ///< Reset board to initial state (not implemented)
Block *get_block(int row, int col); ///< Access a block at given position (not implemented)

#endif // GAME_2048_HPP