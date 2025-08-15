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
        printf_color(INFO_COLOR, DEFAULT,"\n\t\t=== Elige una opcion ===\n\n");
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
                game_in_progress = false; // Update state for new game
                break;
            case 'B':
                flow_resume_game(player1, player2);
                break;
            case 'C':
                clear_screen();
                return;
            default:
                invalid = true;
                printf_color(ERROR_COLOR, DEFAULT, "¡Tecla invalida!\n");
                pause_timer(1);
                clear_screen();
        }
        pause_timer(1); // Pause so the user can see the progress.
        printf_color(INFO_COLOR, DEFAULT, "Iniciando partida...\n");
        pause_timer(.8); // Pause so the user can see the start message.
        clear_screen(); // Clear the screen after each option.
        if (!invalid) break;
    } while(true);

    if(!game_in_progress) registro_jugadores(player1, player2); // Players registration flow.
    partida(player1, player2); // Start the game.
}

void flow_new_game(struct player *player1, struct player *player2) {
    delete_player_file(player1);
    delete_player_file(player2);
    printf_color(INFO_COLOR, DEFAULT, "Creando archivos de jugadores...\n");
    pause_timer(1);
    createPlayerFile(player1);
    createPlayerFile(player2);
    printf_color(INFO_COLOR, DEFAULT, "Cargando archivos en memoria...\n");
    pause_timer(1);
    getPlayerState(player1);
    getPlayerState(player2);
}

void flow_resume_game(struct player *player1, struct player *player2) {
    getPlayerState(player1);
    getPlayerState(player2);
}

void registro_jugadores(struct player *player1, struct player *player2) {
    
    get_name(player1, player1->player_index);
    get_name(player2, player2->player_index);
    
    printf("Jugadores:\n");
    printf("Jugador 1:"); printf_color(INFO_COLOR, DEFAULT, " %s\n", player1->player_name);
    printf("Jugador 2:"); printf_color(INFO_COLOR, DEFAULT, " %s\n", player2->player_name);
    enter_to_message("continuar", true); // Pause before continuing.

    set_ships_flow(player1, player2); // Place ships for player 1.

    printf("Ahora es turno de"); printf_color(INFO_COLOR, DEFAULT, " %s\n", player2->player_name);
    enter_to_message("continuar", true); // Pause before continuing.

    set_ships_flow(player2, player1); // Place ships for player 2.

    // Randomly choose who starts
    firstTurnRand(player1, player2);

    save_player_state(player1); // Save player 1 state to JSON file.
    save_player_state(player2); // Save player 2 state to JSON file.

    pause_timer(1);
    enter_to_message("continuar", true);
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
        player1->acc_turns < player2->acc_turns ? // If player 1 has fewer accumulated turns, it's their turn.
            (jugador_actual = player1, jugador_enemigo = player2) :
            (jugador_actual = player2, jugador_enemigo = player1);
    }

    // Main game loop
    do {
        if (jugador_actual->acc_turns == 0) set_original_card_weights(jugador_actual);
        
        save_player_state(player1);
        save_player_state(player2);
        menu_por_turno(jugador_actual, jugador_enemigo);

        printf("Tu turno ha terminado.\n");
        enter_to_message("continuar", true);
        
        if (jugador_enemigo->remain_ship_cells == 0) break; // If the enemy player has no remaining ship cells, end the game.

        printf("Ahora es turno de %s\n", jugador_enemigo->player_name);
        enter_to_message("continuar", true);

        jugador_actual->acc_turns++;
        reset_buffer_register(jugador_actual); // Reset current player's buffer registers
        save_player_state(jugador_actual);
        save_player_state(jugador_enemigo);
        // Alternate players (swap current and enemy player)
        temp = jugador_actual;
        jugador_actual = jugador_enemigo;
        jugador_enemigo = temp;

    } while(player1->remain_ship_cells > 0 && player2->remain_ship_cells > 0);

    // Victory condition check
    printf_color(SUCCESS_COLOR, DEFAULT, "!");
    if (player1->enemy_hit_parts >= VICTORY_COND) {
        printf_color(INFO_COLOR, DEFAULT, "%s ", player1->player_name);
    } else if (player2->enemy_hit_parts >= VICTORY_COND) {
        printf_color(INFO_COLOR, DEFAULT, "%s ", player2->player_name);
    }
    printf_color(SUCCESS_COLOR, DEFAULT, "ha ganado la partida!\n");

    free_fleet_memory(player1);
	free_fleet_memory(player2);

    delete_player_file(player1);
    delete_player_file(player2);

    printf("Gracias por jugar a Batalla Naval.\n");
    printf("Presione enter para regresar al menu principal.\n");
    clearInputBuffer();
    clear_screen();
}

void menu_por_turno(struct player *player, struct player *enemy) {
    player->cards_total_weight = calcTotalWeight(player);
    char opc = ' ';
    bool loop = true;
    do {
    currentPlayerWarScreen(player, enemy);
    printf("\n");
    // Turn menu options
    deBugPrint(0, player->cards[0].peso, 0, EMPTY, EMPTY_STR);
    printf("\n");
    deBugPrint(1, player->cards[9].peso, 0, EMPTY, EMPTY_STR);
    printf("\n");
    printf("Elija la opcion que desea realizar:\n");
    printf("[A]: Reporte de barcos enemigos\n");
    printf("[B]: Reporte de flota\n");
    printf("[C]: Sacar carta\n");

    scanf(" %c", &opc);
    clearInputBuffer();
    opc = toupper(opc);
        switch(opc) {
            case 'A':
                clear_screen();
                printEnemyFleetReport(player, enemy);
                break;
            case 'B':
                clear_screen();
                printFleetReport(player, enemy);
                break;
            case 'C':
                clear_screen();
                sacar_carta(player, enemy);
                loop = false;
                break;
            default:
                printf_color(ERROR_COLOR, DEFAULT, "¡Tecla invalida!\n");
        }
        clear_screen();
    }while(loop);
    save_player_state(player);
    save_player_state(enemy);
}

void sacar_carta(struct player *player, struct player *enemy) {
    int carta_id;
    
    if (player->salvo_loaded && !player->salvo_mode) { // If the player has loaded salvo mode but is not in salvo mode
        player->cards[8].peso = 1; // Set the salvo card weight to 1
    } else {
        player->cards[8].peso = 0; // Otherwise, set it to 0
    }

    carta_id = pickRandCard(player);
    
    player->prevCard = carta_id; // Save the ID of the card used in this turn
    
    if (player->cards_total_weight > 0) player->cards[carta_id].peso--; // Decrease the weight of the card used

    currentPlayerWarScreen(player, enemy);
    
    printCardInfo(&player->cards[carta_id]); 

    switch (carta_id) {
    case 0:
        funcion_carta_0(player, enemy);
        break;
    case 1:
        funcion_carta_1(player, enemy);
        break;
    case 2:
        funcion_carta_2(player, enemy);
        break;
    case 3:
        funcion_carta_3(player, enemy);
        break;
    case 4: 
        funcion_carta_4(player, enemy);
        break;
    case 5:
        funcion_carta_5(player, enemy);
        break;
    case 6: 
        funcion_carta_6(player, enemy);
        break;
    case 7:
        funcion_carta_7(player, enemy);
        break;
    case 8:
        funcion_carta_8(player, enemy);
        break;
    case 9:
        funcion_carta_9(player, enemy);
        break;
    case 10:
        funcion_carta_10(player, enemy);
        break;
    }
}