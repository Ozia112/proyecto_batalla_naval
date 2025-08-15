#ifndef BS_CARDS_H
#define BS_CARDS_H

#include "master.h" // Libreria con todas las cabeceras del proyecto.

void shoot_func(struct player *player, struct player *enemy);

void changeToDamaged(struct player *enemy, int cc_Row, int cc_Col, int ship_idx, int ship_cell);

void bomb_row(struct player *player, struct player *enemy);

void bomb_col(struct player *player, struct player *enemy);

void reveal(struct player *player, struct player *enemy);

void activateCheckInRow(struct player *player, struct player *enemy);

void activateCheckInCol(struct player *player, struct player *enemy);

void activar_salvo(struct player *player);

void desactivar_salvo(struct player *player);

void advantage_tower(struct player *player);

void mover_barco_adelante(struct player *player, int id_barco);

#endif // BS_EFECTOS_CARTAS_H