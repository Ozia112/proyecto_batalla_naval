#include "model/bs_Fleet.h"

// Declaracion de prototipos de funciones privadas
static Ship* Fleet_get_ship(Fleet *self, int index);
static int Fleet_get_ships_placed(Fleet *self);
static int Fleet_get_remaining_ships(Fleet *self);
static int Fleet_get_remaining_cells(Fleet *self);

static void Fleet_set_ship(Fleet *self, int index, Ship *ship);
static void Fleet_set_ships_placed(Fleet *self, int placed);
static void Fleet_increment_ships_placed(Fleet *self);
static void Fleet_set_remaining_ships(Fleet *self, int ships);
static void Fleet_decrement_remaining_ships(Fleet *self);
static void Fleet_set_remaining_cells(Fleet *self, int cells);
static void Fleet_decrement_remaining_cells(Fleet *self);

static void Fleet_init(Fleet *self);

// Funciones externas
Fleet* bs_Fleet_create(void) {
    Fleet *fleet = NULL;
    try {
        fleet = (Fleet *)malloc(sizeof(Fleet));
        if (fleet == NULL) {
            throw(BS_ERROR_MEMORY_ALLOCATION, "No se pudo asignar memoria para la flota");
        }

        for (int i = 0; i < NUM_SHIPS; i++) {
            fleet->ships[i] = NULL;
        }

        fleet->ships_placed = 0;
        fleet->remaining_ships = 0;
        fleet->remaining_cells = 0;

        // Asignar punteros a funciones
        fleet->get_ship = Fleet_get_ship;
        fleet->get_ships_placed = Fleet_get_ships_placed;
        fleet->get_remaining_ships = Fleet_get_remaining_ships;
        fleet->get_remaining_cells = Fleet_get_remaining_cells;

        fleet->set_ship = Fleet_set_ship;
        fleet->set_ships_placed = Fleet_set_ships_placed;
        fleet->increment_ships_placed = Fleet_increment_ships_placed;
        fleet->set_remaining_ships = Fleet_set_remaining_ships;
        fleet->decrement_remaining_ships = Fleet_decrement_remaining_ships;
        fleet->set_remaining_cells = Fleet_set_remaining_cells;
        fleet->decrement_remaining_cells = Fleet_decrement_remaining_cells;

        fleet->init = Fleet_init;

        /**
         * TODO: Implementar lectura desde archivo donde se llamara init
         */
        //fleet->init(fleet);

        return fleet;
    } catch_any {
        if (fleet != NULL) {
            bs_Fleet_destroy(fleet);
        }
        return NULL;
    }
}

void bs_Fleet_destroy(Fleet *self) {
    if (self == NULL) return;

    for (int i = 0; i < NUM_SHIPS; i++) {
        if (self->ships[i] != NULL) {
            bs_Ship_destroy(self->ships[i]);
            self->ships[i] = NULL;
        }
    }
    free(self);
}

// Implementación de métodos
static Ship* Fleet_get_ship(Fleet *self, int index) {
    if (index < 0 || index >= NUM_SHIPS) {
        return NULL;
    }
    return self->ships[index];
}

static int Fleet_get_ships_placed(Fleet *self) {
    return self->ships_placed;
}

static int Fleet_get_remaining_ships(Fleet *self) {
    return self->remaining_ships;
}

static int Fleet_get_remaining_cells(Fleet *self) {
    return self->remaining_cells;
}

static void Fleet_set_ship(Fleet *self, int index, Ship *ship) {
    if (index < 0 || index >= NUM_SHIPS) {
        return;
    }

    if (self->ships[index] != NULL) {
        bs_Ship_destroy(self->ships[index]);
    }

    self->ships[index] = ship;
}

static void Fleet_set_ships_placed(Fleet *self, int placed) {
    self->ships_placed = placed;
}

static void Fleet_increment_ships_placed(Fleet *self) {
    self->ships_placed++;
}

static void Fleet_set_remaining_ships(Fleet *self, int ships) {
    self->remaining_ships = ships;
}

static void Fleet_decrement_remaining_ships(Fleet *self) {
    if (self->remaining_ships > 0) {
        self->remaining_ships--;
    }
}

static void Fleet_set_remaining_cells(Fleet *self, int cells) {
    self->remaining_cells = cells;
}

static void Fleet_decrement_remaining_cells(Fleet *self) {
    if (self->remaining_cells > 0) {
        self->remaining_cells--;
    }
}

static void Fleet_init(Fleet *self) {
    try {
        self->ships_placed = 0;
        self->remaining_ships = NUM_SHIPS;
        self->remaining_cells = 0;

        const char* ship_names[NUM_SHIPS] = {
            "Portaaviones",
            "Acorazado",
            "Crucero",
            "Destructor",
            "Fragata"
        };

        int ship_sizes[NUM_SHIPS] = {5, 4, 3, 3, 2};

        for (int i = 0; i < NUM_SHIPS; i++) {
            Ship* ship = bs_Ship_create();
            if (ship == NULL) {
                throw(BS_ERROR_MEMORY_ALLOCATION, "No se pudo crear el barco para la flota");
            }

            ship->init(ship, i, ship_names[i], ship_sizes[i]);
            self->set_ship(self, i, ship);
            self->remaining_cells += ship_sizes[i];
        }
    } catch_any {
        // error ya logeado
    }
}

static void Fleet_init_from_file(Fleet *self, const char *filename) {
    FILE* file = NULL;
    try {
        if (filename == NULL) {
            throw(BS_ERROR_INVALID_PARAMETER, "El nombre del archivo no puede ser NULL");
        }

        file = fopen(filename, "r");
        if (file == NULL) {
            throw(BS_ERROR_FILE_NOT_FOUND, "No se pudo abrir el archivo de configuración de la flota");
        }

        self->ships_placed = 0;
        self->remaining_ships = 0;
        self->remaining_cells = 0;

        char line[256];
        int ship_count = 0;

        while (fgets(line, sizeof(line), file) != NULL && ship_count < NUM_SHIPS) {
            if (line[0] == '#' || line[0] == '\n') {
                continue; // Saltar comentarios y líneas vacías
            }

            int id;
            char name[64];
            int size;

            if (sscanf(line, "%d,%63[^,],%d", &id, name, &size) == 3) {
                if (id < 0 || id >= NUM_SHIPS || size <= 0 || size > 5) {
                    printf("Advertencia: Datos inválidos para el barco en la línea: %s", line);
                    continue;
                }

                Ship* ship = bs_Ship_create();
                if (ship == NULL) {
                    throw(BS_ERROR_MEMORY_ALLOCATION, "No se pudo crear el barco para la flota");
                }

                ship->init(ship, id, name, size);

                self->set_ship(self, id, ship);
                self->remaining_cells += size;
                ship_count++;
            } else {
                printf("Advertencia: Formato inválido en la línea: %s", line);
            }
        }
        self->remaining_ships = ship_count;

        fclose(file);
        file = NULL;

        if (ship_count < NUM_SHIPS) {
            printf("Advertencia: Solo se cargaron %d de %d barcos\n", ship_count, NUM_SHIPS);
        }
    } catch_any {
        if (file != NULL) {
            fclose(file);
        }
    }
} 