#ifndef BS_GAME_H
#define BS_GAME_H

#include "master.h" // Libreria con todas las cabeceras del proyecto.
void flow_game(struct player *player1, struct player *player2);

void flow_new_game(struct player *player1, struct player *player2);

void flow_resume_game(struct player *player1, struct player *player2);

void registro_jugadores(struct player *player1, struct player *player2);

void partida(struct player *player1, struct player *player2);

void menu_por_turno(struct player *player, struct player *enemy);

void sacar_carta(struct player *player, struct player *enemy);

#endif // BS_GAME_H