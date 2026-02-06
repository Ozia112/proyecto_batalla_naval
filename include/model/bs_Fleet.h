#ifndef BS_FLEET_H
#define BS_FLEET_H

#include "bs_lang.h"
#include "bs_Ship.h"

#define NUM_SHIPS 5

typedef struct Fleet {
    Ship* ships[NUM_SHIPS];
    int ships_placed;
    int remaining_ships;
    int remaining_cells;

    // Getters
    Ship* (*get_ship)(struct Fleet *self, int index);
    int (*get_ships_placed)(struct Fleet *self);
    int (*get_remaining_ships)(struct Fleet *self);
    int (*get_remaining_cells)(struct Fleet *self);

    // Setters
    void (*set_ship)(struct Fleet *self, int index, Ship* ship);
    void (*set_ships_placed)(struct Fleet *self, int placed);
    void (*increment_ships_placed)(struct Fleet *self);
    void (*set_remaining_ships)(struct Fleet *self, int ships);
    void (*decrement_remaining_ships)(struct Fleet *self);
    void (*set_remaining_cells)(struct Fleet *self, int cells);
    void (*decrement_remaining_cells)(struct Fleet *self);
    

    // Lifecycle
    void (*init)(struct Fleet *self);
} Fleet;

// Constructor - única función pública
Fleet* bs_Fleet_create(void);
void bs_Fleet_destroy(Fleet *self); // Libera memoria
#endif // BS_FLEET_H