

# 2048 Game in C++ (Raylib)

A C++ implementation of the popular 2048 game using **Raylib** for graphics and animations.

---

## **Table of Contents**

- [Overview](#overview)
- [Folder Structure](#folder-structure)
- [Dependencies](#dependencies)
- [Compilation & Running](#compilation--running)
- [Code Structure](#code-structure)
- [Game Logic](#game-logic)
- [Rendering & Animations](#rendering--animations)
- [Include Guards](#include-guards)
- [License](#license)

---

## **Overview**

This project recreates the 2048 puzzle game in C++ with smooth animations, sliding tiles, and merging effects.

- Uses **Raylib** for rendering graphics, text, and handling input
- Implements **game logic** separately from rendering for clarity and maintainability
- Supports keyboard input (arrow keys) and reset (spacebar)

---

## **Folder Structure**
```

2048/
├── assets/ # Images, fonts, icons used in the game
├── include/ # Header files (.h / .hpp)
│ └── 2048.h # Game logic declarations
├── src/ # Source code files (.cpp)
│ └── Renderer2048.cpp # Rendering, animations, and main loop
├── Makefile # Optional build automation
├── obj/ # Object files (if compiled using Makefile)
└── README.md # Documentation

````

---

## **Dependencies**

- C++17 compatible compiler (`g++`)
- Raylib graphics library
- Linux build tools: `make`, `cmake` (if needed)

**Install Raylib on Ubuntu / Linux:**

```bash
sudo apt update
sudo apt install build-essential g++ cmake libraylib-dev
````

> Note: If `libraylib-dev` is not found, you may need to build Raylib from source.

---

## **Compilation & Running**

1. **Navigate to project directory**:

```bash
cd ~/2048
```

2. **Compile with g++**:

```bash
g++ -std=c++17 src/*.cpp -Iinclude -o game -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

3. **Run the game**:

```bash
./game
```

---

## **Code Structure**

### 1. `include/2048.h` (Game Logic)

- **Structs:**

  ```cpp
  struct Block {
      bool init;
      int num;
      float x, y, alpha;
      bool merged;
      int merged_num;
      float merged_x, merged_y, merged_alpha;
  };
  ```

- **Enums:**

  ```cpp
  enum Direction { LEFT, RIGHT, UP, DOWN };
  ```

- **Functions:**

  - `init_2048()` → Initialize game board
  - `spawn_block()` → Spawn a 2 or 4 in an empty cell
  - `merge(Direction dir)` → Merge tiles based on input
  - `reset_2048()` → Reset the game board
  - `get_block(int row, int col)` → Access a block

---

### 2. `src/Renderer2048.cpp` (Rendering & Main Loop)

**Responsibilities:**

- Initialize window, font, icon, and animation settings
- Draw background and blocks
- Handle keyboard input and game state transitions
- Animate sliding and merging tiles
- Maintain main game loop until window is closed

**Key Functions:**

- `draw_empty_blocks()` → Draws the empty 4x4 grid
- `rendernum(int num, float x, float y)` → Centers and renders numbers
- `draw_blocks(float dt)` → Animates and draws active tiles
- `manage_gamestate()` → Updates game state (INPUT, MERGE, SLIDE, RESET)
- `render(float dt)` → Renders full frame
- `init_renderer()` → Window setup and assets loading
- `gameloop()` → Main game loop
- `end_renderer()` → Cleanup

---

## **Game Logic**

- **States:**

  - INPUT → Waiting for arrow keys or reset
  - MERGE → Merges tiles based on direction
  - SLIDE → Animates tile movement
  - RESET → Resets board to initial state

- **Animations:**

  - **Fade-in / Fade-out** for merged tiles
  - **Sliding** of tiles to their new positions
  - Uses `BLOCK_SIZE`, `PADDING`, `SLIDE_SPEED`, and `FADE_SPEED`

---

## **Include Guards in C++**

Prevent multiple inclusions of the same header:

```cpp
#ifndef GAME_2048_H
#define GAME_2048_H

// ... header contents ...

#endif
```

Or modern alternative:

```cpp
#pragma once
```

---

## **License**

This project is open-source and free to use for learning and personal projects.

---
