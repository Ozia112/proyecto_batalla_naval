#include "bs_cards.h"

void shoot_func(struct player *player, struct player *enemy) {
    int cc_Row = player->prevRowInput;
    int cc_Col = player->prevColInput;
    int ship_idx = enemy->board[cc_Row][cc_Col].ship_id;
    int ship_cell = enemy->board[cc_Row][cc_Col].ship_cell;

    if (enemy->board[cc_Row][cc_Col].status != WATER) {
        player->prevHitCol = cc_Col;
        player->prevHitRow = cc_Row;        
        if (!player->upgrade_enable) {
            changeToDamaged(enemy, cc_Row, cc_Col, ship_idx, ship_cell);
            player->enemy_hit_parts++;
            player->hitsInTurn++;
            get_remain_fleet_cells(enemy);
            printShootreport(player, enemy, ship_idx);
            enemy->ships[ship_idx].is_alive = !is_sunk(player, enemy, ship_idx);
            save_player_state(player);
            save_player_state(enemy);
            return;
        }
        instant_sunk(player, ship_idx, enemy);
        save_player_state(player);
        save_player_state(enemy);
        return;
    }
    player->prevHitCol = -1;
    player->prevHitRow = -1;
    printShootreport(player, enemy, -1);
    enemy->board[cc_Row][cc_Col].status = FAILED_SHOT;
    save_player_state(player);
    save_player_state(enemy);
}

void bomb_row(struct player *player, struct player *enemy) {
    int cc_Row = player->prevRowInput; // Row index of the shot
    int i, ship_idx, ship_cell;
    bool it_was_hit = false;
    int temp_row;

    for (i = 0; i < BOARD_SIZE; i++) {
        ship_idx = enemy->board[cc_Row][i].ship_id; // Ship ID in the cell
        ship_cell = enemy->board[cc_Row][i].ship_cell; // Ship part in the cell
        player->prevColInput = i; // Update column of shot
        if (!is_intact_cell(enemy, cc_Row, i)) continue;

        if (enemy->board[cc_Row][i].status != WATER) {
            player->prevHitRow = cc_Row;
            player->prevHitCol = i;
            temp_row = cc_Row; // Save row before changing it
            it_was_hit = true;
            if (!player->upgrade_enable) {
                changeToDamaged(enemy, cc_Row, i, ship_idx, ship_cell);
                player->enemy_hit_parts++;
                player->hitsInTurn++;
                get_remain_fleet_cells(enemy);
                printShootreport(player, enemy, ship_idx);
                enemy->ships[ship_idx].is_alive = !is_sunk(player, enemy, ship_idx);
                pause_timer(.7);
                continue;
            }
            instant_sunk(player, ship_idx, enemy);
            pause_timer(.7);
            continue;
        }
        player->prevHitRow = -1;
        player->prevHitCol = -1;
        printShootreport(player, enemy, -1);
        enemy->board[cc_Row][i].status = FAILED_SHOT;
        pause_timer(.7);
        save_player_state(player);
        save_player_state(enemy);
    }
    if (it_was_hit) player->prevHitRow = temp_row; 
}

void bomb_col(struct player *player, struct player *enemy) {
    int cc_Col = player->prevColInput; // Column index of the shot
    int i, ship_idx, ship_cell;
    bool it_was_hit = false;
    int temp_col;

    for (i = 0; i < BOARD_SIZE; i++) {
        ship_idx = enemy->board[i][cc_Col].ship_id; // Ship ID in the cell
        ship_cell = enemy->board[i][cc_Col].ship_cell; // Ship part in the cell
        player->prevRowInput = i; // Update row of shot

        if (!is_intact_cell(enemy, i, cc_Col)) continue;

        if (enemy->board[i][cc_Col].status != WATER) {
            player->prevHitRow = i;
            player->prevHitCol = cc_Col;
            temp_col = cc_Col; // Save column before changing it
            it_was_hit = true;
            if (!player->upgrade_enable) {
                changeToDamaged(enemy, i, cc_Col, ship_idx, ship_cell);
                player->enemy_hit_parts++;
                player->hitsInTurn++;
                get_remain_fleet_cells(enemy);
                printShootreport(player, enemy, ship_idx);
                enemy->ships[ship_idx].is_alive = !is_sunk(player, enemy, ship_idx);
                pause_timer(.7);
                continue;
            }
            instant_sunk(player, ship_idx, enemy);
            pause_timer(.7);
            continue;
        }
        player->prevHitRow = -1;
        player->prevHitCol = -1;
        printShootreport(player, enemy, -1);
        enemy->board[i][cc_Col].status = FAILED_SHOT;
        pause_timer(.7);
        save_player_state(player);
        save_player_state(enemy);
    }
    if (it_was_hit) player->prevHitCol = temp_col; // Restore column if shot was successful
}

void reveal(struct player *player, struct player *enemy) {
    int coords_arr[SHIP_CELLS_QTY][CC_STATUS];
    int counter = 0;
    int i, cc_Row, cc_Col, ship_idx, ship_cell, rand_idx;

    for (i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
        cc_Row = i / BOARD_SIZE; // Row index of the cell
        cc_Col = i % BOARD_SIZE; // Column index of the cell
        ship_idx = enemy->board[cc_Row][cc_Col].ship_id; // Ship ID in the cell
        ship_cell = enemy->board[cc_Row][cc_Col].ship_cell;
        if (ship_idx == -1 || ship_cell == -1) continue;
        if (is_unshooted_cell_ship(enemy, ship_idx, ship_cell)) {
            coords_arr[counter][0] = cc_Row;
            coords_arr[counter][1] = cc_Col;
            counter++;
        }
    }

    rand_idx = rand() % counter;
    cc_Row = coords_arr[rand_idx][0];
    cc_Col = coords_arr[rand_idx][1];
    ship_idx = enemy->board[cc_Row][cc_Col].ship_id; // Ship ID in the cell
    ship_cell = enemy->board[cc_Row][cc_Col].ship_cell; // Ship part in the cell

    printf("Presione enter para revelar...\n");
    printf_color(INFO_COLOR, DEFAULT,"---> ");
    printf_color(ERROR_COLOR, DEFAULT,"(%c,%d)", cc_Row + 'A', cc_Col + 1);
    printf_color(INFO_COLOR, DEFAULT," <---\n");
    pause_timer(1);
    
    printf("Presione enter para disparar...\n");
    getchar();

    if (!player->upgrade_enable) {
        player->prevHitRow = cc_Row;
        player->prevHitCol = cc_Col;
        changeToDamaged(enemy, cc_Row, cc_Col, ship_idx, ship_cell);
        player->enemy_hit_parts++;
        player->hitsInTurn++;
        get_remain_fleet_cells(enemy);
        printShootreport(player, enemy, ship_idx);
        enemy->ships[ship_idx].is_alive = !is_sunk(player, enemy, ship_idx);
        save_player_state(player);
        save_player_state(enemy);
        return;
    }
    instant_sunk(player, ship_idx, enemy);
    save_player_state(player);
    save_player_state(enemy);
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
    if (player->cards[8].peso <= 0) {
        player->salvo_mode = true;
    }
}

void advantage_tower(struct player *player) {
    player->acc_towers++; // cards[9] is the tower advantage card ID
    if (player->cards[9].peso == 0) player->upgrade_enable = true;
}