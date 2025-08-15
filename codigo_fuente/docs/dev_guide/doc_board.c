/**
 * @file bs_board.c
 * @brief Implementation of board-related logic for the Battleship game.
 *
 * This file contains functions for validating ship placement, checking attack ranges,
 * managing board state, and handling errors for the Battleship game.
 *
 * Functions:
 * - in_range: Checks if given row and column are within the board boundaries.
 * - first_coord_is_free: Checks if the initial coordinate for ship placement is free (water).
 * - is_straight: Validates if the ship placement is straight (horizontal or vertical) and sets direction.
 * - dim_match_index: Checks if the ship's placement matches its expected size.
 * - end_cell_is_water: Ensures all cells along the ship's path are water (no overlap).
 * - get_remain_fleet_cells: Counts the remaining ship cells on the player's board.
 * - attack_in_range: Validates if an attack is within the allowed range, considering special cards.
 * - getEnemyCellsInRow: Counts the number of enemy ship cells in a given row.
 * - getEnemyCellsInCol: Counts the number of enemy ship cells in a given column.
 * - cell_is_valid_to_shot: Checks if a cell can be targeted for a shot (not previously shot).
 *
 * Error handling is performed via the codeError function, which reports specific issues such as
 * out-of-range coordinates, overlapping ships, invalid directions, and repeated shots.
 *
 * @param player Pointer to the player structure.
 * @param enemy Pointer to the enemy player structure.
 * @param ship_idx Index of the ship being placed or validated.
 * @param cc_RowIn Starting row coordinate.
 * @param cc_RowEnd Ending row coordinate.
 * @param cc_ColIn Starting column coordinate.
 * @param cc_ColEnd Ending column coordinate.
 * @param cc_Row Row coordinate for attack or validation.
 * @param cc_Col Column coordinate for attack or validation.
 */