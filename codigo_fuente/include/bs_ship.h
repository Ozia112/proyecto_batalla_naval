#ifndef BS_SHIP_H
#define BS_SHIP_H

#include "master.h" // Incluye todas las cabeceras del proyecto
bool isShipHere(struct player *player, int index, int ship_cell, int fila, int columna); // Verifica si una parte del barco está en la posición (x, y)

bool isEnemyShipHere(struct player *player, int index, int ship_cell, struct player *enemy);

bool is_intact_cell(struct player *enemy, int cc_Row, int cc_Col); // Verifica si una parte del barco está saludable

bool is_unshooted_cell_ship(struct player *enemy, int index, int ship_cell); // Verifica si una parte del barco es candidata para revelar

bool is_sunk(struct player *player, struct player *enemy, int index); // Verifica si un barco enemigo ha sido hundido

void instant_sunk(struct player *player, int index, struct player *enemy); // Hundir un barco enemigo con la mejora activada

void liberar_status(struct ship *barco); // Libera la memoria del estado de un barco

void free_fleet_memory(struct player *player); // Libera la memoria de la flota de barcos del jugador

#endif