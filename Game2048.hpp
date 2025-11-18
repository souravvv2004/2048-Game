#ifndef GAME2048_HPP
#define GAME2048_HPP

#include "Block.hpp"
#include <array>

// Grid size
//====================================================//
// OBJECTIVE: Define the size of the game board grid.
// APPROACH: Use a constant expression for grid size.
//====================================================//
constexpr int SIZE = 4;

enum class Direction
{
    LEFT,
    RIGHT,
    UP,
    DOWN
};

enum class GameState
{
    INPUT,
    MERGE,
    SLIDE,
    RESET
};

class Game2048
{
public:
    Game2048();

    void reset();
    void init();
    void spawnBlock();
    bool merge(Direction dir);

    Block *get(int r, int c) { return &board[r][c]; }

    GameState state = GameState::INPUT;
    Direction dir = Direction::LEFT;
    bool newBlock = false;
    bool isSliding = false;

private:
    std::array<std::array<Block, SIZE>, SIZE> board;

    void clearMergedFlags();
    void slideArray(std::array<Block *, SIZE> &arr, bool &moved);
    void mergeArray(std::array<Block *, SIZE> &arr, bool &moved);
    void rotate90();
    void rotateBack(Direction dir);
};

#endif
