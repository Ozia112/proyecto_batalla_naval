#include "bs_ui.h" // libreria para poder usar funciones de UI.

void tittle_screen() {
    // Logo para la pantalla de inicio.
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

	// Menu de opciones inicial.
	do{
        tittle_screen(); // Mostrar la pantalla de bienvenida.
        
        printf_color(INFO_COLOR, "\n\t\t=== MENU PRINCIPAL ===\n\n");

		printf("Ingrese la opcion que desee realizar.\n");
		printf("Opciones:\n");
        printf("[A]: Empezar\n");
        printf("[B]: Ver reglas.\n");
        printf("[C]: Salir\n");
		scanf(" %c", &opc);
        clearInputBuffer(); // Limpiar el buffer de entrada
        opc = toupper(opc); // Convertir a mayúscula para evitar problemas de comparación.
        if (opc == 'A' || opc == 'B' || opc == 'C') {
            clear_screen(); // Limpiar la pantalla antes de mostrar el menú.
        }
		switch(opc) {
		    // Caso en el que el usuario quiere iniciar partida.
            case 'A':
                flow_game(player1, player2); // Llamar a la función que inicia el juego.
                break;
            // caso en el que el usuario quiere leer las reglas.
            case 'B':
                rules();
                break;
            // Caso en el que el usuario quiere salir del programa (se especifica con un "case" porque sino el programa imprime "tecla invalida" cuando se presiona la c).
            case 'C':
                printf("Gracias por haber usado el programa :)\n");
                return;
            default:
                puts("¡Tecla invalida!");
                pause_timer(1); // funcion para tener demora en la ejecucion de la limpieza de la terminal.
                clear_screen(); // Limpiar la pantalla.
                break;
            }
	}while(true); // Mientras loop sea verdadero, el menu se repetira.
}

void rules() {
    char opc;
    do {
        printf_color(INFO_COLOR, "=== REGLAS DEL JUEGO \"BATALLA NAVAL\" ===\n\n");
        printf_color(SUCCESS_COLOR, "1. Cada jugador contara con una flota de barcos,\n");
        printf("   la cual ordenara en una distribucion que considere conveniente para ocultarla de su rival.\n\n");
        printf_color(SUCCESS_COLOR, "2. El juego es por turnos.\n");
        printf("   En cada turno se le proporcionara a cada jugador la posibilidad de usar una ");
        printf_color(INFO_COLOR, "carta");
        printf(" con diversos efectos sobre el tablero.\n\n");
        printf_color(SUCCESS_COLOR, "3. Las cartas disponibles son:\n");
        printf("   ");
        printf_color(INFO_COLOR, "- Disparo: ");
        printf("Dispara a una coordenada del tablero enemigo.\n   ");
        printf_color(INFO_COLOR, "- Bombardeo de fila: ");
        printf("Bombardea una fila del tablero enemigo.\n   ");
        printf_color(INFO_COLOR, "- Bombardeo de columna: ");
        printf("Bombardea una columna del tablero enemigo.\n   ");
        printf_color(INFO_COLOR, "- Revelar: ");
        printf("Revela el estado de los barcos enemigos.\n   ");
        printf_color(INFO_COLOR, "- Chequeo de fila: ");
        printf("Revela el estado de una fila del tablero enemigo.\n   ");
        printf_color(INFO_COLOR, "- Chequeo de columna: ");
        printf("Revela el estado de una columna del tablero enemigo.\n   ");
        printf_color(INFO_COLOR, "- Salvo: ");
        printf("Permite disparar a una coordenada del tablero enemigo sin que el enemigo pueda defenderse.\n   ");
        printf_color(INFO_COLOR, "- Torre de ventaja: ");
        printf("Permite disparar a una coordenada y revela el estado de la fila y columna del barco.\n   ");
        printf_color(INFO_COLOR, "- Revelar casilla aleatoria: ");
        printf("Permite revelar una casilla aleatoria en la que haya un barco enemigo.\n\n");
        printf_color(SUCCESS_COLOR, "4. El objetivo del juego es ");
        printf_color(INFO_COLOR, "hundir todos los barcos del enemigo");
        printf(" antes de que el enemigo hunda los tuyos.\n\n");
        printf_color(SUCCESS_COLOR, "5. Un barco se considera hundido cuando ");
        printf_color(INFO_COLOR, "todas sus partes han sido alcanzadas.");
        printf("\n\n");
        printf_color(ERROR_COLOR, " PROHIBIDO\n");
        printf("   ");
        printf_color(ERROR_COLOR, "- No puedes colocar barcos fuera del tablero.\n");
        printf("   ");
        printf_color(ERROR_COLOR, "- No puedes sobreponer barcos.\n");
        printf("   ");
        printf_color(ERROR_COLOR, "- No puedes disparar dos veces a la misma casilla.\n");
        printf("   ");
        printf_color(ERROR_COLOR, "- No puedes mover barcos una vez colocados.\n\n");
        printf_color(INFO_COLOR, "Consejos:\n");
        printf("   - Piensa estrategicamente la ubicacion de tus barcos.\n");
        printf("   - Aprovecha las cartas especiales para obtener ventaja.\n");
        printf("   - Recuerda que la informacion revelada por cartas es temporal.\n\n");

        printf("[Z]: Regresar al"); printf_color(INFO_COLOR, " menu.\n");

        scanf(" %c", &opc);
        clearInputBuffer();
        clear_screen(); // Limpiar la pantalla antes de mostrar el
        opc = toupper(opc);
    } while (opc != 'Z');
}

void get_name(struct player *player) {
    do {
        printf("Introduce el nombre del jugador (max 20 caracteres): ");
        if (fgets(player->player_name, sizeof(player->player_name), stdin) == EMPTY) {
            printf_color(ERROR_COLOR, "Error de entrada. Intenta de nuevo.\n");
            continue;
        }

        // Si no hay salto de línea, limpiar el buffer y repetir
        if (strchr(player->player_name, '\n') == EMPTY) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF); // Limpiar buffer completamente
            printf_color(ERROR_COLOR, "El nombre no puede estar vacio. Intenta de nuevo.\n");
            continue;
        }

        // Eliminar salto de línea
        player->player_name[strcspn(player->player_name, "\n")] = '\0';

        // Validar longitud
        size_t len = strlen(player->player_name);
        if (len > 20) {
            printf_color(ERROR_COLOR, "El nombre no puede tener mas de 20 caracteres. Intenta de nuevo.\n");
            continue;
        }

        // Si pasa todas las validaciones, salir del ciclo
        save_player_state(player); // Guardar el estado del jugador en un archivo JSON.
        break;
    } while (1);
}

void set_ships_screen(struct player *player, int index, struct player *enemy) {
    printf("Colocando barcos de"); 
    printf_color(INFO_COLOR," %s\n", player->player_name);
    printCurrentPlayerBoard(player, enemy);
    printf("Coloca tu %s (", player->ships[index].ship_name); printf_color(INFO_COLOR," %d ",player->ships[index].ship_size);
    printf("celdas).\nBarco"); printf_color(INFO_COLOR," %d ",index + 1);
    printf("de"); printf_color(INFO_COLOR," %d\n",NUM_SHIPS);
}

void set_ships_flow(struct player *player, struct player *enemy) {
    for(int index_ship = 0; index_ship < NUM_SHIPS; index_ship++) {
        getSetShipsCoords(player, index_ship, enemy);
    }
    printf("Colocando barcos de"); printf_color(INFO_COLOR," %s\n", player->player_name);
    printCurrentPlayerBoard(player, enemy);
    get_remain_fleet_cells(player); // Calcular partes restantes de barcos.
    save_player_state(player); // Guardar el estado del jugador en un archivo JSON.
    printf("Barcos colocados correctamente. \n");
    pause_timer(0.2);
    enter_continuar_cls();
}

void printCurrentPlayerBoard(struct player *player, struct player *enemy) {
    int boardWidth, filler, windowWidth;
    int row, col, cellState;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    
    // Obtener ancho de consola
    windowWidth = 80; //Por defecto si no es posible
    if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        windowWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }

    // Calculando relleno para centrar
    boardWidth = BOARD_SIZE * 2 + 3; // Cada columna ocupa 3 caracteres (" 1 ")
    filler = (windowWidth - boardWidth) / 2;

    printf_color(INFO_COLOR, "\n%*s Tablero de Flota\n", filler, "");

    // Encabezado de columnas (1 2 3 ... 10)
    printf("%*s", filler,""); // Imprime relleno con espacios para centrar
    printf("  ");
    for (int num = 0; num < BOARD_SIZE; num++) {
        num + 1 < BOARD_SIZE ? printf("%2d", num + 1) : printf(" J");
    }
    printf("\n");

    // Dibujar cada fila
    for (row = 0; row < BOARD_SIZE; row++) {
        // Encabezado de filas (A B C ... J)
        printf("%*s%c  ", filler,"", 'A' + row);

        // Recorrer cada columna
        for (col = 0; col < BOARD_SIZE; col++) {
            cellState = player->board[row][col].status; // Obtener el estado de la celda
            
            // Imprimir el resultado según el cellState
            switch (cellState) {
                case SHIP_STER:
                    printf_color(SHIP_COLOR, "%c ", STER_PRINT);
                    break;
                case SHIP_BODY:
                    printf_color(SHIP_COLOR, "%c ", BODY_PRINT);
                    break;
                case SHIP_STER_D:
                    printf_color(ERROR_COLOR, "%c ", STER_PRINT);
                    break;
                case SHIP_BODY_D:
                    printf_color(ERROR_COLOR, "%c ", BODY_PRINT);
                    break;
                case FAILED_SHOT:
                    printf_color(INFO_COLOR, "%c ", FAILED_SHOT_PRINT);
                    break;
                default:
                    printf_color(WATER_COLOR, "%c ", WATER_PRINT);
                    break;
            }
        }
        printf("\n");
    }
}

void getSetShipsCoords(struct player *player, int index, struct player *enemy) {
    char input[32], *nl, opc; // Buffer para la entrada del usuario
    int rowIn,  columnIn;
    int rowEnd, columnEnd;

    REINICIAL:
    //-------------------------- Coordenada inicial ------------------------------
    while (true) {
        set_ships_screen(player, index, enemy);
        printf("Coordenada inicial del barco (Fila: letras de A a la J; Columna: numero del 1 al 10): \n");
        printf("Ejemplo:"); printf_color(INFO_COLOR, "A 1");
        printf("\n> ");

        if (!fgets(input, sizeof(input), stdin)) {
            clearInputBuffer();
            codeError(EMPTY, INVALID_INPUT, -1, 0, 0);
            enter_continuar_cls(); // Esperar a que el usuario presione ENTER
            continue; // Si falla la lectura, repetir
        }

        nl = strchr(input, '\n'); // Buscar salto de línea
        if (nl) *nl = '\0'; // Eliminar salto de línea al final de la entrada

        // Validar formato de las coordenadas
        if(!parsing_coords(input, &rowIn, &columnIn)) {
            enter_continuar_cls(); // Esperar a que el usuario presione ENTER
            continue;
        }

        // Ahora rowIn ∈ [0..9], columnIn ∈ [0..9] y acepta "JJ", "J10", etc. 
        if (!in_range(rowIn, columnIn)) {
            enter_continuar_cls(); // Esperar a que el usuario presione ENTER
            continue; // Validar rango de coordenadas
        }
        if(!first_coord_is_free(player, index, rowIn, columnIn)) {
            enter_continuar_cls(); // Esperar a que el usuario presione ENTER
            continue; // Si hay solapamiento, continuar pidiendo coordenadas
        }

        // Si la coordenada inicial es valida, imprimir menú de confirmación
        while (true) {
            char confirm_input[8];
            printf("Presiona"); printf_color(INFO_COLOR, " ENTER"); printf(" para confirmar.\n");
            printf("[Z]: Intentar de nuevo.\n");
            if (!fgets(confirm_input, sizeof(confirm_input), stdin)) {
                clearInputBuffer();
                printf_color(ERROR_COLOR, "Opcion invalida. Intente de nuevo\n");
                continue;
            }
            // Si solo se presiona ENTER
            if (confirm_input[0] == '\n') break;
            // Si se presiona Z o z
            if (toupper((unsigned char)confirm_input[0]) == 'Z') {
                clear_screen(); // Limpiar la pantalla antes de reingresar coordenada inicial
                goto REINICIAL;
            }
            printf_color(ERROR_COLOR, "Opcion invalida. Intente de nuevo\n");
        }
        clear_screen(); // Limpiar la pantalla después de confirmar coordenadas
        break;
    }
    set_initial_cell(player, index, rowIn, columnIn); // Colocar la casilla inicial en el tablero
    printf_color(SUCCESS_COLOR, "Coordenada inicial colocada exitosamente");
    save_player_state(player); // Guardar el estado del jugador en un archivo JSON.
    printf_color(SUCCESS_COLOR, "Coordenada inicial guardada exitosamente");

REFINAL:
    // --------------------------- Coordenada final ------------------------------
    while (true) {
        set_ships_screen(player, index, enemy);
        printf("Coordenada final del barco (Fila: letras de A a la J; Columna: numero del 1 al 10): \n");
        printf("[Z]: Reingresar coordenada inicial.\n");
        printf("Ejemplo:"); printf_color(INFO_COLOR, "A 1\n");
        printf("> ");

        if (!fgets(input, sizeof(input), stdin)) {
            clearInputBuffer();
            codeError(EMPTY, INVALID_INPUT, -1, 0, 0);
            enter_continuar_cls();
            continue;
        }

        // Intento letra+letra (e.j. "JJ" o "J J")
        if(!parsing_coords(input, &rowEnd, &columnEnd)) {
            enter_continuar_cls();
            continue;
        }

        //validaciones
        if (!coords_validation(player, index, rowIn, rowEnd, columnIn, columnEnd)) {
            enter_continuar_cls(); // Esperar a que el usuario presione ENTER
            continue; 
        }

        while (true) {  // Logica de confirmacion de coordenadas
            char confirm_input[8];
            printf("Presiona "); printf_color(INFO_COLOR, "ENTER"); 
            printf(" para confirmar.\n");
            printf("[Z]: Intentar de nuevo.\n");

            if (!fgets(confirm_input, sizeof(confirm_input), stdin)) {
                clearInputBuffer();
                printf_color(ERROR_COLOR, "Opcion invalida. Intente de nuevo\n");
                continue;
            }
            // Si solo se presiona ENTER
            if (confirm_input[0] == '\n') goto SETSHIP;
            // Si se presiona Z o z
            if (toupper((unsigned char)confirm_input[0]) == 'Z') {
                clear_screen(); // Limpiar la pantalla antes de reingresar coordenada final
                goto REFINAL;
            }
            printf_color(ERROR_COLOR, "Opcion invalida. Intente de nuevo\n");
        }        
        break;
    }

SETSHIP:
    set_ship(player, index, rowIn, rowEnd, columnIn, columnEnd);
    player->placed_ships++; // Incrementar el contador de barcos colocados."
    
    printf_color(SUCCESS_COLOR, "Barco colocado exitosamente de");
    printf_color(INFO_COLOR, " (%c, %d) ", rowIn + 'A', columnIn + 1);
    printf("a");
    printf_color(INFO_COLOR, " (%c, %d)\n", rowEnd + 'A', columnEnd + 1);
    save_player_state(player); // Guardar el estado del jugador en un archivo JSON.
    printf_color(SUCCESS_COLOR, "Pocision del barco guardada exitosamente.\n");
    pause_timer(.3); // Pausa para mostrar el mensaje de éxito.
    clear_screen(); // Limpiar la pantalla después de colocar el barco.
}

void currentPlayerWarScreen(struct player *player, struct player *enemy) {
    int espacios = 5; // Espacios para centrar el texto
    char output[10];

    printf("Turno de");
    printf_color(INFO_COLOR, " %s %*s", player->player_name, espacios, "");

    printf(" | Modo Salvo: ");
    printf_color(player->salvo_mode ? SUCCESS_COLOR : ERROR_COLOR, "%s ", player->salvo_mode ? "Activo" : "Inactivo");
    
    printf("| %s: ", player->upgrade_enable ? "Torre de ventaja" : "Torres acumuladas");
    if (player->upgrade_enable) {
        strcpy(output, "Activada");
    } else {
        snprintf(output, sizeof(output), "%d", player->acc_towers);
    }
    printf_color(player->upgrade_enable ? SUCCESS_COLOR : INFO_COLOR, "%s\n", output);

    printCurrentPlayerWarBoard(player, enemy); // Imprimir el tablero del enemigo.
}

void printCurrentPlayerWarBoard(struct player *player, struct player *enemy) {
    int boardWidth, filler, windowWidth;
    int row, col, cellState;
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    // Obtener ancho de consola
    windowWidth = 80; //Por defecto si no es posible
    if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        windowWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }

    // Calculando relleno para centrar
    boardWidth = BOARD_SIZE * 3 + 3; // Cada columna ocupa 3 caracteres (" 1 ")
    filler = (windowWidth - boardWidth) / 2;

    printf("%*s", filler, ""); // Imprime relleno con espacios para centrar
    printf_color(INFO_COLOR, "Tablero de Guerra\n");
    // Encabezado de columnas (1 2 3 ... 10)
    printf("%*s", filler,""); // Imprime relleno con espacios para centrar
    printf("  ");
    for (int num = 1; num <= BOARD_SIZE; num++) {
        num < BOARD_SIZE ? printf("%2d", num) : printf(" J");
    }
    printf("\n");

    // Dibujar cada fila
    for (row = 0; row < BOARD_SIZE; row++) {

        // Encabezado de filas (A B C ... J)
        printf("%*s%c  ", filler,"", 'A' + row);

        // Recorrer cada columna
        for (col = 0; col < BOARD_SIZE; col++) {
            cellState = enemy->board[row][col].status; // Obtener el estado de la celda
            
            switch (cellState) {
                case SHIP_STER_D:
                    printf_color(ERROR_COLOR, "%c ", STER_PRINT);
                    break;
                case SHIP_BODY_D:
                    printf_color(ERROR_COLOR, "%c ", BODY_PRINT);
                    break;
                case FAILED_SHOT:
                    printf("%c ", FAILED_SHOT_PRINT);
                    break;
                default:    
                    printf_color(WATER_COLOR, "%c ", WATER_PRINT);
                    break;
            }
        }
        // Mostrar chequeo de fila al final de la fila solo si está activado
        if (player->row_check[row]) {
            player->cellCntRow[row] = getEnemyCellsInRow(player, enemy, row); // Mostrar el número de barcos en la fila
            printf_color(SUCCESS_COLOR, player->cellCntRow[row] == BOARD_SIZE ? "F " : "%d ",
                player->cellCntRow[row]); // F = Full, o el número de barcos restantes
        }
        printf("\n");
    }

    // Mostrar chequeo de columna (una sola línea debajo del tablero)
    printf("%*s   ", filler, "");
    for (col = 0; col < BOARD_SIZE; col++) {
        if (player->col_check[col]) {
            player->cellCntCol[col] = getEnemyCellsInCol(player, enemy, col); // Mostrar el número de barcos en la columna
            printf_color(SUCCESS_COLOR, player->cellCntCol[col] == BOARD_SIZE ? "F " : "%d ",
                player->cellCntCol[col]); // F = Full, o el número de barcos restantes
        }  else printf("  "); // Espacio vacío si no hay chequeo
    }
    printf("\n");
}

void printEnemyFleetReport(struct player *player, struct player *enemy) {
    char opc;

    currentPlayerWarScreen(player, enemy);
    printf("\n");
    printf("Reporte de barcos enemigos:\n");
    printf("Barcos enemigos hundidos: ");
    printf_color(INFO_COLOR, "%d\n", player->sunken_ships);

    printf("Partes de barcos enemigos hundidos: ");
    printf_color(INFO_COLOR, "%d\n", player->enemy_hit_parts);

    printf("Modo Salvo enemigo activado: ");
    printf_color(enemy->salvo_mode ? INFO_COLOR : SUCCESS_COLOR, 
        "%s\n", enemy->salvo_mode ? "Si" : "No");

    printf("Torre de ventaja enemigo activada: ");
    printf_color(enemy->upgrade_enable ? INFO_COLOR : SUCCESS_COLOR, 
        "%s\n", enemy->upgrade_enable ? "Si" : "No");

    printf("\n");

    printf("[z]: Regresar al menu.\n");
    
    do {
        scanf(" %c", &opc);
        clearInputBuffer(); // Limpiar el buffer de entrada
        opc = toupper(opc);
        if (opc == 'Z') {
            return;
        } else {
            printf_color(ERROR_COLOR, "¡Tecla invalida!\n");
        }
    } while (true);
}

void printFleetReport(struct player *player, struct player *enemy) {
    int ship_index, ship_cell;
    int max_dim = 0;
    int col_width = 12; // Ancho fijo para cada barco
    int local_status;
    char opc;

    currentPlayerScreen(player, enemy);
    printf("\n");
    printf("Reporte de barcos:\n");

    // Encontrar el tamaño máximo de los barcos
    for (ship_index = 0; ship_index < NUM_SHIPS; ship_index++) {
        if (player->ships[ship_index].ship_size > max_dim)
            max_dim = player->ships[ship_index].ship_size;
    }

    // Encabezado
    for (ship_index = 0; ship_index < NUM_SHIPS; ship_index++) {
        printf("%*sBote %02d", col_width / 2 - 4, "", ship_index + 1);
        printf("%*s", col_width - (col_width / 2 + 4), "");
    }
    printf("\n");

    // Dibujo de proa (/'\)
    for (ship_index = 0; ship_index < NUM_SHIPS; ship_index++) {
        local_status = player->ships[ship_index].status[0][CC_STATUS];
        if (ship_index == 0) printf("%*s", col_width - 8, "");
        printf_color(local_status == SHIP_STER ? SHIP_COLOR : ERROR_COLOR, "/'\\");
        printf("%*s", col_width - 4, "");
    }
    printf("\n");

    // Cuerpo del barco
    for (ship_cell = 1; ship_cell < max_dim; ship_cell++) {
        for (ship_index = 0; ship_index < NUM_SHIPS; ship_index++) {
            if (ship_cell < player->ships[ship_index].ship_size) {
                if (ship_index == 0) printf("%*s", col_width - 8, "");

                local_status = player->ships[ship_index].status[ship_cell][CC_STATUS];
                printf_color(local_status == SHIP_BODY ? SHIP_COLOR : ERROR_COLOR,
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
        clearInputBuffer(); // Limpiar el buffer de entrada
        opc = toupper(opc);
        if (opc == 'Z') {
            return; // Salir del reporte de flota
        } else printf_color(ERROR_COLOR, "¡Tecla invalida!\n");
    } while (true);
}

void currentPlayerScreen(struct player *player, struct player *enemy) {
    int espacios = 5; // Espacios para centrar el texto
    char output[10];
    
    printf("Turno de");
    printf_color(INFO_COLOR, " %s %*s", player->player_name, espacios, "");
    
    printf(" | Modo Salvo: ");
    printf_color(player->salvo_mode ? SUCCESS_COLOR : ERROR_COLOR, "%s ", player->salvo_mode ? "Activado" : "Desactivado") ;
    
    printf("| %s: ", player->upgrade_enable ? "Torre de ventaja" : "Torres acumuladas");
    if (player->upgrade_enable) {
        strcpy(output, "Activada");
    }
    else {
        snprintf(output, sizeof(output), "%d", player->acc_towers);
    }
    printf_color(player->upgrade_enable ? SUCCESS_COLOR : INFO_COLOR, "%s", output);
    
    printCurrentPlayerBoard(player, enemy); // Imprimir el tablero del jugador.
}

void printCardInfo(struct card *carta) {
    printf("Te ha salido la carta:"); printf_color(INFO_COLOR, " %s\n", carta->card_name);
}

void captureCoordToShot(struct player *player, struct player *enemy) {
    char input[32];
    char *nl;
    int row, column;

    do {
        puts("Ingrese las coordenadas (Fila: letras de A a la J; Columna: numeros del 1 al 10): ");
        printf("Ejemplo: "); printf_color(INFO_COLOR, "A 1\n");
        printf("> ");

        if(!fgets(input, sizeof(input), stdin)) {
            clearInputBuffer();
            codeError(EMPTY, INVALID_INPUT, -1, 0, 0);
            continue; // Si falla la lectura, repetir
        }

        nl = strchr(input, '\n');
        if(nl) *nl = '\0'; // Eliminar el salto de línea al final de la entrada

        // Validar el formato de las coordenadas
        if (!parsing_coords(input, &row, &column)) {
            continue; // Repetir el ciclo si el formato es invalido
        }
        
        player->prevRowInput = row; // Guardar la fila ingresada
        player->prevColInput = column; // Guardar la columna ingresada

        if (!attack_in_range(player, row, column)) {
            continue; // Repetir el ciclo si las coordenadas están fuera de rango
        }

        if (!cell_is_valid_to_shot(player, enemy)) {
            continue;
        }
        break; // Salir del ciclo si las coordenadas son válidas
    } while (true);

    printf("Coordenadas ingresadas: %c, %d\n",
           player->prevRowInput + 'A',
           player->prevColInput + 1);

    shoot_func(player, enemy);
}

void printPrevHitCoord(struct player *player) {
    if (player->prevHitCol != -1 && player->prevHitRow != -1) {
        printf_color(SUCCESS_COLOR, 
            "Disparo exitoso en: %c,%d", player->prevHitRow + 'A', player->prevHitCol + 1);
    } else {
        printf_color(ERROR_COLOR, "No se ha realizado ningun disparo exitoso.\n");
    }
}

void captrRowORColumn(struct player *player, struct player *enemy) {
    char input[32];
    char row_char, col_char;
    int col_num;

    if (player->prevCard == 3 || player->prevCard == 6) {
        // Bombardeo/Chequeo de fila: letra
        do {
            printf("Ingresa la fila que desees %s (A-%c): ",
                (player->prevCard == 3) ? "bombardear" : "revisar",'A' + BOARD_SIZE - 1);

            if (scanf(" %c", &row_char) == 0) {
                clearInputBuffer();
                codeError(EMPTY, INVALID_FORMAT, -1, 0, 0);
                continue;
            }
            row_char = toupper(row_char);
            player->prevRowInput = row_char - 'A';
            if (!attack_in_range(player, player->prevRowInput, player->prevColInput)) continue;

            break;
        } while (true);
        if (player->prevCard == 3) bomb_row(player, enemy);
        else {
            player->prev_check_row = player->prevRowInput;
            activateCheckInRow(player, enemy);
        }
    }
    else if (player->prevCard == 4 || player->prevCard == 7) {
        // Bombardeo/Chequeo de columna: número/letra
        do {
            printf("Ingresa la columna que desees %s (1-%d o J): ",
                   (player->prevCard == 4) ? "bombardear" : "revisar",
                   BOARD_SIZE);

            if (!fgets(input, sizeof(input), stdin)) {
                clearInputBuffer();
                codeError(EMPTY, INVALID_FORMAT, -1, 0, 0);
                continue;
            }
            // Eliminar salto de línea
            char *nl = strchr(input, '\n');
            if (nl) *nl = '\0';

            // Intentar leer como número
            if (sscanf(input, "%d", &col_num) == 1) {
                if (col_num < 1 || col_num > BOARD_SIZE) {
                    codeError(EMPTY, INVALID_FORMAT, -1, 0, 0);
                    continue;
                }
                col_num--; // Convertir a índice (1=0, 2=1, ..., 10=9)
            } else if (sscanf(input, " %c", &col_char) == 1) {
                col_char = toupper(col_char);
                if (col_char != 'J') {
                    codeError(EMPTY, INVALID_FORMAT, -1, 0, 0);
                    continue; // Si no es 'J', repetir
                }
                col_num = col_char - 'A'; // 'J' - 'A' = 9
            } else {
                codeError(EMPTY, INVALID_FORMAT, -1, 0, 0);
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
        } else printf("No habian barcos en la fila: %c.\n", player->prevHitRow + 'A');
        break;
    case 4:
        if (player->prevHitCol != -1) {
            printf("Bombardeo exitoso en la columna: %d\n", player->prevHitCol + 1);
            printf("Casillas enemigas destruidas: %d\n", player->hitsInTurn);
        } else printf("No habian barcos en la columna: %d.\n", player->prevHitCol + 1);
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