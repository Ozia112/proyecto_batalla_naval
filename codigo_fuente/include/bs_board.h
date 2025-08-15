#ifndef BS_BOARD_H
#define BS_BOARD_H

#include "master.h"

// administrar colocacion y movimiento de barcos

bool in_range(int cc_Row, int cc_Col);

bool first_coord_is_free(struct player *player, int ship_idx, int cc_RowIn, int cc_ColIn);

void change_to_water(struct player *player, int ship_idx, int cc_RowIn, int cc_ColIn);

bool is_straight(struct player *player, int ship_idx, int cc_RowIn, int cc_RowEnd, int cc_ColIn, int cc_ColEnd);

bool dim_match_index(struct player *player, int ship_idx, int cc_RowIn, int cc_RowEnd, int cc_ColIn, int cc_ColEnd);

bool end_cell_is_water(struct player *player, int ship_idx, int cc_RowIn, int cc_RowEnd, int cc_ColIn, int cc_ColEnd);

void get_remain_fleet_cells(struct player *player);

// Administrar ataques a barcos

void changeToDamaged(struct player *enemy, int cc_Row, int cc_Col, int ship_idx, int ship_cell);

bool attack_in_range(struct player *player, int cc_Row, int cc_Col);

int getEnemyCellsInRow(struct player *player, struct player *enemy, int cc_Row);

int getEnemyCellsInCol(struct player *player, struct player *enemy, int cc_Col);

bool cell_is_valid_to_shot(struct player *player, struct player *enemy);

#endif