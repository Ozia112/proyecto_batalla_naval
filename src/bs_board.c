#include "bs_board.h"

// Administrate ship placement

bool in_range(int cc_Row, int cc_Col) {
    if (cc_Row >= 0 && cc_Row < BOARD_SIZE && cc_Col >= 0 && cc_Col < BOARD_SIZE) return true;
    
    codeError(NULL, OUT_OF_RANGE, -1, cc_Row, cc_Col);
    return false;
}

bool first_coord_is_free(struct player *player, int ship_idx, int cc_RowIn, int cc_ColIn) {
    if (player->board[cc_RowIn][cc_ColIn].is_water == true) return true;
    
    codeError(player, OVERLAP_DETECTED, ship_idx, cc_RowIn, cc_ColIn);
    return false;
}

void change_to_water(struct player *player, int ship_idx, int cc_RowIn, int cc_ColIn) {
    player->board[cc_RowIn][cc_ColIn].status = WATER;
    player->board[cc_RowIn][cc_ColIn].ship_id = UNSET;
    player->board[cc_RowIn][cc_ColIn].is_water = true;
    player->board[cc_RowIn][cc_ColIn].ship_cell = UNSET;

    player->ships[ship_idx].status[0][CC_ROW] = UNSET;
    player->ships[ship_idx].status[0][CC_COLUMN] = UNSET;
    player->ships[ship_idx].status[0][CC_STATUS] = NONE;
}

bool is_straight(struct player *player, int ship_idx, int cc_RowIn, int cc_RowEnd, int cc_ColIn, int cc_ColEnd) {
    if (cc_RowIn == cc_RowEnd) {
        player->ships[ship_idx].ship_direction = (cc_ColIn < cc_ColEnd) ? 'O' : 'E';
        return true;
    }

    if(cc_ColIn == cc_ColEnd) {
        player->ships[ship_idx].ship_direction = (cc_RowIn < cc_RowEnd) ? 'N' : 'S';
        return true;
    }
    
    codeError(player, INVALID_DIRECTION, ship_idx, -1, -1);
    return false;
}

bool dim_match_index(struct player *player, int ship_idx, int cc_RowIn, int cc_RowEnd, int cc_ColIn, int cc_ColEnd) {

    if (abs(cc_RowEnd - cc_RowIn) == player->ships[ship_idx].ship_size - 1) return true;
    if (abs(cc_ColEnd - cc_ColIn) == player->ships[ship_idx].ship_size - 1) return true;
    
    codeError(player, INVALID_DIMENSION, ship_idx, 0, 0);
    return false;
}

bool end_cell_is_water(struct player *player, int ship_idx, int cc_RowIn, int cc_RowEnd, int cc_ColIn, int cc_ColEnd) {
    int i;

    if (player->ships[ship_idx].ship_direction == 'E' || player->ships[ship_idx].ship_direction == 'O') {
        for (i = cc_ColIn + 1; i <= cc_ColEnd; i++) {
            if (player->board[cc_RowIn][i].is_water == false) {
                codeError(player, OVERLAP_DETECTED, ship_idx, cc_RowIn, i);
                return false;
            }
        }
    } 
    else if (player->ships[ship_idx].ship_direction == 'S' || player->ships[ship_idx].ship_direction == 'N') {
        for (i = cc_RowIn + 1; i <= cc_RowEnd; i++) {
            if (player->board[i][cc_ColIn].is_water == false) {
                codeError(player, OVERLAP_DETECTED, ship_idx, i, cc_ColIn);
                return false;
            }
        }
    }
    return true;
}

void get_remain_fleet_cells(struct player *player) {
    int i, idx_R, idx_C, counter = 0;

    for (i = 0; i < BOARD_SIZE*BOARD_SIZE; i++) {
        idx_R = i / BOARD_SIZE;
        idx_C = i % BOARD_SIZE;
        if (player->board[idx_R][idx_C].status == SHIP_STER || 
            player->board[idx_R][idx_C].status == SHIP_BODY) counter++;
    }
    player->remain_ship_cells = counter;
}

// Administrate ship damage

void changeToDamaged(struct player *enemy, int cc_Row, int cc_Col, int ship_idx, int ship_cell) {
    if (enemy->board[cc_Row][cc_Col].status == SHIP_STER) {
        enemy->board[cc_Row][cc_Col].status = SHIP_STER_D;
        enemy->ships[ship_idx].status[ship_cell][CC_STATUS] = enemy->board[cc_Row][cc_Col].status;
        return;
    }

    if (enemy->board[cc_Row][cc_Col].status == SHIP_BODY) {
        enemy->board[cc_Row][cc_Col].status = SHIP_BODY_D;
        enemy->ships[ship_idx].status[ship_cell][CC_STATUS] = enemy->board[cc_Row][cc_Col].status;
        
        return;
    }
}

bool attack_in_range(struct player *player, int cc_Row, int cc_Col) {
    
    if (player->prevCard == 3 || player->prevCard == 6) {
        if (cc_Row < 0 ||
            cc_Row >= BOARD_SIZE) codeError(player, OUT_OF_RANGE, -1, cc_Row, cc_Col);
        return (cc_Row >= 0 && cc_Row < BOARD_SIZE);
    }

    if (player->prevCard == 4 || player->prevCard == 7) {
        if (cc_Col < 0 ||
            cc_Col >= BOARD_SIZE) codeError(player, OUT_OF_RANGE, -1, cc_Row, cc_Col);
        return (cc_Col >= 0 && cc_Col < BOARD_SIZE);
    }
    return in_range(cc_Row, cc_Col);
}

int getEnemyCellsInRow(struct player *player, struct player *enemy, int cc_Row) {
    int i, contador = 0;
    
    for (i = 0; i < BOARD_SIZE; i++) {
        if (enemy->board[cc_Row][i].status == SHIP_STER ||
            enemy->board[cc_Row][i].status == SHIP_BODY) contador++;
    }
    if (contador == 0) player->row_check[cc_Row] = false;
    return contador;
}

int getEnemyCellsInCol(struct player *player, struct player *enemy, int cc_Col) {
    int i, contador = 0;

    for (i = 0; i < BOARD_SIZE; i++) {
        if (enemy->board[i][cc_Col].status == SHIP_STER ||
            enemy->board[i][cc_Col].status == SHIP_BODY) contador++;
    }
    if (contador == 0) player->col_check[cc_Col] = false;
    return contador;
}

bool cell_is_valid_to_shot(struct player *player, struct player *enemy) {
    int cc_Row = player->prevRowInput;
    int cc_Col = player->prevColInput;

    if (enemy->board[cc_Row][cc_Col].status == SHIP_STER_D ||
        enemy->board[cc_Row][cc_Col].status == SHIP_BODY_D ||
        enemy->board[cc_Row][cc_Col].status == FAILED_SHOT) {
        codeError(player, PREVIOUSLY_SHOT, -1, cc_Row, cc_Col);
        return false;
    }
    return true;
}