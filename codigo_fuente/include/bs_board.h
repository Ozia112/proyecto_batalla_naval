#ifndef BS_BOARD_H
#define BS_BOARD_H

#include "master.h"

// administrar colocacion y movimiento de barcos

bool in_range(int cc_row, int cc_column);

bool first_coord_is_free(struct player *player, int index, int rowIn, int columnIn);

bool is_straight(struct player *player, int index, int rowIn, int rowEnd, int columnIn, int columnEnd);

bool dim_match_index(struct player *player, int index, int rowIn, int rowEnd, int columnIn, int columnEnd);

bool end_cell_is_water(struct player *player, int index, int rowIn, int rowEnd, int columnIn, int columnEnd);

void get_remain_fleet_cells(struct player *player);

// Administrar ataques a barcos

bool attack_in_range(struct player *player, int cc_row, int cc_column);

int getEnemyCellsInRow(struct player *player, struct player *enemy, int cc_row);

int getEnemyCellsInCol(struct player *player, struct player *enemy, int cc_columna);

bool cell_is_valid_to_shot(struct player *player, struct player *enemy);


#endif