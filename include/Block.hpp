/**
 * @file Block.hpp
 * @brief Defines the Block structure for the 2048 game tiles.
 *
 * This file contains the Block structure which represents a single tile
 * on the 2048 game board. Each block tracks its value, screen position,
 * animation state, and merge information for smooth visual transitions.
 *
 * @author Tarundeep Singh and Saurabh Madan
 * @date November 2025
 * @version 1.0
 */

#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <raylib.h>

/**
 * @struct Block
 * @brief Represents a single tile (block) in the 2048 game grid.
 *
 * The Block structure contains all the data needed to:
 * - Track the tile's numeric value (2, 4, 8, 16, ..., 2048)
 * - Manage screen position for rendering
 * - Handle slide and fade animations
 * - Support merge animations when two tiles combine
 *
 * @note A block with num=0 represents an empty cell on the game board.
 *
 * @par Animation System:
 * The block uses two sets of position/alpha values:
 * - Primary (x, y, alpha): Current block's rendering state
 * - Merged (merged_x, merged_y, merged_alpha): Ghost of absorbed block
 *
 * @par Merge Animation:
 * When two blocks merge, the surviving block gains the combined value,
 * while temporarily displaying a "ghost" of the absorbed block that
 * fades out for visual feedback.
 *
 * @par Example Usage:
 * @code
 *     Block tile;
 *     tile.num = 2;           // Set tile value
 *     tile.x = 100.0f;        // Set x position
 *     tile.y = 100.0f;        // Set y position
 *     tile.init = true;       // Mark as initialized
 * @endcode
 */
struct Block {
    /**
     * @brief Flag indicating if the block's screen position has been initialized.
     *
     * Set to true after the block's (x, y) position is first calculated
     * based on its grid position. Used to determine if initial positioning
     * is needed during rendering.
     */
    bool init = false;

    /**
     * @brief The numeric value displayed on the block (2, 4, 8, 16, ..., 2048).
     *
     * A value of 0 indicates an empty cell with no block present.
     * Values are always powers of 2, starting from 2.
     */
    int num = 0;

    /**
     * @brief X coordinate of the block's top-left corner in screen pixels.
     */
    float x = 0;

    /**
     * @brief Y coordinate of the block's top-left corner in screen pixels.
     */
    float y = 0;

    /**
     * @brief Alpha (opacity) value for fade-in animation (0.0 to 1.0).
     *
     * New blocks spawn with alpha=0 and fade in to alpha=1.
     * Used to create a smooth appearance animation when new tiles spawn.
     */
    float alpha = 1.0f;

    /**
     * @brief Flag indicating if this block absorbed another block this turn.
     *
     * When true, the merged_* fields contain information about the
     * absorbed block for fade-out animation purposes.
     */
    bool merged = false;

    /**
     * @brief The numeric value of the absorbed block (before merge).
     *
     * Used to render the "ghost" of the absorbed block during the
     * fade-out animation after a merge operation.
     */
    int merged_num = 0;

    /**
     * @brief X coordinate where the absorbed block was located.
     */
    float merged_x = 0;

    /**
     * @brief Y coordinate where the absorbed block was located.
     */
    float merged_y = 0;

    /**
     * @brief Alpha value for the fade-out animation of absorbed block (1.0 to 0.0).
     *
     * Decreases over time to create a smooth disappearance effect
     * for the block that was absorbed during a merge.
     */
    float merged_alpha = 1.0f;

    /**
     * @brief Default constructor initializes all members to default values.
     *
     * Creates an empty block (num=0) with default position and animation state.
     */
    Block() = default;
};

#endif // BLOCK_HPP
