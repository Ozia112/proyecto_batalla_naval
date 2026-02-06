#include "model/bs_Ship.h"

// Declaracion de prototipos de funciones internas
// Getters
static int ship_get_id(Ship *self);
static const char* ship_get_name(Ship *self);
static int ship_get_size(Ship *self);
static Direction ship_get_direction(Ship *self);
static int ship_get_start_row(Ship *self);
static int ship_get_start_col(Ship *self);
static int ship_get_end_row(Ship *self);
static int ship_get_end_col(Ship *self);
static int ship_get_cell_row(Ship *self, int index);
static int ship_get_cell_col(Ship *self, int index);
static int ship_get_cell_index(Ship *self, int row, int col);
static CellType ship_get_cell_type(Ship *self, int index);
static CellState ship_get_cell_status(Ship *self, int index);
static bool ship_get_is_placed(Ship *self);
static bool ship_get_is_sunk(Ship *self);

// Setters
static void ship_set_id(Ship *self, int new_id);
static void ship_set_name(Ship *self, const char *new_name);
static void ship_set_size(Ship *self, int new_size);
static void ship_set_direction(Ship *self, Direction direction);
static void ship_set_start_row(Ship *self, int row);
static void ship_set_start_col(Ship *self, int col);
static void ship_set_end_row(Ship *self, int row);
static void ship_set_end_col(Ship *self, int col);
static void ship_set_cell_coords(Ship *self, int index, int row, int col);
static void ship_set_cell_type(Ship *self, int index, CellType type);
static void ship_set_cell_state(Ship *self, int index, CellState state);
static void ship_set_is_placed(Ship *self, bool is_placed);
static void ship_set_is_sunk(Ship *self, bool is_sunk);

static void ship_init(Ship *ship, int id, const char *name, int size);

// Funciones externas
Ship* bs_Ship_create(void) {
    Ship* ship = NULL;

    try {
        ship = (Ship*)malloc(sizeof(Ship));
        if (ship == NULL) {
            throw(BS_ERROR_MEMORY_ALLOCATION, "No se pudo asignar memoria para el barco");
        }

        ship->id = 0;
        ship->name = NULL;
        ship->size = 0;
        ship->direction = DIR_UNDEFINED;
        ship->start_row = UNSET;
        ship->start_col = UNSET;
        ship->end_row = UNSET;
        ship->end_col = UNSET;
        ship->cell_status = NULL;
        ship->is_placed = false;
        ship->is_sunk = false;

        // Inicializar punteros a métodos
        ship->get_id = ship_get_id;
        ship->get_name = ship_get_name;
        ship->get_size = ship_get_size;
        ship->get_direction = ship_get_direction;
        ship->get_start_row = ship_get_start_row;
        ship->get_start_col = ship_get_start_col;
        ship->get_end_row = ship_get_end_row;
        ship->get_end_col = ship_get_end_col;
        ship->get_cell_row = ship_get_cell_row;
        ship->get_cell_col = ship_get_cell_col;
        ship->get_cell_index = ship_get_cell_index;
        ship->get_cell_type = ship_get_cell_type;
        ship->get_cell_status = ship_get_cell_status;
        ship->get_is_placed = ship_get_is_placed;
        ship->get_is_sunk = ship_get_is_sunk;

        ship->set_id = ship_set_id;
        ship->set_name = ship_set_name;
        ship->set_size = ship_set_size;
        ship->set_direction = ship_set_direction;
        ship->set_start_row = ship_set_start_row;
        ship->set_start_col = ship_set_start_col;
        ship->set_end_row = ship_set_end_row;
        ship->set_end_col = ship_set_end_col;
        ship->set_cell_coords = ship_set_cell_coords;
        ship->set_cell_type = ship_set_cell_type;
        ship->set_cell_state = ship_set_cell_state;
        ship->set_is_placed = ship_set_is_placed;
        ship->set_is_sunk = ship_set_is_sunk;
        ship->init = ship_init;

        return ship;
    }
    catch_any {
        if (ship != NULL) {
            free(ship);
        }
        return NULL;
    }
}

void bs_Ship_destroy(Ship *ship) {
    if (ship == NULL) return;

    if (ship->name != NULL) {
        free(ship->name);
        ship->name = NULL;
    }

    if (ship->cell_status != NULL) {
        for (int i = 0; i < ship->size; i++) {
            if (ship->cell_status[i] != NULL) {
                free(ship->cell_status[i]);
                ship->cell_status[i] = NULL;
            }
        }
        free(ship->cell_status);
        ship->cell_status = NULL;
    }

    free(ship);
}

// Implementación de métodos
static int ship_get_id(Ship *self) {
    return self->id;
}

static const char* ship_get_name(Ship *self) {
    return self->name;
}

static int ship_get_size(Ship *self) {
    return self->size;
}

static Direction ship_get_direction(Ship *self) {
    return self->direction;
}

static int ship_get_start_row(Ship *self) {
    return self->start_row;
}

static int ship_get_start_col(Ship *self) {
    return self->start_col;
}

static int ship_get_end_row(Ship *self) {
    return self->end_row;
}

static int ship_get_end_col(Ship *self) {
    return self->end_col;
}

static int ship_get_cell_row(Ship *self, int index) {
    if (index < 0 || index >= self->size) {
        return UNSET;
    }
    return self->cell_status[index][CELL_ROW];
}

static int ship_get_cell_col(Ship *self, int index) {
    if (index < 0 || index >= self->size) {
        return UNSET;
    }
    return self->cell_status[index][CELL_COL];
}

static int ship_get_cell_index(Ship *self, int row, int col) {
    for (int i = 0; i < self->size; i++) {
        if (self->cell_status[i][CELL_ROW] == row && self->cell_status[i][CELL_COL] == col) {
            return i;
        }
    }
    return UNSET;
}

static CellType ship_get_cell_type(Ship *self, int index) {
    if (index < 0 || index >= self->size) {
        return TYPE_UNDEFINED;
    }
    return (CellType)self->cell_status[index][CELL_TYPE];
}

static CellState ship_get_cell_status(Ship *self, int index) {
    if (index < 0 || index >= self->size) {
        return CELL_INTACT;
    }
    return (CellState)self->cell_status[index][CELL_STATUS];
}

static bool ship_get_is_placed(Ship *self) {
    return self->is_placed;
}

static bool ship_get_is_sunk(Ship *self) {
    return self->is_sunk;
}

// Setters implementation
static void ship_set_id(Ship *self, int new_id) {
    self->id = new_id;
}

static void ship_set_name(Ship *self, const char *new_name) {
    if (new_name == NULL) return;

    if (self->name != NULL) {
        free(self->name);
    }

    self->name = bs_safe_strdup(new_name);
    if (self->name == NULL) {
        throw(BS_ERROR_MEMORY_ALLOCATION, "No se pudo asignar memoria para el nombre del barco");
    }
}

static void ship_set_size(Ship *self, int new_size) {
    self->size = new_size;
}

static void ship_set_direction(Ship *self, Direction direction) {
    self->direction = direction;
}

static void ship_set_start_row(Ship *self, int row) {
    self->start_row = row;
}

static void ship_set_start_col(Ship *self, int col) {
    self->start_col = col;
}

static void ship_set_end_row(Ship *self, int row) {
    self->end_row = row;
}

static void ship_set_end_col(Ship *self, int col) {
    self->end_col = col;
}

static void ship_set_cell_coords(Ship *self, int index, int row, int col) {
    if (index < 0 || index >= self->size) {
        return;
    }
    self->cell_status[index][CELL_ROW] = row;
    self->cell_status[index][CELL_COL] = col;
}

static void ship_set_cell_type(Ship *self, int index, CellType type) {
    if (index < 0 || index >= self->size) {
        return;
    }
    self->cell_status[index][CELL_TYPE] = (int)type;
}

static void ship_set_cell_state(Ship *self, int index, CellState state) {
    if (index < 0 || index >= self->size) {
        return;
    }
    self->cell_status[index][CELL_STATUS] = (int)state;
}

static void ship_set_is_placed(Ship *self, bool is_placed) {
    self->is_placed = is_placed;
}

static void ship_set_is_sunk(Ship *self, bool is_sunk) {
    self->is_sunk = is_sunk;
}

void ship_init(Ship *ship, int id, const char *name, int size) {
    try {
        if (ship == NULL || name == NULL || size <= 0) {
            throw(BS_ERROR_INVALID_PARAMETER, "Parámetros inválidos para inicializar el barco");
        }
        ship_set_id(ship, id);
        ship_set_name(ship, name);
        ship_set_size(ship, size);
        ship_set_direction(ship, DIR_UNDEFINED);
        ship_set_start_row(ship, UNSET);
        ship_set_start_col(ship, UNSET);
        ship_set_end_row(ship, UNSET);
        ship_set_end_col(ship, UNSET);
        ship_set_is_placed(ship, false);
        ship_set_is_sunk(ship, false);

        ship->cell_status = (int **)malloc(size * sizeof(int *));
        if (ship->cell_status == NULL) {
            throw(BS_ERROR_MEMORY_ALLOCATION, "No se pudo asignar memoria para el estado de las celdas del barco");
        }

        for (int i = 0; i < size; i++) {
            ship->cell_status[i] = (int *)malloc(4 * sizeof(int));
            if (ship->cell_status[i] == NULL) {
                for (int j = 0; j < i; j++) {
                    free(ship->cell_status[j]);
                }
                free(ship->cell_status);
                ship->cell_status = NULL;
                throw(BS_ERROR_MEMORY_ALLOCATION, "No se pudo asignar memoria para el estado de las celdas del barco");
            }
        }
        for (int i = 0; i < size; i++) {

            ship_set_cell_coords(ship, i, UNSET, UNSET);
            ship_set_cell_type(ship, i, TYPE_UNDEFINED);
            ship_set_cell_state(ship, i, CELL_INTACT);
        }
    }
    catch_any {
        // El error ya ha sido loggeado
    }
}

