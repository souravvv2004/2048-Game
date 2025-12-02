/**
 * @file Game2048.hpp
 * @brief Core game logic definitions for the 2048 puzzle game.
 *
 * This file contains the Game2048 class which implements all game mechanics
 * including tile movement, merging, spawning new tiles, and state management.
 * The game uses a 4×4 grid where players slide tiles to combine matching
 * numbers and reach the 2048 tile.
 *
 * @author Tarundeep Singh and Saurabh Madan
 * @date November 2025
 * @version 1.0
 *
 * @par Game Rules:
 * - Tiles slide in the chosen direction until they hit a wall or another tile
 * - Two tiles with the same number merge into one with their sum
 * - After each move, a new 2 or 4 tile spawns in a random empty cell
 * - Game continues until no moves are possible or 2048 is reached
 */

#ifndef GAME2048_HPP
#define GAME2048_HPP

#include "Block.hpp"
#include <array>

/**
 * @brief Grid size constant defining the dimensions of the game board.
 *
 * The standard 2048 game uses a 4×4 grid (SIZE=4), resulting in 16 cells.
 * This constant is used throughout the codebase for array sizing and
 * loop bounds.
 */
constexpr int SIZE = 4;

/**
 * @enum Direction
 * @brief Enumeration representing the four possible movement directions.
 *
 * Used to specify which direction tiles should slide and merge when
 * the player makes a move. The direction affects how tiles are processed
 * and combined on the game board.
 */
enum class Direction
{
    LEFT,   ///< Slide all tiles to the left edge
    RIGHT,  ///< Slide all tiles to the right edge
    UP,     ///< Slide all tiles to the top edge
    DOWN    ///< Slide all tiles to the bottom edge
};

/**
 * @enum GameState
 * @brief Enumeration representing the current state of the game loop.
 *
 * The game operates as a finite state machine, transitioning between
 * these states to handle input, animation, and game logic in order.
 *
 * @par State Flow:
 * INPUT → MERGE → SLIDE → INPUT (normal gameplay)
 * INPUT → RESET → INPUT (when reset is triggered)
 */
enum class GameState
{
    INPUT,  ///< Waiting for player input (arrow keys or space for reset)
    MERGE,  ///< Processing tile merge logic after a direction is chosen
    SLIDE,  ///< Animating tile movement to new positions
    RESET   ///< Resetting the game board to initial state
};

/**
 * @class Game2048
 * @brief Main game logic class implementing the 2048 puzzle mechanics.
 *
 * This class encapsulates all game state and logic for the 2048 puzzle.
 * It manages the 4×4 grid of tiles, handles merging operations, spawns
 * new tiles, and tracks the game state for animation coordination.
 *
 * @par Architecture:
 * The class separates game logic from rendering. The Renderer2048 class
 * queries this class for game state and block positions, while this class
 * handles all game rules and state transitions.
 *
 * @par Merge Algorithm:
 * To simplify directional logic, all merges are performed as "left" merges
 * after rotating the board to the appropriate orientation. After merging,
 * the board is rotated back to its original orientation.
 *
 * @par Example Usage:
 * @code
 *     Game2048 game;                    // Creates new game with 2 tiles
 *     game.merge(Direction::LEFT);      // Merge tiles left
 *     game.spawnBlock();                // Add new tile after move
 *     Block* b = game.get(0, 0);        // Get block at row 0, col 0
 * @endcode
 */
class Game2048
{
public:
    /**
     * @brief Constructs a new Game2048 instance and initializes the game.
     *
     * Seeds the random number generator and calls reset() to set up
     * the initial game state with two randomly placed tiles.
     */
    Game2048();

    /**
     * @brief Resets the game board to its initial state.
     *
     * Clears all tiles from the board, spawns two new tiles in random
     * positions, and sets the game state to INPUT. Called at game start
     * and when the player presses the reset key (spacebar).
     */
    void reset();

    /**
     * @brief Initializes the game (alias for reset).
     *
     * Provided for API consistency. Simply calls reset() to initialize
     * the game board.
     */
    void init();

    /**
     * @brief Spawns a new tile (2 or 4) in a random empty cell.
     *
     * Finds all empty cells on the board, randomly selects one, and
     * places a new tile there. The tile has a 90% chance of being 2
     * and 10% chance of being 4. The new tile spawns with alpha=0
     * for a fade-in animation effect.
     *
     * @note Does nothing if the board is full (no empty cells).
     */
    void spawnBlock();

    /**
     * @brief Performs a merge operation in the specified direction.
     *
     * Slides all tiles in the given direction, merges adjacent tiles
     * with matching values, and slides again to fill gaps. Uses board
     * rotation to simplify the algorithm - all actual merging is done
     * as "left" merges.
     *
     * @param dir The direction to merge tiles (LEFT, RIGHT, UP, DOWN).
     * @return true if any tiles moved or merged, false if no change occurred.
     *
     * @par Algorithm:
     * 1. Rotate board so target direction becomes "left"
     * 2. For each row: slide tiles left, merge adjacent pairs, slide again
     * 3. Rotate board back to original orientation
     */
    bool merge(Direction dir);

    /**
     * @brief Gets a pointer to the block at the specified grid position.
     *
     * @param r Row index (0 to SIZE-1, top to bottom).
     * @param c Column index (0 to SIZE-1, left to right).
     * @return Pointer to the Block at position (r, c).
     *
     * @warning No bounds checking is performed. Ensure r and c are valid.
     */
    Block *get(int r, int c) { return &board[r][c]; }

    /**
     * @brief Current game state (INPUT, MERGE, SLIDE, or RESET).
     *
     * Used by the renderer to determine what actions to take and
     * what to display. Transitions are managed by processInput()
     * in the Renderer2048 class.
     */
    GameState state = GameState::INPUT;

    /**
     * @brief Direction of the last/current merge operation.
     *
     * Set when the player presses an arrow key, used by the merge
     * logic to determine which direction to move tiles.
     */
    Direction dir = Direction::LEFT;

    /**
     * @brief Flag indicating if the last merge caused any movement.
     *
     * Set by merge() and used by the renderer to determine if a new
     * tile should be spawned after the slide animation completes.
     */
    bool newBlock = false;

    /**
     * @brief Flag indicating if slide animation is currently in progress.
     *
     * Set by the renderer during drawBlocks() when any tile's current
     * position differs from its target position. Used to hold the game
     * in SLIDE state until all animations complete.
     */
    bool isSliding = false;

private:
    /**
     * @brief 2D array representing the 4×4 game board.
     *
     * board[row][column] where row 0 is the top and column 0 is the left.
     * Each Block contains the tile value and animation state.
     */
    std::array<std::array<Block, SIZE>, SIZE> board;

    /**
     * @brief Clears the merged flag on all blocks.
     *
     * Called at the start of each merge operation to reset the merge
     * animation state from the previous turn.
     */
    void clearMergedFlags();

    /**
     * @brief Slides non-empty tiles to the left in an array.
     *
     * Moves all non-zero tiles as far left as possible, filling gaps.
     * This is a helper function used during the merge operation.
     *
     * @param arr Array of pointers to blocks in a row.
     * @param moved Set to true if any tiles were moved.
     */
    void slideArray(std::array<Block *, SIZE> &arr, bool &moved);

    /**
     * @brief Merges adjacent tiles with matching values in an array.
     *
     * Combines pairs of adjacent tiles with the same value, doubling
     * the left tile and zeroing the right tile. Updates merge animation
     * state for visual feedback.
     *
     * @param arr Array of pointers to blocks in a row.
     * @param moved Set to true if any tiles were merged.
     */
    void mergeArray(std::array<Block *, SIZE> &arr, bool &moved);

    /**
     * @brief Rotates the game board 90 degrees clockwise.
     *
     * Used to transform the board so that any direction merge can be
     * performed as a left merge, simplifying the merge algorithm.
     */
    void rotate90();

    /**
     * @brief Rotates the board to prepare for a directional merge.
     *
     * Rotates the board so that merging in the specified direction
     * becomes equivalent to merging left. Called before merge operations.
     *
     * @param dir The target merge direction.
     *
     * @par Rotation Mapping:
     * - LEFT: No rotation needed
     * - DOWN: 1 clockwise rotation (90°)
     * - RIGHT: 2 clockwise rotations (180°)
     * - UP: 3 clockwise rotations (270°)
     */
    void rotateBack(Direction dir);
};

#endif // GAME2048_HPP
