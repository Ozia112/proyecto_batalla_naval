#ifndef BS_FLOW_CARD_H
#define BS_FLOW_CARD_H

#include "master.h"

// Prototipos de funciones
static void shot_flow(struct player *player, struct player *enemy, int shoots);

static int search_index_ship(struct player *player, struct player *enemy);

static void end_card_flow(struct player *player, struct player *enemy);

void funcion_carta_0(struct player *player, struct player *enemy);

void funcion_carta_1(struct player *player, struct player *enemy);

void funcion_carta_2(struct player *player, struct player *enemy);

void funcion_carta_3(struct player *player, struct player *enemy);

void funcion_carta_4(struct player *player, struct player *enemy);

void funcion_carta_5(struct player *player, struct player *enemy);

void funcion_carta_6(struct player *player, struct player *enemy);

void funcion_carta_7(struct player *player, struct player *enemy);

void funcion_carta_8(struct player *player, struct player *enemy);

void funcion_carta_9(struct player *player, struct player *enemy);

void funcion_carta_10(struct player *player, struct player *enemy);

#endif // BS_FLOW_CARD_H