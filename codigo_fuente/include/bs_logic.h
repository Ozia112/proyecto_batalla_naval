#ifndef BS_LOGIC_H
#define BS_LOGIC_H

#include "master.h" // Libreria con todas las cabeceras del proyecto.

// Prototipos de funciones
void init_struct_player(struct player *player);

bool coords_validation(struct player *player, int index, int rowIn, int rowEnd, int columnIn, int columnEnd);

void set_initial_cell(struct player *player, int index, int rowIn, int columnIn);

void set_ship(struct player *player, int index, int rowIn, int rowEnd, int columnIn, int columnEnd);

void firstTurnRand(struct player *player1, struct player *player2);

int pickRandCard(struct player *player);

void set_original_card_weights(struct player *player);

void get_original_card_weights(struct player *player);

int calcTotalWeight(struct player *player);

bool parsing_coords(const char *input, int *row, int *col);
#endif // BS_LOGIC_H