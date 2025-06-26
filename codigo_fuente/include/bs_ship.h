#ifndef BS_SHIP_H
#define BS_SHIP_H

#include "master.h" // Incluye todas las cabeceras del proyecto
bool isShipHere(struct player *player, int index, int ship_cell, int fila, int columna); // Verifica si una parte del barco está en la posición (x, y)

bool isEnemyShipHere(struct player *player, int index, int ship_cell, struct player *enemy);

bool cell_is_intact_ship_cell(struct player *enemy, int index, int ship_cell); // Verifica si una parte del barco está saludable

void instant_sunk(struct player *player, int index, struct player *enemy); // Hundir un barco enemigo con la mejora activada

void is_sunk(struct player *player, struct player *enemy, int index); // Verifica si un barco enemigo ha sido hundido

void liberar_status(struct ship *barco); // Libera la memoria del estado de un barco

void liberar_flota(struct player *player); // Libera la memoria de la flota de barcos del jugador

#endif