#include "bs_ship.h"

bool isShipHere(struct player *player, int index, int ship_cell, int fila, int columna) {
    return player->ships[index].status[ship_cell][0] == fila
        && player->ships[index].status[ship_cell][1] == columna;
}

bool isEnemyShipHere(struct player *player, int index, int ship_cell, struct player *enemy) {
    return enemy->ships[index].status[ship_cell][CC_ROW] == player->prevRowInput
        && enemy->ships[index].status[ship_cell][CC_COLUMN] == player->prevColInput;
}

bool is_intact_cell(struct player *enemy, int cc_Row, int cc_Col) {
    if (enemy->board[cc_Row][cc_Col].status == SHIP_STER) return true;
    if (enemy->board[cc_Row][cc_Col].status == SHIP_BODY) return true;
    if (enemy->board[cc_Row][cc_Col].status == WATER) return true;
    return false;
}

bool is_unshooted_cell_ship(struct player *enemy, int index, int ship_cell) {
    if (enemy->ships[index].status[ship_cell][CC_STATUS] == SHIP_STER) return true;
    if (enemy->ships[index].status[ship_cell][CC_STATUS] == SHIP_BODY) return true;
    return false;
}

bool is_sunk(struct player *player, struct player *enemy, int index) {
    int i, cc_Row, cc_Col, ship_cell;
    bool undamaged_found = false;

    for (i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
        cc_Row = i / BOARD_SIZE; // Row of the cell
        cc_Col = i % BOARD_SIZE; // Column of the cell
        ship_cell = enemy->board[cc_Row][cc_Col].ship_cell;
        if (enemy->board[cc_Row][cc_Col].ship_id != index) continue; // Check if the cell belongs to the current ship

        if (is_unshooted_cell_ship(enemy, index, ship_cell)) {
            undamaged_found = true; // If there is an intact part, it is not sunk
            break; // Exit the loop if an intact part is found
        }
    }
    if (!undamaged_found) {
        if (enemy->ships[index].is_alive) { // Only count if it wasn't sunk before
            enemy->ships[index].is_alive = false; // Mark ship as sunk
            player->sunken_ships++;
        }
        return true; // Return as sunk
    }
    return false; // Return as not sunk
}

void instant_sunk(struct player *player, int index, struct player *enemy) {
    int i, cc_Row, cc_Col, ship_cell;

    for (i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
        cc_Row = i / BOARD_SIZE; // Row of the cell
        cc_Col = i % BOARD_SIZE; // Column of the cell
        ship_cell = enemy->board[cc_Row][cc_Col].ship_cell;
        
        if (!enemy->ships[index].is_alive) return;

        if (enemy->board[cc_Row][cc_Col].ship_id == index && is_unshooted_cell_ship(enemy, index, ship_cell)) {
            changeToDamaged(enemy, cc_Row, cc_Col, index, ship_cell); // Change status to damaged
            player->enemy_hit_parts++; // Increment hit parts
            player->hitsInTurn++; // Increment hits in turn
            get_remain_fleet_cells(enemy); // Update remaining fleet cells
        }
    }
    enemy->ships[index].is_alive = is_sunk(player, enemy, index); // Check if the ship has been sunk
}

void liberar_status(struct ship *barco) {
    for (int i = 0; i < barco->ship_size; i++) {
        free(barco->status[i]);
    }
    free(barco->status);
    barco->status = EMPTY; // Avoid dangling pointer
}

void free_fleet_memory(struct player *player) {
    for (int i = 0; i < NUM_SHIPS; i++) {
        liberar_status(&player->ships[i]);
    }
}