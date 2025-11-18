#include "Game2048.hpp"
#include <cstdlib>
#include <ctime>



//================= Game2048 Methods =================//
// Objective: Implement the core logic of the 2048 game.
// input: None
// output: None
// Side Effects: Modifies the game board state.
//====================================================//
Game2048::Game2048() {
    std::srand((unsigned)std::time(nullptr));  // Seed random number generator
    reset(); // Initialize the game board
}



//================= Game2048 Reset Methods =================//
// Objective: Reset the game board to the initial state.
// input: None
// output: None
// Side Effects: Resets the game board and spawns two initial blocks.
//====================================================//    
void Game2048::reset() {
    for (auto& row : board)
        for (auto& b : row)
            b = Block();

    spawnBlock(); // Spawn first block
    spawnBlock(); // Spawn second block
    state = GameState::INPUT;
}


//================= Game2048 Core Methods =================//
// Objective: Core methods to handle game logic like spawning blocks and merging.
// input: None
// output: None
// Side Effects: Modifies the game board state.
void Game2048::init() {
    reset();
}


//================= Game2048 Spawn Block Methods =================//
// Objective: Spawn a new block (2 or 4) in a random empty position.
// input: None
// output: None
// Side Effects: Modifies the game board state by adding a new block.
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
                    b.alpha = 0; // fade-in
                    return;
                }
                k++;
            }
        }
    }
}


//================= Game2048 Merge Methods =================//
// Objective: Merge blocks in the specified direction.
// input: Direction dir - the direction to merge blocks.
// output: bool - true if any blocks were moved or merged, false otherwise.
// Side Effects: Modifies the game board state by merging blocks.       


void Game2048::clearMergedFlags() {
    for (auto& row : board)
        for (auto& b : row)
            b.merged = false;
}



//================= Game2048 Helper Methods =================//
// Objective: Helper methods for sliding and merging arrays of blocks.
// input: std::array<Block*, SIZE>& arr - array of block pointers.      '   
//        bool& moved - flag to indicate if any blocks were moved.
// output: None
// Side Effects: Modifies the array of blocks and the moved flag.
//====================================================//
void Game2048::slideArray(std::array<Block*, SIZE>& arr, bool& moved) {
    for (int i = 1; i < SIZE; i++) {
        int j = i;
// Approach: Slide non-zero blocks to the left.        
        while (j > 0 && arr[j - 1]->num == 0 && arr[j]->num != 0) {
            std::swap(*arr[j - 1], *arr[j]);
            moved = true;
            j--;
        }
    }
}


//================= Game2048 Merge Array Methods =================//
// Objective: Merge adjacent blocks in the array if they have the same number.
// input: std::array<Block*, SIZE>& arr - array of block pointers.
//        bool& moved - flag to indicate if any blocks were merged.
// output: None
// Side Effects: Modifies the array of blocks and the moved flag.

void Game2048::mergeArray(std::array<Block*, SIZE>& arr, bool& moved) {
//Approach: Merge adjacent blocks with the same number.


    for (int i = 0; i < SIZE - 1; i++) {
        if (arr[i]->num != 0 && arr[i]->num == arr[i + 1]->num) {
            arr[i]->merged = true;
            arr[i]->merged_num = arr[i + 1]->num;
            arr[i + 1]->num = 0;
            arr[i]->num *= 2;
            moved = true;
            i++;
        }
    }
}


//================= Game2048 Rotation Methods =================//
// Objective: Rotate the game board 90 degrees clockwise.
// input: None  
// output: None
// Side Effects: Modifies the game board state by rotating it.`
//====================================================//
void Game2048::rotate90() {
    std::array<std::array<Block, SIZE>, SIZE> tmp;
    for (int r = 0; r < SIZE; r++)
        for (int c = 0; c < SIZE; c++)
            tmp[c][SIZE - 1 - r] = board[r][c];
    board = tmp;
}
//================= Game2048 Rotate Back Methods =================//
// Rotate back to original orientation based on direction
// input: Direction dir - the direction to rotate back from.
// output: None
// Side Effects: Modifies the game board state by rotating it back.
//====================================================//
void Game2048::rotateBack(Direction dir) {


    if (dir == Direction::LEFT) return;
    if (dir == Direction::DOWN) { rotate90(); }
    if (dir == Direction::RIGHT) { rotate90(); rotate90(); }
    if (dir == Direction::UP) { rotate90(); rotate90(); rotate90(); }
}

//================= Game2048 Merge Methods =================//
// Objective: Merge blocks in the specified direction.
// input: Direction dir - the direction to merge blocks.
// output: bool - true if any blocks were moved or merged, false otherwise.
// Side Effects: Modifies the game board state by merging blocks.
//====================================================//

bool Game2048::merge(Direction dir) {
    clearMergedFlags();
    bool moved = false;
//Approach: Rotate the board to simplify merging logic.
    // Rotate so movement is always "left"
    rotateBack(dir);

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
