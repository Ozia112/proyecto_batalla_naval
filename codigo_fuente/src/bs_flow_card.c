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
        enter_continuar_cls();
        end_card_flow(player, enemy);
        if(enemy->remain_ship_cells == 0) {
            printf("¡%s ha hundido todos los barcos enemigos!\n", player->player_name);
            break; // Terminar el flujo si se hunden todos los barcos enemigos.
        }
         
    }
}

static void end_card_flow(struct player *player, struct player *enemy) {
    // Mostrar el tablero de guerra después de usar la carta.
    currentPlayerWarScreen(player, enemy);
    printf("Tablero de guerra actualizado:\n");
    if (player->prevCard == 3 || player->prevCard == 4
        || player->prevCard == 6 || player->prevCard == 7) {
        printBombORCheckSucces(player); // Mostrar el resultado del bombardeo.
    } else {
        printPrevHitCoord(player); // Mostrar el último disparo exitoso.
    }
    pause_timer(1);
    enter_continuar_cls();
}

void funcion_carta_0(struct player *player, struct player *enemy) {
    int shoots = player->salvo_mode 
    ? (NUM_SHIPS - enemy->sunken_ships) : 1;

    shot_flow(player, enemy, shoots);
}

void funcion_carta_1(struct player *player, struct player *enemy) {
    shot_flow(player, enemy, 2);
}

void funcion_carta_2(struct player *player, struct player *enemy) {
    shot_flow(player, enemy, 3);
}

void funcion_carta_3(struct player *player, struct player *enemy) {
    if (player->ships[1].is_alive) { // Barco de 4 casillas
        printf("Su barco de %d casillas logro activar su habilidad especial.\n", player->ships[1].ship_size);
        captrRowORColumn(player, enemy);
    } else {
        printf("No hemos podido activar la carta. El barco de %d casillas no esta a flote.\n", player->ships[1].ship_size);
        printf("Solo pudimos activar la carta de 1 disparo.\n");
        enter_continuar_cls();
        currentPlayerWarScreen(player, enemy);
        shot_flow(player, enemy, 1); // Dispara una vez si el barco no está a flote
    }
    enter_continuar_cls();
    end_card_flow(player, enemy);
}

void funcion_carta_4(struct player *player, struct player *enemy) {
    if (player->ships[0].is_alive) { // Barco de 5 casillas
        printf("Su barco de %d casillas logro activar su habilidad especial.\n", player->ships[0].ship_size);
        captrRowORColumn(player, enemy);
    } else {
        printf("No hemos podido activar la carta. El barco de %d casillas no esta a flote.\n", player->ships[0].ship_size);
        printf("Solo pudimos activar la carta de 1 disparo.\n");
        enter_continuar_cls();
        currentPlayerWarScreen(player, enemy);
        shot_flow(player, enemy, 1); // Dispara una vez si el barco no está a flote
    }
    enter_continuar_cls();
    end_card_flow(player, enemy);
}

void funcion_carta_5(struct player *player, struct player *enemy) {
    printf("Inteligencia a decifrado con exito la ubicacion de una casilla enemiga.\n");
    revela(player, enemy);
    enter_continuar_cls();
    end_card_flow(player, enemy);
}

void funcion_carta_6(struct player *player, struct player *enemy) {
    printf("Hemos logrado activar el radar de filas.\n");
    captrRowORColumn(player, enemy);
    enter_continuar_cls();
    end_card_flow(player, enemy);
}

void funcion_carta_7(struct player *player, struct player *enemy) {
    printf("Hemos logrado activar el radar de columnas.\n");
    captrRowORColumn(player, enemy);
    enter_continuar_cls();
    end_card_flow(player, enemy);
}

void funcion_carta_8(struct player *player, struct player *enemy) {
    activar_salvo(player);
    printf("\nModo Salvo activado\n");
    printf("Durante este turno NO disparas. En tu proximo turno, podras disparar multiples veces.\n");
    pause_timer(1);
    enter_continuar_cls();

    currentPlayerWarScreen(player, enemy);
    printf("Modo");
    printf_color(SUCCESS_COLOR," Salvo");
    printf(" actualizado:\n");
    pause_timer(1.3);
    enter_continuar_cls();
}

void funcion_carta_9(struct player *player, struct player *enemy) {
    if(player->acc_towers < 4 ) {
            player->acc_towers++;
            printf("Has acumulado una torre de ventaja. Acumula 4 para hundir instantaneamente los barcos enemigos.\n");
            // Reducir el peso de la carta Torre de ventaja
            if (player->cards[9].peso > 0) player->cards[9].peso--;
    } else torre_ventaja(player);
    printf("En este turno no disparas.\n");
    enter_continuar_cls();

    currentPlayerWarScreen(player, enemy);
    printf("Torre de ");
    printf_color(SUCCESS_COLOR, "ventaja");
    printf(" actualizada:\n");
    pause_timer(.8);
    enter_continuar_cls();
}

void funcion_carta_10(struct player *player, struct player *enemy) {
    shot_flow(player, enemy, 1); // Dispara una vez con la carta 10
    printf("Presiona");
    printf_color(INFO_COLOR, " ENTER");
    printf(" para sacar otra carta.\n");
    clearInputBuffer(); // Limpiar el buffer de entrada para evitar problemas con la entrada del usuario.
    clear_screen(); // Limpiar la pantalla antes de sacar otra carta.
    sacar_carta(player, enemy);
}