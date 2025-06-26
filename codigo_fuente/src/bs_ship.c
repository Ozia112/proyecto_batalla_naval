#include "bs_ship.h" // Incluye definiciones de barcos



bool isShipHere(struct player *player, int index, int ship_cell, int fila, int columna) {
    // Verifica si la parte del barco esta en la posicion (x, y)
    return player->ships[index].status[ship_cell][0] == fila
        && player->ships[index].status[ship_cell][1] == columna;
}

bool isEnemyShipHere(struct player *player, int index, int ship_cell, struct player *enemy) {
    return enemy->ships[index].status[ship_cell][CC_ROW] == player->prevRowInput
        && enemy->ships[index].status[ship_cell][CC_COLUMN] == player->prevColInput;
}

bool cell_is_intact_ship_cell(struct player *enemy, int index, int ship_cell) {
    if (enemy->ships[index].status[ship_cell][CC_STATUS] == SHIP_STER) return true;
    if (enemy->ships[index].status[ship_cell][CC_STATUS] == SHIP_BODY) return true;
    return false;
}

void instant_sunk(struct player *player, int index, struct player *enemy) {
    int i, j;

    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            if (enemy->board[i][j].ship_id == index) {
                if(cell_is_intact_ship_cell(enemy, index, enemy->board[i][j].ship_cell)) {
                    enemy->board[i][j].status = SHIP_STER_D;
                }
                if (cell_is_intact_ship_cell(enemy, index, enemy->board[i][j].ship_cell)) {
                    enemy->board[i][j].status = SHIP_BODY_D;
                }
                player->enemy_hit_parts++; // Incrementar partes alcanzadas
                get_remain_fleet_cells(enemy); // Actualizar partes restantes
                player->hitsInTurn++; // Incrementar aciertos por turno
            }
        }
    }
    for (i = 0; i < enemy->ships[index].ship_size; i++) {
        if (cell_is_intact_ship_cell(enemy, index, i)) {
            enemy->ships[index].status[i][CC_STATUS] += 2; // Cambiar estado a dañado
        }
    }
    is_sunk(player, enemy, index); // Verifica si el barco está hundido
}

void is_sunk(struct player *player, struct player *enemy, int index) {
    int i, j;

    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            if (enemy->board[i][j].ship_id == index) {
                if (cell_is_intact_ship_cell(enemy, index, enemy->board[i][j].ship_cell)) {
                    enemy->ships[index].is_alive = true; // Marcar barco como hundido
                    return; // Salir si se encuentra una parte intacta
                }
            }
        }
    }
    enemy->ships[index].is_alive = false; // Marcar barco como hundido
    player->sunken_ships++;
    printf("El %s (%d) enemigo ha sido hundido.\n", enemy->ships[index].ship_name, enemy->ships[index].ship_id + 1);
    if (enemy->salvo_mode) {
        enemy->salvo_mode = false; // Desactivar Salvo si estaba activo
        printf("El modo Salvo enemigo se "); color_txt(INFO_COLOR); printf("desactiva.\n"); color_txt(DEFAULT_COLOR);
    }
}

void liberar_status(struct ship *barco) {
    for (int i = 0; i < barco->ship_size; i++) {
        free(barco->status[i]);
    }
    free(barco->status);
    barco->status = EMPTY; // Evitar puntero colgante
}

void liberar_flota(struct player *player) {
    for (int i = 0; i < NUM_SHIPS; i++) {
        liberar_status(&player->ships[i]);
    }
}