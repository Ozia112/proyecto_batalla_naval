#include "bs_logic.h"

void init_struct_player(struct player *player) {
    int i, j;

    player->player_index = -1;
    player->player_name[0] = '\0';
    player->placed_ships = 0;
    player->turn = 0;
    player->acc_turns = 0;
    for (i = 0; i < NUM_SHIPS; i++) {
        player->ships[i].ship_id = i;
        player->ships[i].ship_name[0] = '\0';
        switch(i) {
            case 0:
            player->ships[i].ship_size = 5;
            break;
            case 1:
            player->ships[i].ship_size = 4;
            break;
            case 2:
            player->ships[i].ship_size = 3;
            break;
            case 3:
            player->ships[i].ship_size = 3;
            break;
            case 4:
            player->ships[i].ship_size = 2;
            break;
            default:
            player->ships[i].ship_size = 0;
        }
        player->ships[i].ship_direction = 'U';
        player->ships[i].is_alive = true;

        // status
        player->ships[i].status = malloc(player->ships[i].ship_size * sizeof(int *));
        for (j = 0; j < player->ships[i].ship_size; j++) {
            player->ships[i].status[j] = malloc(3 * sizeof(int)); // three fields: row, column, status
            player->ships[i].status[j][CC_ROW] = -1; // Initialize row
            player->ships[i].status[j][CC_COLUMN] = -1; // Initialize column
            player->ships[i].status[j][CC_STATUS] = WATER; // Initialize status
        }
    }

    player->enemy_hit_parts = 0;
    player->sunken_ships = 0;
    player->remain_ship_cells = 0;
    
    // board
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            player->board[i][j].status = WATER;
            player->board[i][j].ship_id = -1;
            player->board[i][j].is_water = true;
            player->board[i][j].ship_cell = -1;
        }
    }

    // cards
    for (i = 0; i < CARD_QTY; i++) {
        player->cards[i].card_id = i;
        player->cards[i].peso = 0;
        player->cards[i].card_name[0] = '\0';
        player->cards[i].description[0] = '\0';
    }
    calcTotalWeight(player);
    player->acc_towers = 0;

    // board checks
    for(i = 0; i < BOARD_SIZE; i++) {
        player->row_check[i] = false;
        player->col_check[i] = false;
        player->cellCntRow[i] = 0;
        player->cellCntCol[i] = 0;
    }

    // Buffer registers
    player->prev_check_row = -1;
    player->prev_check_col = -1;
    player->salvo_mode = false;
    player->upgrade_enable = false;
    player->prevRowInput = -1;
    player->prevColInput = -1;
    player->prevHitRow = -1;
    player->prevHitCol = -1;
    player->prevCard = -1;
    player->hitsInTurn = 0;
}

void set_original_card_weights(struct player *player) {
    int i;
    for (i = 0; i < CARD_QTY; i++) {
        player->original_cards_weight[i] = player->cards[i].peso;
    }
}

void get_original_card_weights(struct player *player) {
    int i;

    for (i = 0; i < CARD_QTY; i++) {
        if (i == 9 && player->upgrade_enable) continue;
        if (i == 8 && player->salvo_mode) continue;
        player->cards[i].peso = player->original_cards_weight[i];
        deBugPrint(2, i, player->cards[i].peso, EMPTY, EMPTY_STR);
        printf("\n");
    }
    player->cards_total_weight = calcTotalWeight(player);
}

bool coords_validation(struct player *player, int index, int rowIn, int rowEnd, int columnIn, int columnEnd) {
    if (!in_range(rowEnd, columnEnd)) return false;
    if (!is_straight(player, index, rowIn, rowEnd, columnIn, columnEnd)) return false;
    if (!dim_match_index(player, index, rowIn, rowEnd, columnIn, columnEnd)) return false;
    if (!end_cell_is_water(player, index, rowIn, rowEnd, columnIn, columnEnd)) return false;
    return true;
}

// ship placement functions

void set_initial_cell(struct player *player, int index, int rowIn, int columnIn) {
    player->board[rowIn][columnIn].status = SHIP_STER;
    player->board[rowIn][columnIn].ship_id = index;
    player->board[rowIn][columnIn].is_water = false;
    player->board[rowIn][columnIn].ship_cell = 0;

    player->ships[index].status[0][CC_ROW] = rowIn;
    player->ships[index].status[0][CC_COLUMN] = columnIn;
    player->ships[index].status[0][CC_STATUS] = SHIP_STER;
}

void set_ship(struct player *player, int index, int rowIn, int rowEnd, int columnIn, int columnEnd) {
    int i;

    switch (player->ships[index].ship_direction) {
        // i = columnIn
        case 'O':
            for (i = columnIn + 1; i <= columnEnd; i++) {
                player->board[rowIn][i].status = SHIP_BODY;
                player->board[rowIn][i].ship_id = index;
                player->board[rowIn][i].is_water = false;
                player->board[rowIn][i].ship_cell = i - columnIn;

                // i - columnIn = index of the ship part
                player->ships[index].status[i - columnIn][CC_ROW] = rowIn;
                player->ships[index].status[i - columnIn][CC_COLUMN] = i;
                player->ships[index].status[i - columnIn][CC_STATUS] = SHIP_BODY;
            }
            break;
        case 'E':
            for (i = columnIn - 1; i >= columnEnd; i--) {
                player->board[rowIn][i].status = SHIP_BODY;
                player->board[rowIn][i].ship_id = index;
                player->board[rowIn][i].is_water = false;
                player->board[rowIn][i].ship_cell = columnIn - i;

                // columnIn - i = index of the ship part
                player->ships[index].status[columnIn - i][CC_ROW] = rowIn;
                player->ships[index].status[columnIn - i][CC_COLUMN] = i;
                player->ships[index].status[columnIn - i][CC_STATUS] = SHIP_BODY;
            }
            break;
        // i = rowIn
        case 'N':
            for (i = rowIn + 1; i <= rowEnd; i++) {
                player->board[i][columnIn].status = SHIP_BODY;
                player->board[i][columnIn].ship_id = index;
                player->board[i][columnIn].is_water = false;
                player->board[i][columnIn].ship_cell = i - rowIn;

                // i - rowIn = index of the ship part
                player->ships[index].status[i - rowIn][CC_ROW] = i;
                player->ships[index].status[i - rowIn][CC_COLUMN] = columnIn;
                player->ships[index].status[i - rowIn][CC_STATUS] = SHIP_BODY;
            }
            break;
        case 'S':
            for (i = rowIn - 1; i >= rowEnd; i--) {
                player->board[i][columnIn].status = SHIP_BODY;
                player->board[i][columnIn].ship_id = index;
                player->board[i][columnIn].is_water = false;
                player->board[i][columnIn].ship_cell = rowIn - i;

                // rowIn - i = index of the ship part
                player->ships[index].status[rowIn - i][CC_ROW] = i;
                player->ships[index].status[rowIn - i][CC_COLUMN] = columnIn;
                player->ships[index].status[rowIn - i][CC_STATUS] = SHIP_BODY;
            }
            break;
    }
}

// Rand functions

void firstTurnRand(struct player *player1, struct player *player2) {
    int primer_turno = (rand() % 2) + 1;
    printf("Decidiendo quien sera el primer jugador de forma aleatoria...\n");
    pause_timer(1);

    if (primer_turno == 1) {
        player1->turn = 1;
        player2->turn = 2;
        printf_color(INFO_COLOR, DEFAULT,"%s ", player1->player_name); printf("ha sido elegido para comenzar la partida\n");
        printf_color(INFO_COLOR, DEFAULT,"%s ", player2->player_name); printf("sera el segundo en jugar\n");
    } else {
        player2->turn = 1;
        player1->turn = 2;
        printf_color(INFO_COLOR, DEFAULT,"%s ", player2->player_name); printf("ha sido elegido para comenzar la partida\n");
        printf_color(INFO_COLOR, DEFAULT,"%s ", player1->player_name); printf("sera el segundo en jugar\n");
    }
    save_player_state(player1);
}

int pickRandCard(struct player *player) {
    player->cards_total_weight = calcTotalWeight(player); // Recalculate leftover card weight
    if(player->cards_total_weight <= 0) {
        return -1;
    }

    // Refine randomness: avoid bias from rand() % N
    int limit = RAND_MAX - (RAND_MAX % player->cards_total_weight);
    int numero_rand;
    do {
        numero_rand = rand();
    } while (numero_rand >= limit);
    numero_rand = numero_rand % player->cards_total_weight;
    int acumulado = 0;
    for (int i = 0; i < CARD_QTY; i++) {
        acumulado += player->cards[i].peso;
        if (numero_rand < acumulado) {
            return i;
        }
    }
    return -1; // No valid card found, should not happen
}

int calcTotalWeight(struct player *player) {
    int total_weight = 0;
    for (int i = 0; i < CARD_QTY; i++) {
        total_weight += player->cards[i].peso;
    }
    return total_weight;
}

bool parsing_coords(const char *input, int *row, int *col) {
    char row_char, col_char;
    int col_num;

    // Check for input starting with a letter (A-Z)
    if (input[0] == '\0') {
        codeError(NULL, INVALID_INPUT, -1, 0, 0);
        return false; // Empty input
    }
    if (!isalpha(input[0])) {
        codeError(NULL, INVALID_FORMAT, -1, 0, 0);
        return false; // First character must be a letter
    }
    // Case letter + number (A1, A 1, a1, a 1, etc.)
    if (sscanf(input, " %c %d", &row_char, &col_num) == 2 ||
        sscanf(input, " %c%d", &row_char, &col_num) == 2) {
        row_char = toupper((unsigned char)row_char);
        *row = row_char - 'A';
        *col = col_num - 1;
        return true;
    }
    // Case letter + letter (A J, AJ, aJ, etc.)
    else if (sscanf(input, " %c %c", &row_char, &col_char) == 2 ||
             sscanf(input, " %c%c", &row_char, &col_char) == 2) {
            row_char = toupper((unsigned char)row_char);
            col_char = toupper((unsigned char)col_char);
            if (col_char != 'J') {
                codeError(NULL, INVALID_FORMAT, -1, 0, 0);
                return false;
            }
            *row = row_char - 'A'; // Convert letter to index (A=0, B=1, ..., J=9)
            *col = col_char - 'A'; // Convert letter to index (A=0, B=1, ..., J=9)
            return true;
    }

    codeError(NULL, INVALID_FORMAT, -1, 0, 0);
    return false; // If no valid format is matched
}