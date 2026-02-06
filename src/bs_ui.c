#include "bs_ui.h"

void tittle_screen() {
	printf(" _______  _______  _______  _______  ___      ___      _______    __    _  _______  __   __  _______  ___     \n");
	printf("|  _    ||   _   ||       ||   _   ||   |    |   |    |   _   |  |  |  | ||   _   ||  | |  ||   _   ||   |    \n");
	printf("| |_|   ||  |_|  ||_     _||  |_|  ||   |    |   |    |  |_|  |  |   |_| ||  |_|  ||  |_|  ||  |_|  ||   |    \n");
	printf("|       ||       |  |   |  |       ||   |    |   |    |       |  |       ||       ||       ||       ||   |    \n");
	printf("|  _   | |       |  |   |  |       ||   |___ |   |___ |       |  |  _    ||       ||       ||       ||   |___ \n");
	printf("| |_|   ||   _   |  |   |  |   _   ||       ||       ||   _   |  | | |   ||   _   | |     | |   _   ||       | \n");
	printf("|_______||__| |__|  |___|  |__| |__||_______||_______||__| |__|  |_|  |__||__| |__|  |___|  |__| |__||_______| \n");
}

void main_menu(struct player *player1, struct player *player2) {
    char opc;

	do{
        tittle_screen();

        printf_color(INFO_COLOR, DEFAULT, "\n\t\t=== MENU PRINCIPAL ===\n\n");

		printf("Ingrese la opcion que desee realizar.\n");
		printf("Opciones:\n");
        printf("[A]: Empezar\n");
        printf("[B]: Ver reglas.\n");
        printf("[C]: Salir\n");
		scanf(" %c", &opc);
        clearInputBuffer();
        opc = toupper(opc);
        if (opc == 'A' || opc == 'B' || opc == 'C') {
            clear_screen();
        }
		switch(opc) {
            case 'A':
                flow_game(player1, player2);
                break;
            case 'B':
                rules();
                break;
            case 'C':
                printf("Gracias por haber usado el programa :)\n");
                return;
            default:
                puts("¡Tecla invalida!");
                pause_timer(1);
                clear_screen();
                break;
            }
	}while(true);
}

void rules() {
    char opc;
    do {
        printf_color(INFO_COLOR, DEFAULT,"=== REGLAS DEL JUEGO \"BATALLA NAVAL\" ===\n\n");
        printf_color(SUCCESS_COLOR, DEFAULT,"1. Cada jugador contara con una flota de barcos,\n");
        printf_color(DEFAULT, DEFAULT,"   la cual ordenara en una distribucion que considere conveniente para ocultarla de su rival.\n\n");
        printf_color(SUCCESS_COLOR, DEFAULT,"2. El juego es por turnos.\n");
        printf_color(DEFAULT, DEFAULT,"   En cada turno se le proporcionara a cada jugador la posibilidad de usar una ");
        printf_color(INFO_COLOR, DEFAULT,"carta");
        printf_color(DEFAULT, DEFAULT," con diversos efectos sobre el tablero.\n\n");
        printf_color(SUCCESS_COLOR, DEFAULT,"3. Las cartas disponibles son:\n");
        printf_color(DEFAULT, DEFAULT,"   ");
        printf_color(INFO_COLOR, DEFAULT,"- Disparo: ");
        printf_color(DEFAULT, DEFAULT,"Dispara a una coordenada del tablero enemigo.\n   ");
        printf_color(INFO_COLOR, DEFAULT,"- Bombardeo de fila: ");
        printf_color(DEFAULT, DEFAULT,"Bombardea una fila del tablero enemigo.\n   ");
        printf_color(INFO_COLOR, DEFAULT,"- Bombardeo de columna: ");
        printf_color(DEFAULT, DEFAULT,"Bombardea una columna del tablero enemigo.\n   ");
        printf_color(INFO_COLOR, DEFAULT,"- Revelar: ");
        printf_color(DEFAULT, DEFAULT,"Revela el estado de los barcos enemigos.\n   ");
        printf_color(INFO_COLOR, DEFAULT,"- Chequeo de fila: ");
        printf_color(DEFAULT, DEFAULT,"Revela el estado de una fila del tablero enemigo.\n   ");
        printf_color(INFO_COLOR, DEFAULT,"- Chequeo de columna: ");
        printf_color(DEFAULT, DEFAULT,"Revela el estado de una columna del tablero enemigo.\n   ");
        printf_color(INFO_COLOR, DEFAULT,"- Salvo: ");
        printf_color(DEFAULT, DEFAULT,"Permite disparar a una coordenada del tablero enemigo sin que el enemigo pueda defenderse.\n   ");
        printf_color(INFO_COLOR, DEFAULT,"- Torre de ventaja: ");
        printf_color(DEFAULT, DEFAULT,"Permite disparar a una coordenada y revela el estado de la fila y columna del barco.\n   ");
        printf_color(INFO_COLOR, DEFAULT,"- Revelar casilla aleatoria: ");
        printf_color(DEFAULT, DEFAULT,"Permite revelar una casilla aleatoria en la que haya un barco enemigo.\n\n");
        printf_color(SUCCESS_COLOR, DEFAULT,"4. El objetivo del juego es ");
        printf_color(INFO_COLOR, DEFAULT,"hundir todos los barcos del enemigo");
        printf_color(DEFAULT, DEFAULT," antes de que el enemigo hunda los tuyos.\n\n");
        printf_color(SUCCESS_COLOR, DEFAULT,"5. Un barco se considera hundido cuando ");
        printf_color(INFO_COLOR, DEFAULT,"todas sus partes han sido alcanzadas.");
        printf("\n\n");
        printf_color(ERROR_COLOR, DEFAULT," PROHIBIDO\n");
        printf("   ");
        printf_color(ERROR_COLOR, DEFAULT,"- No puedes colocar barcos fuera del tablero.\n");
        printf("   ");
        printf_color(ERROR_COLOR, DEFAULT,"- No puedes sobreponer barcos.\n");
        printf("   ");
        printf_color(ERROR_COLOR, DEFAULT,"- No puedes disparar dos veces a la misma casilla.\n");
        printf("   ");
        printf_color(ERROR_COLOR, DEFAULT,"- No puedes mover barcos una vez colocados.\n\n");
        printf_color(INFO_COLOR, DEFAULT,"Consejos:\n");
        printf("   - Piensa estrategicamente la ubicacion de tus barcos.\n");
        printf("   - Aprovecha las cartas especiales para obtener ventaja.\n");
        printf("   - Recuerda que la informacion revelada por cartas es temporal.\n\n");

        printf("[Z]: Regresar al"); printf_color(INFO_COLOR, DEFAULT," menu.\n");

        scanf(" %c", &opc);
        opc = toupper(opc);
        if (opc != 'Z' && opc != 'z') {
            printf_color(ERROR_COLOR, DEFAULT,"Entrada invalida. Intente de nuevo.\n");
            pause_timer(.5);
        }
        clearInputBuffer();
        clear_screen();
        
    } while (opc != 'Z');
}

void get_name(struct player *player, int player_index) {
    char name[MAX_NAME_LENGTH] = {0};
    while (1) {
        clear_screen();
        printf("Jugador %s:\n", player->player_index == 1 ? "1" : "2");
        printf("Introduce el nombre del jugador (max 20 caracteres): ");
        if (!fgets(name, sizeof(name), stdin)) {
            codeError(player, INVALID_INPUT, -1, 0, 0);
            printf("Entrada vacia invalida1.\n");
            enter_to_message("continuar", true);
            continue;
        }

        size_t len = strlen(name);
        if (len > 0 && name[len - 1] != '\n') { // Check for newline
            clearInputBuffer();
        }

        char *nl = strchr(name, '\n');
        if (nl) *nl = '\0'; // Replace newline with null terminator

        if (name[0] == '\0') {
            codeError(player, INVALID_INPUT, -1, 0, 0);
            printf("Entrada vacia invalida2.\n");
            enter_to_message("continuar", true); // Enter to continue
            continue;
        }

        // Check length of the name
        len = strlen(name);
        if (len == 0) { // If the name is empty after removing newline 
            codeError(player, INVALID_INPUT, -1, 0, 0);
            printf("Debe ingresar al menos un caracter.\n");
            enter_to_message("continuar", true); // Enter to continue
            continue;
        }

        if (len > 20) { // If the name is longer than 20 characters
            codeError(player, INVALID_FORMAT, -1, 0, 0);
            printf("El nombre no puede tener mas de 20 caracteres.\n");
            enter_to_message("continuar", true); // Enter to continue
            continue;
        }
        
        break; // If we reach here, the name is valid
    }
    strncpy(player->player_name, name, MAX_NAME_LENGTH); // Copy the name to the player structure
    player->player_name[MAX_NAME_LENGTH - 1] = '\0'; // Ensure the string is null-terminated
    clear_screen(); // Clear the screen after getting the name.
    save_player_state(player); // Save the player's state after getting the name.
}

void set_ships_screen(struct player *player, int index, struct player *enemy) {
    printf("Colocando barcos de"); 
    printf_color(INFO_COLOR, DEFAULT, " %s\n", player->player_name);
    printPlayerBoard(player, enemy);
    printf("Coloca tu %s (", player->ships[index].ship_name); printf_color(INFO_COLOR, DEFAULT, " %d ", player->ships[index].ship_size);
    printf("celdas).\nBarco"); printf_color(INFO_COLOR, DEFAULT, " %d ", index + 1);
    printf("de"); printf_color(INFO_COLOR, DEFAULT, " %d\n", NUM_SHIPS);
}

void set_ships_flow(struct player *player, struct player *enemy) {
    for(int index_ship = 0; index_ship < NUM_SHIPS; index_ship++) {
        getSetShipsCoords(player, index_ship, enemy);
    }
    printf("Colocando barcos de"); printf_color(INFO_COLOR, DEFAULT," %s\n", player->player_name);
    printPlayerBoard(player, enemy);
    get_remain_fleet_cells(player);
    printf("Barcos colocados correctamente. \n");
    pause_timer(0.2);
    enter_to_message("continuar", true);
    save_player_state(player);
}

void printPlayerBoard(struct player *player, struct player *enemy) {
    int boardWidth, filler, windowWidth;
    int row, col, cellState;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    
    // Get the console window size
    windowWidth = 80; // Default width
    if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        windowWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }

    // Filler calculation for centering the board
    boardWidth = BOARD_SIZE * 3 + 3; // Each column occupies 3 characters (" 1 ")
    filler = (windowWidth - boardWidth) / 2;

    printf("%*s", filler, ""); // Print filler spaces to center
    printf_color(INFO_COLOR, DEFAULT,"Tablero de Flota\n");

    // Column header (1 2 3 ... 10)
    printf("%*s", filler,""); // Print filler spaces to center
    printf("  ");
    for (int num = 0; num < BOARD_SIZE; num++) {
        num + 1 < BOARD_SIZE ? printf("%2d", num + 1) : printf(" J");
    }
    printf("\n");

    // Draw each row
    set_CP_config(850);
    for (row = 0; row < BOARD_SIZE; row++) {
        // Row header (A B C ... J)
        printf("%*s%c  ", filler,"", 'A' + row);

        // Iterate over each column
        for (col = 0; col < BOARD_SIZE; col++) {
            cellState = player->board[row][col].status; // Obtaining the cell state
            
            // Print depending on the cell state
            switch (cellState) {
                case SHIP_STER:
                    printf_color(SHIP_COLOR, DEFAULT,"%c ", STER_PRINT);
                    break;
                case SHIP_BODY:
                    printf_color(SHIP_COLOR, DEFAULT,"%c ", BODY_PRINT);
                    break;
                case SHIP_STER_D:
                    printf_color(ERROR_COLOR, DEFAULT,"%c ", STER_PRINT);
                    break;
                case SHIP_BODY_D:
                    printf_color(ERROR_COLOR, DEFAULT,"%c ", BODY_PRINT);
                    break;
                case FAILED_SHOT:
                    printf_color(INFO_COLOR, DEFAULT,"%c ", FAILED_SHOT_PRINT);
                    break;
                default:
                    printf_color(WATER_COLOR, DEFAULT,"%c ", WATER_PRINT);
                    break;
            }

        }
        printf("\n");
    }
    set_CP_config(65001);
}

void getSetShipsCoords(struct player *player, int index, struct player *enemy) {
    char input[32], *nl, opc; // Buffer for input and newline pointer
    int rowIn, colIn;
    int rowEnd, colEnd;

    REINICIAL:
    //-------------------------- Initial coords -----------------------------
    set_ships_screen(player, index, enemy);
    printf("Coordenada inicial del barco (Fila: letras de A a la J; Columna: numero del 1 al 10): \n");
    printf("Ejemplo:"); printf_color(INFO_COLOR, DEFAULT, "A 1\n");
    while (true) {
        READ1:
        printf("> ");
        fgets(input, sizeof(input), stdin);

        nl = strchr(input, '\n'); // Search for newline character
        if (nl) *nl = '\0'; // Remove newline character from end of input

        // Validate format of coordinates
        if (!parsing_coords(input, &rowIn, &colIn)) continue;

        // Now rowIn ∈ [0..9], colIn ∈ [0..9] and accepts "JJ", "J10", etc.
        if (!in_range(rowIn, colIn)) continue; // Validate coordinate range

        if(!first_coord_is_free(player, index, rowIn, colIn)) continue; // If overlap, reestart the loop
        
        // If initial coordinates are valid, print confirmation menu
        while (true) {
            char confirm_input[8];
            printf("Presiona"); printf_color(INFO_COLOR, DEFAULT, " ENTER"); printf(" para confirmar.\n");
            printf("[Z]: Intentar de nuevo.\n");
            if (!fgets(confirm_input, sizeof(confirm_input), stdin)) {
                clearInputBuffer();
                printf_color(ERROR_COLOR, DEFAULT, "Opcion invalida. Intente de nuevo\n");
                continue;
            }
            // Case where only ENTER is pressed
            if (confirm_input[0] == '\n') break;
            // Case where Z or z is pressed
            if (toupper((unsigned char)confirm_input[0]) == 'Z') {
                clear_screen();
                goto REINICIAL;
            }
            printf_color(ERROR_COLOR, DEFAULT, "Opcion invalida. Intente de nuevo\n");
        }
        clear_screen();
        break;
    }
    set_initial_cell(player, index, rowIn, colIn); // Place the initial cell of the ship
    save_player_state(player);

REFINAL:
    // --------------------------- Final coords ------------------------------
    set_ships_screen(player, index, enemy);
    printf("Coordenada final del barco (Fila: letras de A a la J; Columna: numero del 1 al 10): \n");
    printf("[Z]: Reingresar coordenada inicial.\n");
    printf("Ejemplo:"); printf_color(INFO_COLOR, DEFAULT, "A 1\n");
    while (true) {
        printf("> ");
        fgets(input, sizeof(input), stdin);

        nl = strchr(input, '\n'); // Search for newline character
        if (nl) *nl = '\0'; // Remove newline character from end of input
        if (input[0] == 'Z' || input[0] == 'z') {
            if (input[1] == '\0') {
                clear_screen();
                change_to_water(player, index, rowIn, colIn);
                goto REINICIAL; // If Z or z is pressed, return to initial coordinates
            }
        }

        // Case letter + letter (e.g. "JJ", "J J")
        if(!parsing_coords(input, &rowEnd, &colEnd)) continue;

        // Validations
        if (!coords_validation(player, index, rowIn, rowEnd, colIn, colEnd)) continue;

        while (true) {  // Logic for confirming coordinates
            char confirm_input[8];
            printf("Presiona "); printf_color(INFO_COLOR, DEFAULT, "ENTER"); 
            printf(" para confirmar.\n");
            printf("[Z]: Intentar de nuevo.\n");

            if (!fgets(confirm_input, sizeof(confirm_input), stdin)) {
                clearInputBuffer();
                printf_color(ERROR_COLOR, DEFAULT, "Opcion invalida. Intente de nuevo\n");
                continue;
            }
            // Case where only ENTER is pressed
            if (confirm_input[0] == '\n') goto SETSHIP;
            // Case where Z or z is pressed
            if (toupper((unsigned char)confirm_input[0]) == 'Z') {
                clear_screen();
                goto REFINAL;
            }
            printf_color(ERROR_COLOR, DEFAULT, "Opcion invalida. Intente de nuevo\n");
        }        
        break;
    }

SETSHIP:
    set_ship(player, index, rowIn, rowEnd, colIn, colEnd);
    player->placed_ships++;
    save_player_state(player);
    
    printf_color(SUCCESS_COLOR, DEFAULT,"Barco colocado exitosamente de");
    printf_color(INFO_COLOR, DEFAULT, " (%c, %d) ", rowIn + 'A', colIn + 1);
    printf("a");
    printf_color(INFO_COLOR, DEFAULT, " (%c, %d)\n", rowEnd + 'A', colEnd + 1);
    pause_timer(.3);
    clear_screen();
}

void currentPlayerWarScreen(struct player *player, struct player *enemy) {
    int filler = 5;
    char output[10];
    player->cards_total_weight = calcTotalWeight(player); 

    printf("Turno de");
    printf_color(INFO_COLOR, DEFAULT," %s %*s", player->player_name, filler, "");
    
    printf(" | Modo Salvo: ");
    printf_color(player->salvo_mode ? SUCCESS_COLOR : ERROR_COLOR, DEFAULT, "%s ", player->salvo_mode ? "Activo" : "Inactivo");

    printf("| %s: ", player->upgrade_enable ? "Torre de ventaja" : "Torres acumuladas");
    if (player->upgrade_enable) {
        strcpy(output, "Activada");
    } else {
        snprintf(output, sizeof(output), "%d", player->acc_towers);
    }
    printf_color(player->upgrade_enable ? SUCCESS_COLOR : INFO_COLOR, DEFAULT, "%s", output);

    printf(" | ");
    deBugPrint(3, player->cards_total_weight, 0, EMPTY, EMPTY_STR);
    printf("\n");

    printPlayerWarBoard(player, enemy);
}

void printPlayerWarBoard(struct player *player, struct player *enemy) {
    int boardWidth, filler, windowWidth;
    int row, col, cellState;
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    // Get the console window size
    windowWidth = 80; // Default width
    if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        windowWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }

    // Filler calculation for centering the board
    boardWidth = BOARD_SIZE * 3 + 3; // Each column occupies 3 characters (" 1 ")
    filler = (windowWidth - boardWidth) / 2;

    printf("%*s", filler, ""); // Print filler spaces to center
    printf_color(INFO_COLOR, DEFAULT, "Tablero de Guerra\n");

    // Column header (1 2 3 ... 10)
    printf("%*s", filler,""); // Print filler spaces to center
    printf("  ");
    for (int num = 1; num <= BOARD_SIZE; num++) {
        num < BOARD_SIZE ? printf("%2d", num) : printf(" J");
    }
    printf("\n");

    // Draw each row
    set_CP_config(850);
    for (row = 0; row < BOARD_SIZE; row++) {

        // Row header (A B C ... J)
        printf("%*s%c  ", filler,"", 'A' + row);

        // Iterate over each column
        for (col = 0; col < BOARD_SIZE; col++) {
            cellState = enemy->board[row][col].status; // Obtaining the cell state

            // Print depending on the cell state
            switch (cellState) {
                case SHIP_STER_D:
                    printf_color(ERROR_COLOR, DEFAULT, "%c ", STER_PRINT);
                    break;
                case SHIP_BODY_D:
                    printf_color(ERROR_COLOR, DEFAULT, "%c ", BODY_PRINT);
                    break;
                case FAILED_SHOT:
                    printf("%c ", FAILED_SHOT_PRINT);
                    break;
                default:    
                    printf_color(WATER_COLOR, DEFAULT, "%c ", WATER_PRINT);
                    break;
            }
        }
    
        // Show row check at the end of the row only if enabled
        if (player->row_check[row]) {
            player->cellCntRow[row] = getEnemyCellsInRow(player, enemy, row); // Mostrar el número de barcos en la fila
            printf_color(SUCCESS_COLOR, DEFAULT, player->cellCntRow[row] == BOARD_SIZE ? "F " : "%d ",
                player->cellCntRow[row]); // F = Full, otherwise remaining ship parts
        }
        printf("\n");
    }
    set_CP_config(65001);
    // Show column check (one line below the board)
    printf("%*s   ", filler, "");
    for (col = 0; col < BOARD_SIZE; col++) {
        if (player->col_check[col]) {
            player->cellCntCol[col] = getEnemyCellsInCol(player, enemy, col); // Mostrar el número de barcos en la columna
            printf_color(SUCCESS_COLOR, DEFAULT, player->cellCntCol[col] == BOARD_SIZE ? "F " : "%d ",
                player->cellCntCol[col]); // F = Full, otherwise remaining ship parts
        }  else printf("  "); // Empty space if no check
    }
    printf("\n");
}

void printEnemyFleetReport(struct player *player, struct player *enemy) {
    char opc;

    currentPlayerWarScreen(player, enemy);
    printf("\n");
    printf("Reporte de barcos enemigos:\n");
    printf("Barcos enemigos hundidos: ");
    printf_color(INFO_COLOR, DEFAULT, "%d\n", player->sunken_ships);

    printf("Partes de barcos enemigos hundidos: ");
    printf_color(INFO_COLOR, DEFAULT, "%d\n", player->enemy_hit_parts);

    printf("Modo Salvo enemigo activado: ");
    printf_color(enemy->salvo_mode ? INFO_COLOR : SUCCESS_COLOR, DEFAULT,
        "%s\n", enemy->salvo_mode ? "Si" : "No");

    printf("Torre de ventaja enemigo activada: ");
    printf_color(enemy->upgrade_enable ? INFO_COLOR : SUCCESS_COLOR, DEFAULT,
        "%s\n", enemy->upgrade_enable ? "Si" : "No");

    printf("\n");

    printf("[z]: Regresar al menu.\n");
    
    do {
        scanf(" %c", &opc);
        clearInputBuffer();
        opc = toupper(opc);
        if (opc == 'Z') {
            return;
        } else {
            printf_color(ERROR_COLOR, DEFAULT, "¡Tecla invalida!\n");
        }
    } while (true);
}

void printFleetReport(struct player *player, struct player *enemy) {
    int ship_index, ship_cell;
    int max_dim = 0;
    int col_width = 12; // Fixed width for each ship
    int local_status;
    char opc;

    currentPlayerScreen(player, enemy);
    printf("\n");
    printf("Reporte de barcos:\n");

    // Find the maximum ship size to determine the number of rows needed
    for (ship_index = 0; ship_index < NUM_SHIPS; ship_index++) {
        if (player->ships[ship_index].ship_size > max_dim)
            max_dim = player->ships[ship_index].ship_size;
    }

    // Header
    for (ship_index = 0; ship_index < NUM_SHIPS; ship_index++) {
        printf("%*sBote %02d", col_width / 2 - 4, "", ship_index + 1);
        printf("%*s", col_width - (col_width / 2 + 4), "");
    }
    printf("\n");

    // Ster ship print (/'\)
    for (ship_index = 0; ship_index < NUM_SHIPS; ship_index++) {
        local_status = player->ships[ship_index].status[0][CC_STATUS];
        if (ship_index == 0) printf("%*s", col_width - 8, "");
        printf_color(local_status == SHIP_STER ? SHIP_COLOR : ERROR_COLOR, DEFAULT, "/'\\");
        printf("%*s", col_width - 4, "");
    }
    printf("\n");

    // Body ship print ([_] or [X])
    for (ship_cell = 1; ship_cell < max_dim; ship_cell++) {
        for (ship_index = 0; ship_index < NUM_SHIPS; ship_index++) {
            if (ship_cell < player->ships[ship_index].ship_size) {
                if (ship_index == 0) printf("%*s", col_width - 8, "");

                local_status = player->ships[ship_index].status[ship_cell][CC_STATUS];
                printf_color(local_status == SHIP_BODY ? SHIP_COLOR : ERROR_COLOR, DEFAULT,
                     local_status == SHIP_BODY ? "[_]" : "[X]");
                printf("%*s", col_width - 4, "");
            } else {
                printf("%*s", col_width, "");
            }
        }
    printf("\n");
    }

    printf("[Z]: Regresar al menu.\n");
    do {
        scanf(" %c", &opc);
        clearInputBuffer();
        opc = toupper(opc);
        if (opc == 'Z') {
            return;
        } else printf_color(ERROR_COLOR, DEFAULT, "¡Tecla invalida!\n");
    } while (true);
}

void currentPlayerScreen(struct player *player, struct player *enemy) {
    int filler = 5;
    char output[10];
    player->cards_total_weight = calcTotalWeight(player);
    
    printf("Turno de");
    printf_color(INFO_COLOR, DEFAULT, " %s %*s", player->player_name, filler, "");

    printf(" | Modo Salvo: ");
    printf_color(player->salvo_mode ? SUCCESS_COLOR : ERROR_COLOR, DEFAULT, "%s ", player->salvo_mode ? "Activo" : "Inactivo");

    printf("| %s: ", player->upgrade_enable ? "Torre de ventaja" : "Torres acumuladas");
    if (player->upgrade_enable) {
        strcpy(output, "Activada");
    } else {
        snprintf(output, sizeof(output), "%d", player->acc_towers);
    }
    printf_color(player->upgrade_enable ? SUCCESS_COLOR : INFO_COLOR, DEFAULT, "%s", output);

    printf(" | ");
    deBugPrint(3, player->cards_total_weight, 0, EMPTY, EMPTY_STR);
    printf("\n");

    printPlayerBoard(player, enemy);
}

void printCardInfo(struct card *carta) {
    printf("Te ha salido la carta:"); printf_color(INFO_COLOR, DEFAULT, " %s\n", carta->card_name);
}

void captureCoordToShot(struct player *player, struct player *enemy) {
    char input[32];
    char *nl;
    int row, column;

    do {
        puts("Ingrese las coordenadas (Fila: letras de A a la J; Columna: numeros del 1 al 10): ");
        printf("Ejemplo: "); printf_color(INFO_COLOR, DEFAULT, "A 1\n");
        printf("> ");

        if(!fgets(input, sizeof(input), stdin)) {
            clearInputBuffer();
            codeError(player, INVALID_INPUT, -1, 0, 0);
            continue; // If input is invalid, restart the loop
        }

        nl = strchr(input, '\n');
        if(nl) *nl = '\0'; // Remove newline character from end of input

        // Format validation
        if (!parsing_coords(input, &row, &column)) {
            continue; // Repeat the loop if the format is invalid
        }

        player->prevRowInput = row; // Store the entered row
        player->prevColInput = column; // Store the entered column

        if (!attack_in_range(player, row, column)) {
            continue; // Repeat the loop if the coordinates are out of range
        }

        if (!cell_is_valid_to_shot(player, enemy)) {
            continue;
        }
        break; // Exit the loop if the coordinates are valid
    } while (true);

    deBugPrint(4, player->prevRowInput, player->prevColInput + 1, EMPTY, EMPTY_STR);
    printf("\n");
    shoot_func(player, enemy);
}

void printShootreport(struct player *player, struct player *enemy, int index) {

    if (player->prevHitCol != -1 && player->prevHitRow != -1) {
        if (!is_sunk(player, enemy, index)) {
            printf_color(SUCCESS_COLOR, DEFAULT, "Disparo exitoso en: %c,%d\n", player->prevHitRow + 'A', player->prevHitCol + 1);
            return;
        }
        printf_color(SUCCESS_COLOR, DEFAULT, "Disparo exitoso en: %c,%d ", player->prevHitRow + 'A', player->prevHitCol + 1);
        printf("El %s (%d) enemigo ha sido hundido.\n", enemy->ships[index].ship_name, enemy->ships[index].ship_id + 1);
        if (enemy->salvo_mode) {
            enemy->salvo_mode = false; // Deactivate salvo mode after sinking a ship
            printf("El modo Salvo enemigo se "); printf_color(INFO_COLOR, DEFAULT, "desactiva.\n");
        }
        return;
    }
    printf_color(ERROR_COLOR, DEFAULT, "Disparo fallido en %c,%d! Disparaste al ", player->prevRowInput + 'A', player->prevColInput + 1);
    printf_color(WATER_COLOR, DEFAULT, "agua.\n");
}

void captrRowORColumn(struct player *player, struct player *enemy) {
    char input[32];
    char row_char, col_char;
    int col_num;

    if (player->prevCard == 3 || player->prevCard == 6) {
        // Row bomb/check: letter
        do {
            printf("Ingresa la fila que desees %s (A-%c): ",
                (player->prevCard == 3) ? "bombardear" : "revisar",'A' + BOARD_SIZE - 1);

            if (scanf(" %c", &row_char) == 0) {
                clearInputBuffer();
                deBugPrint(5, 0, 0, EMPTY, EMPTY_STR);
                printf("\n");
                codeError(player, INVALID_FORMAT, -1, 0, 0);
                continue;
            }
            row_char = toupper(row_char);
            player->prevRowInput = row_char - 'A';
            if (!attack_in_range(player, player->prevRowInput, player->prevColInput)) continue;

            deBugPrint(6, 0, 0, row_char, EMPTY_STR);
            printf("\n");
            break;
        } while (true);
        if (player->prevCard == 3) bomb_row(player, enemy);
        else {
            player->prev_check_row = player->prevRowInput;
            activateCheckInRow(player, enemy);
        }
    }
    else if (player->prevCard == 4 || player->prevCard == 7) {
        // Column bomb/check: number or 'J'
        do {
            printf("Ingresa la columna que desees %s (1-%d o J): ",
                   (player->prevCard == 4) ? "bombardear" : "revisar",
                   BOARD_SIZE);

            if (!fgets(input, sizeof(input), stdin)) {
                clearInputBuffer();
                deBugPrint(5, 0, 0, EMPTY, EMPTY_STR);
                printf("\n");

                codeError(player, INVALID_FORMAT, -1, 0, 0);
                continue;
            }
            deBugPrint(8, 0, 0, EMPTY, input);
            printf("\n");

            char *nl = strchr(input, '\n'); // Search for newline character
            if (nl) *nl = '\0'; // Remove newline character from end of input

            deBugPrint(9, 0, 0, EMPTY, input);
            printf("\n");

            // Case where input is a number (1-10)
            if (sscanf(input, "%d", &col_num) == 1) {
                if (col_num < 1 || col_num > BOARD_SIZE) {
                    codeError(player, INVALID_FORMAT, -1, 0, 0);
                    continue;
                }
                col_num--; // Convert to index
                deBugPrint(10, col_num, 0, EMPTY, EMPTY_STR);
                printf("\n");
            }
            // Case where input is a letter (A-J)
            else if (sscanf(input, " %c", &col_char) == 1) {
                col_char = toupper(col_char);
                if (col_char != 'J') {
                    codeError(player, INVALID_FORMAT, -1, 0, 0);
                    continue;
                }
                col_num = col_char - 'A'; // Convert to index
            } else {
                codeError(player, INVALID_FORMAT, -1, 0, 0);
                continue;
            }
            player->prevColInput = col_num;
            if (!attack_in_range(player, player->prevRowInput, player->prevColInput)) continue;

            break;
        } while (true);
        if (player->prevCard == 4) bomb_col(player, enemy);
        else {
            player->prev_check_col = player->prevColInput;
            activateCheckInCol(player, enemy);
        }
    }
}

void printBombORCheckSucces(struct player *player) {

    switch (player->prevCard) {
    case 3:
        if (player->prevHitRow != -1) {
            printf("Bombardeo exitoso en la fila: %c\n", player->prevHitRow + 'A');
            printf("Casillas enemigas destruidas: %d\n", player->hitsInTurn);
        } else printf("No habian barcos en la fila: %c.\n", player->prevRowInput + 'A');
        break;
    case 4:
        if (player->prevHitCol != -1) {
            printf("Bombardeo exitoso en la columna: %d\n", player->prevHitCol + 1);
            printf("Casillas enemigas destruidas: %d\n", player->hitsInTurn);
        } else printf("No habian barcos en la columna: %d.\n", player->prevColInput + 1);
        break;
    case 6:
        if (player->cellCntRow[player->prev_check_row] > 0) {
            printf("Chequeo exitoso en la fila: %c\n", player->prev_check_row + 'A');
            printf("Casillas enemigas encontradas: %d\n", player->cellCntRow[player->prev_check_row]);
        } else printf("No se han encontrado casillas enemigas en la fila: %c\n", player->prev_check_row + 'A');
        break;
    case 7:
        if (player->cellCntCol[player->prev_check_col] > 0) {
            printf("Chequeo exitoso en la columna: %d\n", player->prev_check_col + 1);
            printf("Casillas enemigas encontradas: %d\n", player->cellCntCol[player->prev_check_col]);
        } else printf("No se han encontrado casillas enemigas en la columna: %d\n", player->prev_check_col + 1);
        break;
    }
}