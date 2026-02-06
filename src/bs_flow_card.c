#include "bs_flow_card.h"

static void shot_flow(struct player *player, struct player *enemy, int shoots) {
    for (int i = 0; i < shoots; i++) {
        if (i == 0) {
            printf("Dispara una vez.\n");
        } else if (i < shoots - 1) {
            currentPlayerWarScreen(player, enemy);
            printf("Dispara de nuevo.\n");
        } else {
            currentPlayerWarScreen(player, enemy);
            printf("Dispara una vez mas.\n");
        }
        pause_timer(.5);
        captureCoordToShot(player, enemy);
        enter_to_message("continuar", true);
        end_card_flow(player, enemy);
        if(enemy->remain_ship_cells == 0) {
            printf("¡%s ha hundido todos los barcos enemigos!\n", player->player_name);
            break; // End the loop if all enemy ships are sunk.
        }
    }
}

static int search_index_ship(struct player *player, struct player *enemy) {
    int row, col;
    row = player->prevHitRow;
    col = player->prevHitCol;
    return enemy->board[row][col].ship_id; // Return the index of the ship that was hit.
}

static void end_card_flow(struct player *player, struct player *enemy) {
    // Show the war board after using the card.
    if(player->cards_total_weight ==  0) {
        printf_color(INFO_COLOR, DEFAULT, "Has acabado el mazo de tus cartas.\n Volveremos a revolver tu mazo.\n");
        get_original_card_weights(player); // When the cards run out, the weights are reset.
        enter_to_message("continuar", true); // Pause before continuing.
    }
    currentPlayerWarScreen(player, enemy);
    printf("Tablero de guerra actualizado:\n");
    if ((player->prevCard == 3 && player->ships[1].is_alive) || (player->prevCard == 4 && player->ships[0].is_alive)
        || player->prevCard == 6 || player->prevCard == 7) {
        printBombORCheckSucces(player); // Show the result of the bombing.
    } else if (player->prevCard != 8 && player->prevCard != 9){
        int index = search_index_ship(player, enemy);
        printShootreport(player, enemy, index); // Show the last successful shot.
    }
    if (player->prevCard == 8 || player->prevCard == 9) {
        printf("%s", player->prevCard == 8 ? "Modo " : "Torre de ");
        printf_color(SUCCESS_COLOR, DEFAULT, player->prevCard == 8 ? "Salvo" : "ventaja");
        printf(" actualizad%s\n",
            player->prevCard == 8 ? "o" : "a"); // Use 'o' for "Salvo" and 'a' for "ventaja".
    }
    pause_timer(1);
    enter_to_message("continuar", true);
}

void funcion_carta_0(struct player *player, struct player *enemy) {
    int shoots = player->salvo_mode ? (NUM_SHIPS - enemy->sunken_ships) : 1;

    shot_flow(player, enemy, shoots);
}

void funcion_carta_1(struct player *player, struct player *enemy) {
    shot_flow(player, enemy, 2);
}

void funcion_carta_2(struct player *player, struct player *enemy) {
    shot_flow(player, enemy, 3);
}

void funcion_carta_3(struct player *player, struct player *enemy) {
    if (player->ships[1].is_alive) { // 4 cells ship
        printf("Su barco de %d casillas logro activar su habilidad especial.\n", player->ships[1].ship_size);
        captrRowORColumn(player, enemy);
    } else {
        printf("No hemos podido activar la carta. El barco de %d casillas no esta a flote.\n", player->ships[1].ship_size);
        printf("Solo pudimos activar la carta de 1 disparo.\n");
        enter_to_message("continuar", true);
        shot_flow(player, enemy, 1); // Shoots once if the ship is not alive
    }
    enter_to_message("continuar", true);
    end_card_flow(player, enemy);
}

void funcion_carta_4(struct player *player, struct player *enemy) {
    if (player->ships[0].is_alive) { // 5 cells ship
        printf("Su barco de %d casillas logro activar su habilidad especial.\n", player->ships[0].ship_size);
        captrRowORColumn(player, enemy);
    } else {
        printf("No hemos podido activar la carta. El barco de %d casillas no esta a flote.\n", player->ships[0].ship_size);
        printf("Solo pudimos activar la carta de 1 disparo.\n");
        enter_to_message("continuar", true);
        shot_flow(player, enemy, 1); // Shoots once if the ship is not alive
    }
    enter_to_message("continuar", true);
    end_card_flow(player, enemy);
}

void funcion_carta_5(struct player *player, struct player *enemy) {
    reveal(player, enemy);
    int index = search_index_ship(player, enemy);
    clear_screen();
    currentPlayerWarScreen(player, enemy);
    printShootreport(player, enemy, index); // Show the coordinates of the successful shot
    enter_to_message("continuar", true);
    end_card_flow(player, enemy);
}

void funcion_carta_6(struct player *player, struct player *enemy) {
    printf("Hemos logrado activar el radar de filas.\n");
    captrRowORColumn(player, enemy);
    enter_to_message("continuar", true);
    end_card_flow(player, enemy);
}

void funcion_carta_7(struct player *player, struct player *enemy) {
    printf("Hemos logrado activar el radar de columnas.\n");
    captrRowORColumn(player, enemy);
    enter_to_message("continuar", true);
    end_card_flow(player, enemy);
}

void funcion_carta_8(struct player *player, struct player *enemy) {
    activar_salvo(player);
    printf("\nModo Salvo activado\n");
    printf("Durante este turno NO disparas. En tu proximo turno, podras disparar multiples veces.\n");
    pause_timer(1);
    enter_to_message("continuar", true);

    end_card_flow(player, enemy);
}

void funcion_carta_9(struct player *player, struct player *enemy) {
    advantage_tower(player);
    printf(
        player->cards[9].peso == 0
            ? "Has acumulado %d torres de ventaja. Todos tus disparos hunden instantaneamente los barcos enemigos.\n"
            : "Has acumulado una torre de ventaja. Acumula %d más para hundir instantaneamente los barcos enemigos.\n",
        player->cards[9].peso == 0 ? player->acc_towers : player->cards[9].peso
    );
    printf("En este turno no disparas.\n");
    enter_to_message("continuar", true);

    end_card_flow(player, enemy);

}

void funcion_carta_10(struct player *player, struct player *enemy) {
    shot_flow(player, enemy, 1); // Shots once, as the card allows to draw another card.
    printf("Presiona");
    printf_color(INFO_COLOR, DEFAULT, " ENTER");
    printf(" para sacar otra carta.\n");
    clearInputBuffer(); // Clear the input buffer to avoid issues with user input.
    clear_screen(); // Clear the screen before drawing another card.
    sacar_carta(player, enemy);
}