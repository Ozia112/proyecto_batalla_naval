#ifndef BS_LANG_H
#define BS_LANG_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include "utils/bs_string_mngr.h"
#include "utils/bs_error_handler.h"
#include "utils/bs_console.h"

#define NONE 0
#define UNSET -1




// Types que necesitan estar en capas de bajo nivel (model) para evitar dependencias circulares

/**
 * ================ CELL TYPES ================
 * Tipos de celda en el tablero:
 * - WATER: Agua (sin barco)
 * - SHIP_STER: Proa o popa del barco
 * - SHIP_BODY: Cuerpo del barco
 */
typedef enum CellType {
    TYPE_UNDEFINED = -1,
    WATER = 0,
    SHIP_STER = 1,
    SHIP_BODY = 2
} CellType;

/**
 * ================ CELL STATES ================
 * Estados de una celda:
 * - CELL_INTACT: Celda sin daños
 * - CELL_DAMAGED: Celda dañada
 */
typedef enum CellState{
    CELL_INTACT = 0,
    CELL_DAMAGED = 1
} CellState;

#endif // BS_LANG_H