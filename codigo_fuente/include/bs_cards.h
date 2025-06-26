#ifndef BS_CARDS_H
#define BS_CARDS_H

#include "master.h" // Libreria con todas las cabeceras del proyecto.

void shoot_func(struct player *player, struct player *enemy);

void bomb_row(struct player *player, struct player *enemy);

void bomb_col(struct player *player, struct player *enemy);

void revela(struct player *player, struct player *enemy);

void activateCheckInRow(struct player *player, struct player *enemy);

void activateCheckInCol(struct player *player, struct player *enemy);

void activar_salvo(struct player *player);

void desactivar_salvo(struct player *player);

void torre_ventaja(struct player *player);

void mover_barco_adelante(struct player *player, int id_barco);

#endif // BS_EFECTOS_CARTAS_H