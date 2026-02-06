#ifndef BS_BOARD_H
#define BS_BOARD_H

#include "bs_lang.h"

#define BOARD_SIZE 10

// ================ ASCII VALUES ================
typedef enum {
    PRINT_STER = 207, // '¤'
    PRINT_BODY = 254, // '■'
    PRINT_WATER = 126, // '~'
    PRINT_FAILED_SHOT = 158 // '×'
} BoardPrints;

typedef struct Board {
    CellType type[BOARD_SIZE][BOARD_SIZE];
    CellState state[BOARD_SIZE][BOARD_SIZE];
    int ship_ids[BOARD_SIZE][BOARD_SIZE];

    CellType (*get_type)(struct Board *self, int row, int col);
    CellState (*get_state)(struct Board *self, int row, int col);
    int (*get_ship_id)(struct Board *self, int row, int col);

    void (*set_type)(struct Board *self, int row, int col, CellType value);
    void (*set_state)(struct Board *self, int row, int col, CellState value);
    void (*set_ship_id)(struct Board *self, int row, int col, int ship_id);

    void (*init)(struct Board *self);
} Board;

// Constructor - única función pública
void Board_create(Board *board);

#endif // BS_BOARD_H