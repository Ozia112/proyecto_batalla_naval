#ifndef BS_SHIP_H
#define BS_SHIP_H

#include "bs_lang.h"

typedef enum {
    DIR_NORTH = 'N',
    DIR_SOUTH = 'S',
    DIR_EAST = 'E',
    DIR_WEST = 'W',
    DIR_UNDEFINED = 'U'
} Direction;

typedef enum {
    CELL_ROW = 0,
    CELL_COL = 1,
    CELL_TYPE = 2,
    CELL_STATUS = 3
} CellIndex;

typedef struct Ship {
    int id;
    char* name;
    int size;
    Direction direction; // 'E', 'W', 'N', 'S', 'U' for undefined
    int start_row;
    int start_col;
    int end_row;
    int end_col;
    int **cell_status; // [size][4] -> {row, col, type, status}
    bool is_placed;
    bool is_sunk;

    // Getters
    int (*get_id)(struct Ship *self);
    const char* (*get_name)(struct Ship *self);
    int (*get_size)(struct Ship *self);
    Direction (*get_direction)(struct Ship *self);
    int (*get_start_row)(struct Ship *self);
    int (*get_start_col)(struct Ship *self);
    int (*get_end_row)(struct Ship *self);
    int (*get_end_col)(struct Ship *self);
    int (*get_cell_row)(struct Ship *self, int index);
    int (*get_cell_col)(struct Ship *self, int index);
    int (*get_cell_index)(struct Ship *self, int row, int col);
    CellType (*get_cell_type)(struct Ship *self, int index);
    CellState (*get_cell_status)(struct Ship *self, int index);
    bool (*get_is_placed)(struct Ship *self);
    bool (*get_is_sunk)(struct Ship *self);

    // Setters
    void (*set_id)(struct Ship *self, int new_id);
    void (*set_name)(struct Ship *self, const char *new_name);
    void (*set_size)(struct Ship *self, int new_size);
    void (*set_direction)(struct Ship *self, Direction direction);
    void (*set_start_row)(struct Ship *self, int row);
    void (*set_start_col)(struct Ship *self, int col);
    void (*set_end_row)(struct Ship *self, int row);
    void (*set_end_col)(struct Ship *self, int col);
    void (*set_cell_coords)(struct Ship *self, int index, int row, int col);
    void (*set_cell_type)(struct Ship *self, int index, CellType type);
    void (*set_cell_state)(struct Ship *self, int index, CellState state);
    void (*set_is_placed)(struct Ship *self, bool placed);
    void (*set_is_sunk)(struct Ship *self, bool sunk);

    // Lifecycle
    void (*init)(struct Ship *self, int id, const char *name, int size);

} Ship;

// Constructor - única función pública
Ship* bs_Ship_create(void);
void bs_Ship_destroy(Ship *ship);

#endif // BS_SHIP_H