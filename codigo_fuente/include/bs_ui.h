#ifndef BS_UI_H
#define BS_UI_H

#include "master.h" // Libreria con todas las cabeceras del proyecto.

// Function prototypes

void tittle_screen();

void main_menu(struct player *player1, struct player *player2);

void rules();

void get_name(struct player *player);

void set_ships_screen(struct player *player, int index, struct player *enemy);

void set_ships_flow(struct player *player, struct player *enemy);

void printCurrentPlayerBoard(struct player *player, struct player *enemy);

void getSetShipsCoords(struct player *player, int index, struct player *enemy);

void currentPlayerWarScreen(struct player *player, struct player *enemy);

void printCurrentPlayerWarBoard(struct player *player, struct player *enemy);

void printEnemyFleetReport(struct player *player, struct player *enemy);

void printFleetReport(struct player *player, struct player *enemy);

void currentPlayerScreen(struct player *player, struct player *enemy);

void printCardInfo(struct card *cards);

void captureCoordToShot(struct player *player, struct player *enemy);

void printPrevHitCoord(struct player *player);

void printBombORCheckSucces(struct player *player);

void captrRowORColumn(struct player *player, struct player *enemy);
#endif // BS_UI_H