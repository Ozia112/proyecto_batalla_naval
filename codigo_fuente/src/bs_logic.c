#include "bs_logic.h"

void init_struct_player(struct player *player) {
    int i, j;

    player->player_index = -1; // Inicializar el índice del jugador
    player->player_name[0] = '\0'; // Inicializar el nombre del jugador como cadena vacía
    player->placed_ships = 0; // Inicializar el contador de barcos colocados
    player->turn = 0; // Inicializar el turno del jugador
    player->acc_turns = 0; // Inicializar los turnos acumulados del jugador
    for (i = 0; i < NUM_SHIPS; i++) {
        player->ships[i].ship_id = i;
        player->ships[i].ship_name[0] = '\0'; // Inicializar nombre del barco
        switch(i) {
            case 0:
            player->ships[i].ship_size = 5; // Portaaviones
            break;
            case 1:
            player->ships[i].ship_size = 4; // Acorazado
            break;
            case 2:
            player->ships[i].ship_size = 3; // Crucero
            break;
            case 3:
            player->ships[i].ship_size = 3; // Destructor
            break;
            case 4:
            player->ships[i].ship_size = 2; // Fragata
            break;
            default:
            player->ships[i].ship_size = 0; // Barco no definido
        }
        player->ships[i].ship_direction = 'U'; // Por defecto, dirección Este
        player->ships[i].is_alive = true;

        player->ships[i].status = malloc(player->ships[i].ship_size * sizeof(int *));
        for (j = 0; j < player->ships[i].ship_size; j++) {
            player->ships[i].status[j] = malloc(3 * sizeof(int)); // 3 campos: fila, columna, estado
            player->ships[i].status[j][CC_ROW] = -1; // Inicializar fila
            player->ships[i].status[j][CC_COLUMN] = -1; // Inicializar columna
            player->ships[i].status[j][CC_STATUS] = WATER; // Inicializar estado
        }   
    }
    
    player->enemy_hit_parts = 0; // Inicializar partes de barco enemigo alcanzadas
    player->sunken_ships = 0; // Inicializar barcos enemigos hundidos
    player->remain_ship_cells = 0; // Inicializar celdas de barco restantes
    // Inicializar el tablero
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            player->board[i][j].status = WATER;
            player->board[i][j].ship_id = -1;
            player->board[i][j].is_water = true;
            player->board[i][j].ship_cell = -1;
        }
    }
    for (i = 0; i < CARD_QTY; i++) {
        player->cards[i].card_id = i; // Asignar ID a cada carta
        player->cards[i].peso = 0; // Inicializar peso de la carta
        player->cards[i].card_name[0] = '\0'; // Inicializar nombre de la carta
        player->cards[i].description[0] = '\0'; // Inicializar descripción de la carta
    }
    calcTotalWeight(player); // Calcular partes restantes de barcos.
    player->acc_towers = 0; // Inicializar torres acumuladas
    for(i = 0; i < BOARD_SIZE; i++) {
        player->row_check[i] = false; // Inicializar chequeo de filas
        player->col_check[i] = false; // Inicializar chequeo de columnas
        player->cellCntRow[i] = 0; // Contador de celdas en fila
        player->cellCntCol[i] = 0; // Contador de celdas en columna
    }
    player->prev_check_row = -1; // Inicializar fila chequeada previa
    player->prev_check_col = -1; // Inicializar columna chequeada previa
    player->salvo_mode = false; // Inicializar modo Salvo
    player->upgrade_enable = false; // Inicializar modo de mejora
    player->prevRowInput = -1; // Inicializar coordenada de disparo previa
    player->prevColInput = -1; // Inicializar coordenada de disparo previa
    player->prevHitRow = -1; // Inicializar coordenada de acierto previa
    player->prevHitCol = -1; // Inicializar coordenada de acierto previa
    player->prevCard = -1; // Inicializar ID de carta previa
    player->hitsInTurn = 0; // Inicializar número de aciertos en turno
}

bool coords_validation(struct player *player, int index, int rowIn, int rowEnd, int columnIn, int columnEnd) {
    if (!in_range(rowEnd, columnEnd)) return false;
    if (!is_straight(player, index, rowIn, rowEnd, columnIn, columnEnd)) return false;
    if (!dim_match_index(player, index, rowIn, rowEnd, columnIn, columnEnd)) return false;
    if (!end_cell_is_water(player, index, rowIn, rowEnd, columnIn, columnEnd)) return false;
    return true;
}

void set_initial_cell(struct player *player, int index, int rowIn, int columnIn) {
    player->board[rowIn][columnIn].status = SHIP_STER; // Marca la proa del barco
    player->board[rowIn][columnIn].ship_id = index; // Asigna el ID del barco a la celda
    player->board[rowIn][columnIn].is_water = false; // Marca la celda como no agua
    player->board[rowIn][columnIn].ship_cell = 0; // Marca la celda como parte del barco

    player->ships[index].status[0][CC_ROW] = rowIn;
    player->ships[index].status[0][CC_COLUMN] = columnIn;
    player->ships[index].status[0][CC_STATUS] = SHIP_STER; // Marca la proa del barco
}

void set_ship(struct player *player, int index, int rowIn, int rowEnd, int columnIn, int columnEnd) {
    int i;

    // Solo almacena la información en ship_i->status
    switch (player->ships[index].ship_direction) {
        case 'E':
            for (i = columnIn + 1; i <= columnEnd; i++) {
                player->board[rowIn][i].status = SHIP_BODY; // Marca el cuerpo del barco
                player->board[rowIn][i].ship_id = index; // Asigna el ID del barco a la celda
                player->board[rowIn][i].is_water = false; // Marca la celda como no agua
                player->board[rowIn][i].ship_cell = i - columnIn; // Marca la celda como parte del barco

                player->ships[index].status[i - columnIn][CC_ROW] = rowIn; // Almacena la fila
                player->ships[index].status[i - columnIn][CC_COLUMN] = i; // Almacena la columna
                player->ships[index].status[i - columnIn][CC_STATUS] = SHIP_BODY; // Marca el cuerpo del barco
            }
            break;
        case 'O':
            for (i = columnIn - 1; i >= columnEnd; i--) {
                player->board[rowIn][i].status = SHIP_BODY; // Marca el cuerpo del barco
                player->board[rowIn][i].ship_id = index; // Asigna el ID del barco a la celda
                player->board[rowIn][i].is_water = false; // Marca la celda como no agua
                player->board[rowIn][i].ship_cell = columnIn - i; // Marca la celda como parte del barco

                player->ships[index].status[columnIn - i][CC_ROW] = rowIn; // Almacena la fila
                player->ships[index].status[columnIn - i][CC_COLUMN] = i; // Almacena la columna
                player->ships[index].status[columnIn - i][CC_STATUS] = SHIP_BODY; // Marca el cuerpo del barco
            }
            break;
        case 'S':
            for (i = rowIn + 1; i <= rowEnd; i++) {
                player->board[i][columnIn].status = SHIP_BODY; // Marca el cuerpo del barco
                player->board[i][columnIn].ship_id = index; // Asigna el ID del barco a la celda
                player->board[i][columnIn].is_water = false; // Marca la celda como no agua
                player->board[i][columnIn].ship_cell = i - rowIn; // Marca la celda como parte del barco

                player->ships[index].status[i - rowIn][CC_ROW] = i; // Almacena la fila
                player->ships[index].status[i - rowIn][CC_COLUMN] = columnIn; // Almacena la columna
                player->ships[index].status[i - rowIn][CC_STATUS] = SHIP_BODY; // Marca el cuerpo del barco
            }
            break;
        case 'N':
            for (i = rowIn - 1; i >= rowEnd; i--) {
                player->board[i][columnIn].status = SHIP_BODY; // Marca el cuerpo del barco
                player->board[i][columnIn].ship_id = index; // Asigna el ID del barco a la celda
                player->board[i][columnIn].is_water = false; // Marca la celda como no agua
                player->board[i][columnIn].ship_cell = rowIn - i; // Marca la celda como parte del barco

                player->ships[index].status[rowIn - i][CC_ROW] = i; // Almacena la fila
                player->ships[index].status[rowIn - i][CC_COLUMN] = columnIn; // Almacena la columna
                player->ships[index].status[rowIn - i][CC_STATUS] = SHIP_BODY; // Marca el cuerpo del barco
            }
            break;
    }
}

void firstTurnRand(struct player *player1, struct player *player2) {
    int primer_turno = (rand() % 2) + 1;
    printf("Decidiendo quien sera el primer jugador de forma aleatoria...\n");
    pause_timer(1);

    if (primer_turno == 1) {
        player1->turn = 1;
        player2->turn = 2;
        color_txt(INFO_COLOR); printf("%s ", player1->player_name); color_txt(DEFAULT_COLOR); printf("ha sido elegido para comenzar la partida\n");
        color_txt(INFO_COLOR); printf("%s ", player2->player_name); color_txt(DEFAULT_COLOR); printf("sera el segundo en jugar\n");
    } else {
        player2->turn = 1;
        player1->turn = 2;
        color_txt(INFO_COLOR); printf("%s ", player2->player_name); color_txt(DEFAULT_COLOR); printf("ha sido elegido para comenzar la partida\n");
        color_txt(INFO_COLOR); printf("%s ", player1->player_name); color_txt(DEFAULT_COLOR); printf("sera el segundo en jugar\n");
    }
    save_player_state(player1); // Guardar el estado de los jugadores en un archivo JSON.
}

int pickRandCard(struct player *player) {
    // Recalcular probabilidades antes de sacar una carta
    player->cards_total_weight = calcTotalWeight(player);
    if(player->cards_total_weight <= 0) {
        return -1; // No hay cartas disponibles
    }
    
    // Numero aleatorio entre 0 y peso_total
    int numero_rand = rand() % player->cards_total_weight;
    
    int acumulado = 0;
    for (int i = 0; i < CARD_QTY; i++) {
        acumulado += player->cards[i].peso;
        if (numero_rand < acumulado) {
            return i;
        }
    }
    return -1; // No se encontr� una carta v�lida
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

    // Caso letra + numero (A1, A 1, a1, a 1, etc.)
    if (sscanf(input, " %c %d", &row_char, &col_num) == 2 ||
        sscanf(input, " %c%d", &row_char, &col_num) == 2) {
        row_char = toupper((unsigned char)row_char); // Convertir row a mayúscula
        *row = row_char - 'A'; // Convertir letra a índice (A=0, B=1, ..., J=9)
        *col = col_num - 1; // Convertir número a índice (1=0, 2=1, ..., 10=9)
        return true;
    }
    else if (sscanf(input, " %c %c", &row_char, &col_char) == 2 ||
              sscanf(input, " %c%c", &row_char, &col_char) == 2) {
                row_char = toupper((unsigned char)row_char); // Convertir row a mayúscula
                col_char = toupper((unsigned char)col_char); // Convertir col a mayúscula
                if (col_char != 'J') {
                    codeError(EMPTY, INVALID_FORMAT, -1, 0, 0);
                    return false;
                }
                *row = row_char - 'A'; // Convertir letra a índice (A=0, B=1, ..., J=9)
                *col = 9; // Columna J es siempre 9 (índice 9)
                return true;
    }
    codeError(EMPTY, INVALID_FORMAT, -1, 0, 0);
    return false; // Formato inválido
}