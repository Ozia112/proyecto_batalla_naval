#include "bs_cards.h" // libreria para poder usar funciones de efectos de cartas.

void shoot_func(struct player *player, struct player *enemy) {
    bool disparo_exitoso = false; // Variable para verificar si el disparo fue exitoso
    int row = player->prevRowInput; // Fila del disparo
    int column = player->prevColInput; // Columna del disparo
    int index = enemy->board[row][column].ship_id; // ID del barco en la celda disparada
    int ship_cell = enemy->board[row][column].ship_cell; // Parte del barco en la celda disparada

    if (enemy->board[row][column].status == WATER) {
        printf_color(ERROR_COLOR, "Disparo fallido en %c,%d! Disparaste al ", player->prevRowInput + 'A', player->prevColInput + 1);
        printf_color(WATER_COLOR, "agua.\n");
        enemy->board[row][column].status = FAILED_SHOT; // Marca la celda como disparo fallido
    } else {
        player->prevHitCol = column; // Guarda la columna del disparo exitoso
        player->prevHitRow = row; // Guarda la fila del disparo exitoso
        if (player->upgrade_enable) {
            instant_sunk(player, index, enemy); // Hundir el barco instantáneamente
            player->prevHitRow = row; // Guarda la fila del disparo exitoso
            player->prevHitCol = column; // Guarda la columna del disparo exitoso
            return; // Sale de la función si se usa la mejora
        }
        enemy->board[row][column].status += 2;
        enemy->ships[index].status[ship_cell][CC_STATUS] += 2; // Marca la parte del barco como dañada
        player->enemy_hit_parts++; // Incrementa el contador de partes de barco enemigo alcanzadas
        get_remain_fleet_cells(enemy); // Actualiza el número de partes restantes del barco enemigo
        player->hitsInTurn++; // Incrementa el contador de aciertos en el turno actual
        printPrevHitCoord(player); // Imprime las coordenadas del disparo exitoso 
        is_sunk(player, enemy, index); // Verifica si el barco está hundido
    }
}

void bomb_row(struct player *player, struct player *enemy) {
    int row = player->prevRowInput; // Fila del disparo
    int i, index, ship_cell;

    for (i = 0; i < BOARD_SIZE; i++) {
        index = enemy->board[row][i].ship_id; // ID del barco en la celda disparada
        ship_cell = enemy->board[row][i].ship_cell; // Parte del barco en la celda disparada
        player->prevColInput = i; // Actualiza la columna de disparo
        if (index >= 0 && cell_is_intact_ship_cell(enemy, index, ship_cell) && enemy->board[row][i].status != WATER) {
            enemy->board[row][i].status += 2; // Marca la celda como dañada
            enemy->ships[index].status[ship_cell][CC_STATUS] += 2; // Marca la parte del barco como dañada
            player->prevHitRow = row; // Guarda la fila del disparo exitoso
            player->prevHitCol = i; // Guarda la columna del disparo exitoso
            printPrevHitCoord(player); // Imprime las coordenadas del disparo exitoso
            if (player->upgrade_enable) {
                instant_sunk(player, index, enemy);
            } else {
                is_sunk(player, enemy, index); // Verifica si el barco está hundido
            }
            player->enemy_hit_parts++; // Incrementa el contador de partes de barco enemigo alcanzadas
            get_remain_fleet_cells(enemy); // Actualiza el número de partes restantes del barco enemigo
            player->hitsInTurn++; // Incrementa el contador de aciertos en el turno actual
        } else if (enemy->board[row][i].status == WATER) {
        enemy->board[row][i].status = FAILED_SHOT; // Marca la celda como disparo fallido
        printf_color(ERROR_COLOR, "Disparo fallido en %c,%d! Disparaste al ", player->prevRowInput + 'A', i + 1);
        printf_color(WATER_COLOR, "agua.\n");
        }
        pause_timer(.7);
    }
}

void bomb_col(struct player *player, struct player *enemy) {
    int col = player->prevColInput; // Columna del disparo
    int i, index, ship_cell;

    for (i = 0; i < BOARD_SIZE; i++) {
        index = enemy->board[i][col].ship_id; // ID del barco en la celda disparada
        ship_cell = enemy->board[i][col].ship_cell; // Parte del barco en la celda disparada
        player->prevRowInput = i; // Actualiza la fila de disparo
        if (index >= 0 && cell_is_intact_ship_cell(enemy, index, ship_cell) && enemy->board[i][col].status != WATER) {
            enemy->board[i][col].status += 2; // Marca la celda como dañada
            enemy->ships[index].status[ship_cell][CC_STATUS] += 2; // Marca la parte del barco como dañada
            player->prevHitRow = i; // Guarda la fila del disparo exitoso
            player->prevHitCol = col; // Guarda la columna del disparo exitoso
            printPrevHitCoord(player); // Imprime las coordenadas del disparo exitoso
            if (player->upgrade_enable) {
                instant_sunk(player, index, enemy);
            } else {
                is_sunk(player, enemy, index); // Verifica si el barco está hundido
            }
            player->enemy_hit_parts++; // Incrementa el contador de partes de barco enemigo alcanzadas
            get_remain_fleet_cells(enemy); // Actualiza el número de partes restantes del barco enemigo
            player->hitsInTurn++; // Incrementa el contador de aciertos en el turno actual
        } else if (enemy->board[i][col].status == WATER) {
            enemy->board[i][col].status = FAILED_SHOT; // Marca la celda como disparo fallido
            printf_color(ERROR_COLOR, "Disparo fallido en %c,%d! Disparaste al ", i + 'A', player->prevColInput + 1);
            printf_color(WATER_COLOR, "agua.\n");
        }
        pause_timer(.7);
    }
}

void revela(struct player *player, struct player *enemy) {
    // 1. Guardar todas las partes de barco no dañadas
    int posibles[SHIP_CELLS_QTY - player->enemy_hit_parts][CC_STATUS]; // Máximo 5 partes por barco
    int total = 0;
    int ship_index, cell_ship;
    
    for (ship_index = 0; ship_index < NUM_SHIPS; ship_index++) {
        for (cell_ship = 0; cell_ship < player->ships[ship_index].ship_size; cell_ship++) {
            // Si la parte no está dañada (1 = punta, 2 = cuerpo)
            if (cell_is_intact_ship_cell(enemy, ship_index, cell_ship)) {
                posibles[total][0] = player->ships[ship_index].status[cell_ship][CC_ROW]; // fila
                posibles[total][1] = player->ships[ship_index].status[cell_ship][CC_COLUMN]; // columna
                total++;
            }
        }
    }
    // 2. Elegir una coordenada aleatoria
    int idx = rand() % total;
    int fila = posibles[idx][0];
    int columna = posibles[idx][1];


    printf("Presione enter para revelar...\n");
    printf_color(INFO_COLOR,"---> ");
    printf_color(ERROR_COLOR,"(%c,%d)", fila + 'A', columna + 1);
    printf_color(INFO_COLOR," <---\n");
    pause_timer(1);
    
    printf("Presione enter para disparar...\n");
    getchar();
    
    enemy->board[fila][columna].status += 2; // Marca la celda como dañada
    player->prevHitRow = fila; // Guarda la fila del disparo exitoso
    player->prevHitCol = columna; // Guarda la columna del disparo exitoso
    ship_index = enemy->board[fila][columna].ship_id; // ID del barco en la celda disparada
    cell_ship = enemy->board[fila][columna].ship_cell; // Parte del barco en la celda disparada

    enemy->ships[ship_index].status[cell_ship][CC_STATUS] += 2; // Marca la parte del barco como dañada
    player->hitsInTurn++; // Incrementa el contador de aciertos en el turno actual
    player->enemy_hit_parts++; // Incrementa el contador de partes de barco enemigo alcanzadas
    get_remain_fleet_cells(enemy); // Actualiza el número de partes restantes del barco enemigo
    
    if (player->upgrade_enable) {
        instant_sunk(player, ship_index, enemy); // Hundir el barco instantáneamente
        return; // Sale de la función si se usa la mejora
    } else {
        is_sunk(player, enemy, ship_index); // Verifica si el barco está hundido
    }

    clear_screen();
    currentPlayerWarScreen(player, enemy);
    printPrevHitCoord(player); // Imprime las coordenadas del disparo exitoso
}

void activateCheckInRow(struct player *player, struct player *enemy) {
    player->row_check[player->prevRowInput] = true;
    player->cellCntRow[player->prevRowInput] = getEnemyCellsInRow(player, enemy, player->prevRowInput);
}

void activateCheckInCol(struct player *player, struct player *enemy) {
    player->col_check[player->prevColInput] = true;
    player->cellCntCol[player->prevColInput] = getEnemyCellsInCol(player, enemy, player->prevColInput);
}

void activar_salvo(struct player *player) {
    player->salvo_mode = true;
}

void desactivar_salvo(struct player *player) {
    player->salvo_mode = false;
}

void torre_ventaja(struct player *player) {
    player->upgrade_enable = true;
    player->cards[9].peso = 0; // Reducir el peso de la carta Torre de ventaja a 0 para que no pueda ser seleccionada
}