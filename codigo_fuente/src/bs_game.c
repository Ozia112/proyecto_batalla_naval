#include "bs_game.h"
void flow_game(struct player *player1, struct player *player2) {
    bool game_in_progress = false;
    bool invalid;
    FILE *file1 = fopen("configs/player1_save.json", "r");
    FILE *file2 = fopen("configs/player2_save.json", "r");
    if (file1 && file2) game_in_progress = true;
    if (file1) fclose(file1);
    if (file2) fclose(file2);

    do {
        invalid = false;
        printf_color(INFO_COLOR, "\n\t\t=== Elige una opcion ===\n\n");
        printf("[A]: Iniciar partida nueva.\n");
        if(game_in_progress) printf("[B]: Reanudar partida.\n");
        printf("[C]: Regresar.\n");
        char opc;
        scanf(" %c", &opc);
        clearInputBuffer();
        opc = toupper(opc);

        switch(opc) {
            case 'A':
                flow_new_game(player1, player2);
                game_in_progress = false; // Actualizar estado para nueva partida
                break;
            case 'B':
                flow_resume_game(player1, player2);
                break;
            case 'C':
                clear_screen();
                return;
            default:
                invalid = true;
                printf_color(ERROR_COLOR, "¡Tecla invalida!\n");
                pause_timer(1);
                clear_screen();
        }
        pause_timer(2); // Pausa para que el usuario vea de progreso.
        printf_color(INFO_COLOR, "Iniciando partida...\n");
        pause_timer(1); // Pausa para que el usuario vea el mensaje de inicio.
        clear_screen(); // Limpiar la pantalla después de cada opción.
        if (!invalid) break;
    } while(true);

    if(!game_in_progress) registro_jugadores(player1, player2); // Registro de jugadores.
    partida(player1, player2); // Iniciar la partida.
}

void flow_new_game(struct player *player1, struct player *player2) {
    delete_player_file(player1);
    delete_player_file(player2);
    printf_color(INFO_COLOR, "Creando archivos de jugadores...\n");
    pause_timer(3);
    createPlayerFile(player1);
    createPlayerFile(player2);
    printf_color(INFO_COLOR, "Cargando archivos en memoria...\n");
    pause_timer(3);
    getPlayerState(player1);
    getPlayerState(player2);
}

void flow_resume_game(struct player *player1, struct player *player2) {
    getPlayerState(player1);
    getPlayerState(player2);
}

void registro_jugadores(struct player *player1, struct player *player2) {
    printf("Jugador 1:\n");
    get_name(player1);
    clear_screen(); // Limpiar pantalla despues del primer jugador.
    printf("Jugador 2:\n");
    get_name(player2);
    clear_screen(); // Limpiar la pantalla después de inicializar jugadores.
    
    printf("Jugadores:\n");
    printf("Jugador 1:"); color_txt(INFO_COLOR); printf(" %s\n", player1->player_name); color_txt(DEFAULT_COLOR);
    printf("Jugador 2:"); color_txt(INFO_COLOR); printf(" %s\n", player2->player_name); color_txt(DEFAULT_COLOR);
    enter_continuar_cls(); // Pausa antes de continuar.
    
    // Colocar barcos para el jugador 1.
    set_ships_flow(player1, player2); // Colocar barcos para el jugador 1.

    // Colocar barcos para el jugador 2.
    printf("Ahora es turno de"); color_txt(INFO_COLOR); printf(" %s\n", player2->player_name); color_txt(DEFAULT_COLOR);
    enter_continuar_cls(); // Pausa antes de continuar.

    set_ships_flow(player2, player1);

    // Elegir aleatoriamente quién inicia
    firstTurnRand(player1, player2);

    save_player_state(player1); // Guardar el estado de los jugadores en un archivo JSON.
    save_player_state(player2);
    pause_timer(1);
    enter_continuar_cls();
}

void partida(struct player *player1, struct player *player2) {
    struct player *jugador_actual;
    struct player *jugador_enemigo;
    struct player *temp;
    if (player1->acc_turns == 0 && player2->acc_turns == 0) {
        if(player1->turn == 1) {
            jugador_actual = player1;
            jugador_enemigo = player2;
        } else {
            jugador_actual = player2;
            jugador_enemigo = player1;
        }
    } else if (player1->acc_turns != player2->acc_turns) {
        player1->acc_turns < player2->acc_turns ? // Si el jugador 1 tiene menos turnos acumulados, es su turno.
            (jugador_actual = player1, jugador_enemigo = player2) :
            (jugador_actual = player2, jugador_enemigo = player1);
    }
    

    // Bucle principal del juego
    do {
        menu_por_turno(jugador_actual, jugador_enemigo);
        save_player_state(player1); // Guardar el estado de los jugadores en cada turno.
        save_player_state(player2);

        // Despues de acabar el turno, se muestra mensaje de que el turno ha terminado.
        printf("Tu turno ha terminado.\n");
        enter_continuar_cls(); // Pausa antes de continuar.
        
        // Si el jugador actual ya ha ganado, saltar al final del ciclo.
        if (jugador_enemigo->remain_ship_cells == 0) break;

        // Mensaje de cambio de turno
        printf("Ahora es turno de %s\n", jugador_enemigo->player_name);
        enter_continuar_cls();

        // Resetear el registro de input del jugador actual
        reset_buffer_register(jugador_actual);
        jugador_actual->acc_turns++; // Incrementar el contador de turnos del jugador actual.

        save_player_state(player1); // Guardar el estado de los jugadores antes de cambiar de turno.
        save_player_state(player2);

        // Alternar punteros (esto es lo que realmente cambia el turno)
        temp = jugador_actual;
        jugador_actual = jugador_enemigo;
        jugador_enemigo = temp;

    } while(player1->remain_ship_cells > 0 && player2->remain_ship_cells > 0);

    // Mensaje de victoria
    color_txt(INFO_COLOR);
    if (player1->enemy_hit_parts >= VICTORY_COND) {
        printf("¡%s ", player1->player_name);
    } else if (player2->enemy_hit_parts >= VICTORY_COND) {
        printf("¡%s ", player2->player_name);
    }
    color_txt(SUCCESS_COLOR);
        printf("ha ganado la partida!\n");
    color_txt(DEFAULT_COLOR);

    liberar_flota(player1); // Liberar memoria de la flota del jugador 1.
	liberar_flota(player2); // Liberar memoria de la flota del jugador 2.

    printf("Gracias por jugar a Batalla Naval.\n");
    printf("Presione enter para regresar al menu principal.\n");
    clearInputBuffer(); // Limpiar el buffer de entrada antes de esperar la tecla.
    clear_screen(); // Limpiar la pantalla al finalizar la partida.
}

void menu_por_turno(struct player *player, struct player *enemy) {
    char opc = ' ';
    bool loop = true;
    do {
    currentPlayerWarScreen(player, enemy);
    printf("\n");
        // Menu de opciones por turno.
    printf("Elija la opcion que desea realizar:\n");
    printf("[A]: Reporte de barcos enemigos\n");
    printf("[B]: Reporte de flota\n");
    printf("[C]: Sacar carta\n");

    scanf(" %c", &opc);
    clearInputBuffer(); // Limpiar el buffer de entrada antes de procesar la opción.
    opc = toupper(opc); // Convertir a mayúscula para evitar problemas de comparación.
        switch(opc) {
            case 'A':
                clear_screen(); //limpiar pantalla al salir del menu.
                printEnemyFleetReport(player, enemy);
                break;
            case 'B':
                clear_screen(); //limpiar pantalla al salir del menu.
                printFleetReport(player, enemy);
                break;
            case 'C':
                clear_screen(); //limpiar pantalla al salir del menu.
                sacar_carta(player, enemy);
                loop = false;
                break;
            default:
                color_txt(ERROR_COLOR);
                printf("¡Tecla invalida!\n");
                color_txt(DEFAULT_COLOR);
        }
        clear_screen(); // Limpiar la pantalla después de cada opción.
    }while(loop);
}

void sacar_carta(struct player *player, struct player *enemy) {
    int carta_id;
    
    if (player->salvo_mode) player->cards[8].peso = 0; // Si está en modo Salvo, se establece peso = 0 para que la carta 8 no pueda ser seleccionada.
    else player->cards[8].peso = 1; // Si no está en modo Salvo, establecer peso = 1 permite que la carta 8 pueda salir.
    
    carta_id = pickRandCard(player);
    
    player->prevCard = carta_id; // Guardar el ID de la carta utilizada

    currentPlayerWarScreen(player, enemy); // Mostrar el tablero de guerra si la carta no es la 11
    
    printCardInfo(&player->cards[carta_id]);

    switch (carta_id) {
    case 0:
        funcion_carta_0(player, enemy); // Carta 0: Dispara 1 tiro
        break;
    case 1:
        funcion_carta_1(player, enemy); // Carta 1: Dispara 2 tiros
        break;
    case 2:
        funcion_carta_2(player, enemy); // Carta 2: Dispara 3 tiros
        break;
    case 3:
        funcion_carta_3(player, enemy); // Carta 3: Bombardeo en fila
        break;
    case 4: 
        funcion_carta_4(player, enemy); // Carta 4: Bombardeo en columna
        break;
    case 5:
        funcion_carta_5(player, enemy); // Carta 5: Revela una celda enemiga
        break;
    case 6: 
        funcion_carta_6(player, enemy); // Carta 6: Revela una fila enemiga
        break;
    case 7:
        funcion_carta_7(player, enemy); // Carta 7: Revela una columna enemiga
        break;
    case 8:
        funcion_carta_8(player, enemy); // Carta 8: Activa el modo Salvo
        break;
    case 9:
        funcion_carta_9(player, enemy); // Carta 9: Activa la torre de ventaja al acumular 4 torres
        break;
    case 10:
        funcion_carta_10(player, enemy); // Carta 10: Dipara 1 tiro y toma otra carta
        break;
    }
}