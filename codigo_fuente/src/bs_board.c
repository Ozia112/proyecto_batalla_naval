#include "bs_board.h"

// Administrar colocacion y movimiento de barcos

bool in_range(int cc_row, int cc_column) {
    if (cc_row >= 0 && cc_row < BOARD_SIZE && 
        cc_column >= 0 && cc_column < BOARD_SIZE) {
        return true; // Coordenadas válidas
    }
    codeError(EMPTY, OUT_OF_RANGE, -1, cc_row, cc_column);
    return false;
}

bool first_coord_is_free(struct player *player, int index, int rowIn, int columnIn) {
    int index_ship, ship_cell;

    if (player->board[rowIn][columnIn].is_water == true) {
        return true;
    }
    codeError(player, OVERLAP_DETECTED, index, rowIn, columnIn);
    return false; // Marca si la celda esta ocupada
}

bool is_straight(struct player *player, int index, int rowIn, int rowEnd, int columnIn, int columnEnd) {
    if (rowIn == rowEnd) {
        player->ships[index].ship_direction = (columnIn < columnEnd) ? 'E' : 'O';
        return true; // Orientación válida
    } 
    if(columnIn == columnEnd) {
        player->ships[index].ship_direction = (rowIn < rowEnd) ? 'S' : 'N';
        return true; // Orientación válida
    }
    player->ships[index].ship_direction = 'U'; // No debería ocurrir si las validaciones son correctas
    codeError(EMPTY, INVALID_DIRECTION, index, -1, -1);
    return false;
}

bool dim_match_index( struct player *player, int index, int rowIn, int rowEnd, int columnIn, int columnEnd) {

    if (abs(rowEnd - rowIn) == player->ships[index].ship_size - 1) return true; // Dimensión válida
    if (abs(columnEnd - columnIn) == player->ships[index].ship_size - 1) return true; // Dimensión válida    
    
    codeError(player, INVALID_DIMENSION, index, 0, 0);
    return false;
}

bool end_cell_is_water(struct player *player, int index, int rowIn, int rowEnd, int columnIn, int columnEnd) {
    int i;

    if (player->ships[index].ship_direction == 'E' || player->ships[index].ship_direction == 'O') {
        for (int i = columnIn + 1; i <= columnEnd; i++) {
            if (player->board[rowIn][i].is_water == false) {
                codeError(player, OVERLAP_DETECTED, index, rowIn, i);
                return false; // La celda final no es agua
            }
        }
    } 
    else if (player->ships[index].ship_direction == 'S' || player->ships[index].ship_direction == 'N') {
        for (int i = rowIn + 1; i <= rowEnd; i++) {
            if (player->board[i][columnIn].is_water == false) {
                codeError(player, OVERLAP_DETECTED, index, i, columnIn);
                return false; // La celda final no es agua
            }
        }
    }
    return true; // Todas las celdas finales son agua
}

void get_remain_fleet_cells(struct player *player) {
    int i, j;
    // Calcular el número de barcos restantes del jugador
    player->remain_ship_cells = 0;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            if (player->board[i][j].status == SHIP_STER || player->board[i][j].status == SHIP_BODY) {
                player->remain_ship_cells++; // Incrementar el contador de celdas de barco restantes
            }
        }
    }
}

// Administrar ataques a barcos

bool attack_in_range(struct player *player, int cc_row, int cc_column) {
    
    // Cartas que solo requieren validar fila
    if (player->prevCard == 3 || player->prevCard == 6) {
        if (cc_row < 0 || cc_row >= BOARD_SIZE) {
            codeError(player, OUT_OF_RANGE, -1, cc_row, cc_column);
        }
    return (cc_row >= 0 && cc_row < BOARD_SIZE); // Solo fila es válida
    }
    if (player->prevCard == 4 || player->prevCard == 7) {
        if (cc_column < 0 || cc_column >= BOARD_SIZE) {
            codeError(player, OUT_OF_RANGE, -1, cc_row, cc_column);
        }
        return (cc_column >= 0 && cc_column < BOARD_SIZE); // Solo columna es válida
    }
    // Cartas que requieren validar fila y columna
    return in_range(cc_row, cc_column);
}

int getEnemyCellsInRow(struct player *player, struct player *enemy, int cc_row) {
    int i, contador = 0;
    
    for (i = 0; i < BOARD_SIZE; i++) {
        if (enemy->board[cc_row][i].status == SHIP_STER || 
            enemy->board[cc_row][i].status == SHIP_BODY) {
            contador++;
        }
    }
    return contador;
}

int getEnemyCellsInCol(struct player *player, struct player *enemy, int cc_columna) {
    int i, contador = 0;

    for (i = 0; i < BOARD_SIZE; i++) {
        if (enemy->board[i][cc_columna].status == SHIP_STER || 
            enemy->board[i][cc_columna].status == SHIP_BODY) {
            contador++;
        }
    }
    return contador;
}

bool cell_is_valid_to_shot(struct player *player, struct player *enemy) {
    int row = player->prevRowInput;
    int column = player->prevColInput;

    // ¿Hay barco en la coordenada?
    if (enemy->board[row][column].status == SHIP_STER_D || 
        enemy->board[row][column].status == SHIP_BODY_D || 
        enemy->board[row][column].status == FAILED_SHOT) {
        codeError(EMPTY, PREVIOUSLY_SHOT, -1, row, column);
        return false; // Ya disparado
    }
    return true; // No habia barco → puede disparar (agua) o hay barco intacto
}