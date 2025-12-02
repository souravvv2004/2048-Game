/**
 * @file Game2048.cpp
 * @brief Implementation of the Game2048 class for 2048 puzzle game logic.
 *
 * This file implements all game mechanics including:
 * - Board initialization and reset
 * - Spawning new tiles
 * - Merging tiles in any direction using board rotation
 * - Helper functions for sliding and merging tile arrays
 *
 * @author Tarundeep Singh and Saurabh Madan
 * @date November 2025
 * @version 1.0
 *
 * @par Algorithm Overview:
 * The merge algorithm simplifies directional logic by always treating
 * merges as "left" operations. For other directions, the board is rotated
 * to the appropriate orientation, the merge is performed as a left merge,
 * then the board is rotated back.
 */

#include "Game2048.hpp"
#include <cstdlib>
#include <ctime>

/**
 * @brief Constructs a new Game2048 instance.
 *
 * Seeds the random number generator with the current time for
 * randomized tile spawning, then initializes the game board
 * by calling reset() which spawns two initial tiles.
 */
Game2048::Game2048() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    reset();
}

/**
 * @brief Resets the game board to its initial state.
 *
 * Clears all tiles from the board by replacing each Block with
 * a default-constructed Block (num=0), then spawns two new tiles
 * in random positions and sets the game state to INPUT.
 *
 * @note This is called by the constructor and when the player
 *       presses the reset key (spacebar).
 */
void Game2048::reset() {
    for (auto& row : board)
        for (auto& b : row)
            b = Block();

    spawnBlock();
    spawnBlock();
    state = GameState::INPUT;
}

/**
 * @brief Initializes the game (alias for reset).
 *
 * Provided for API consistency. Simply delegates to reset()
 * to initialize the game board to its starting state.
 */
void Game2048::init() {
    reset();
}

/**
 * @brief Spawns a new tile in a random empty cell.
 *
 * Counts all empty cells (num=0), randomly selects one,
 * and places a new tile with value 2 (90% chance) or 4 (10% chance).
 * The new tile is initialized with alpha=0 to enable fade-in animation.
 *
 * @par Algorithm:
 * 1. Count empty cells
 * 2. Generate random index from 0 to emptyCount-1
 * 3. Iterate through board to find the indexed empty cell
 * 4. Place new tile with random value (2 or 4)
 *
 * @note If no empty cells exist, the function returns immediately
 *       without spawning a tile.
 */
void Game2048::spawnBlock() {
    int emptyCount = 0;
    for (auto& row : board)
        for (auto& b : row)
            if (b.num == 0) emptyCount++;

    if (emptyCount == 0) return;

    int idx = rand() % emptyCount;
    int k = 0;

    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            Block& b = board[r][c];
            if (b.num == 0) {
                if (k == idx) {
                    b.num = (rand() % 10 == 0) ? 4 : 2;
                    b.alpha = 0;  // Start transparent for fade-in animation
                    return;
                }
                k++;
            }
        }
    }
}

/**
 * @brief Clears the merged flag on all blocks.
 *
 * Called at the beginning of each merge operation to reset
 * the merge animation state from the previous turn. This ensures
 * that merge animations don't persist across multiple moves.
 */
void Game2048::clearMergedFlags() {
    for (auto& row : board)
        for (auto& b : row)
            b.merged = false;
}

/**
 * @brief Slides non-empty tiles to the left in an array.
 *
 * Moves all non-zero tiles as far left as possible by swapping
 * with empty (zero) tiles to their left. This operation fills
 * all gaps between tiles and is used before and after merging.
 *
 * @param arr Array of pointers to SIZE blocks representing a row.
 * @param moved Reference to a boolean that is set to true if any
 *              tile was moved by this operation.
 *
 * @par Algorithm:
 * For each tile starting from the second position:
 * - While there's an empty cell to the left and current cell is not empty:
 *   - Swap the current tile with the empty cell
 *   - Mark that movement occurred
 */
void Game2048::slideArray(std::array<Block*, SIZE>& arr, bool& moved) {
    for (int i = 1; i < SIZE; i++) {
        int j = i;
        while (j > 0 && arr[j - 1]->num == 0 && arr[j]->num != 0) {
            std::swap(*arr[j - 1], *arr[j]);
            moved = true;
            j--;
        }
    }
}

/**
 * @brief Merges adjacent tiles with matching values in an array.
 *
 * Combines pairs of adjacent tiles that have the same non-zero value.
 * The left tile doubles its value, and the right tile becomes empty.
 * Merge animation data is stored for visual feedback.
 *
 * @param arr Array of pointers to SIZE blocks representing a row.
 * @param moved Reference to a boolean that is set to true if any
 *              tiles were merged by this operation.
 *
 * @par Algorithm:
 * For each pair of adjacent tiles (i, i+1):
 * - If both have the same non-zero value:
 *   - Mark left tile as merged and store original value
 *   - Double the left tile's value
 *   - Set the right tile to empty (0)
 *   - Skip the next pair (increment i) to prevent chain merges
 */
void Game2048::mergeArray(std::array<Block*, SIZE>& arr, bool& moved) {
    for (int i = 0; i < SIZE - 1; i++) {
        if (arr[i]->num != 0 && arr[i]->num == arr[i + 1]->num) {
            arr[i]->merged = true;
            arr[i]->merged_num = arr[i + 1]->num;
            arr[i + 1]->num = 0;
            arr[i]->num *= 2;
            moved = true;
            i++;  // Skip next tile to prevent chain merges in same turn
        }
    }
}

/**
 * @brief Rotates the game board 90 degrees clockwise.
 *
 * Transforms the board by mapping each position (r, c) to (c, SIZE-1-r).
 * Used to convert directional merges into left merges for algorithm
 * simplification.
 *
 * @par Transformation:
 * Original position (r, c) → New position (c, SIZE-1-r)
 * This rotates the board 90° clockwise.
 */
void Game2048::rotate90() {
    std::array<std::array<Block, SIZE>, SIZE> tmp;
    for (int r = 0; r < SIZE; r++)
        for (int c = 0; c < SIZE; c++)
            tmp[c][SIZE - 1 - r] = board[r][c];
    board = tmp;
}

/**
 * @brief Rotates the board to prepare for a directional merge.
 *
 * Rotates the board so that merging in the specified direction
 * becomes equivalent to merging left. This allows the merge
 * algorithm to always use the same left-merge logic.
 *
 * @param dir The target merge direction.
 *
 * @par Rotation Mapping:
 * - LEFT: 0° rotation (no change needed)
 * - DOWN: 90° clockwise (1 rotation)
 * - RIGHT: 180° (2 rotations)
 * - UP: 270° clockwise (3 rotations)
 */
void Game2048::rotateBack(Direction dir) {
    if (dir == Direction::LEFT) return;
    if (dir == Direction::DOWN) { rotate90(); }
    if (dir == Direction::RIGHT) { rotate90(); rotate90(); }
    if (dir == Direction::UP) { rotate90(); rotate90(); rotate90(); }
}

/**
 * @brief Performs a complete merge operation in the specified direction.
 *
 * This is the main game mechanic function. It slides all tiles in the
 * given direction, merges adjacent tiles with matching values, and
 * slides again to fill gaps created by merges.
 *
 * @param dir The direction to merge tiles (LEFT, RIGHT, UP, DOWN).
 * @return true if any tiles moved or merged, false if no change occurred.
 *
 * @par Algorithm:
 * 1. Clear merge flags from previous turn
 * 2. Rotate board to convert target direction to "left"
 * 3. For each row:
 *    a. Slide tiles left to fill gaps
 *    b. Merge adjacent matching tiles
 *    c. Slide again to fill gaps from merges
 * 4. Rotate board back to original orientation
 *
 * @note Returns false if no tiles could move or merge, indicating
 *       that the move was invalid and no new tile should spawn.
 */
bool Game2048::merge(Direction dir) {
    clearMergedFlags();
    bool moved = false;

    // Rotate board so movement is always "left"
    rotateBack(dir);

    // Process each row with slide-merge-slide pattern
    for (int r = 0; r < SIZE; r++) {
        std::array<Block*, SIZE> arr;
        for (int c = 0; c < SIZE; c++)
            arr[c] = &board[r][c];

        slideArray(arr, moved);
        mergeArray(arr, moved);
        slideArray(arr, moved);
    }

    // Rotate back to original orientation
    if (dir == Direction::LEFT) {}
    else if (dir == Direction::DOWN) { rotate90(); }
    else if (dir == Direction::RIGHT) { rotate90(); rotate90(); }
    else if (dir == Direction::UP) { rotate90(); rotate90(); rotate90(); }

    return moved;
}
