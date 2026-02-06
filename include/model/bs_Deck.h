#ifndef BS_DECK_H
#define BS_DECK_H
#include "bs_Card.h"
#include "bs_lang.h"

#define MAX_STACK_SIZE 28

typedef struct Deck {
    const char* name;
    Card* cards[MAX_STACK_SIZE];
    int top;

    // Métodos (estilo OOP)
    bool (*push)(struct Deck* self, Card* card);
    Card* (*pop)(struct Deck* self);
    Card* (*peek)(const struct Deck* self);
    bool (*is_empty)(const struct Deck* self);
    bool (*is_full)(const struct Deck* self);
    int (*get_size)(const struct Deck* self);
    void (*clear)(struct Deck* self);
    void (*shuffle)(struct Deck* self);
    void (*set_name)(struct Deck* self, const char* name);
    const char* (*get_name)(const struct Deck* self);
} Deck;

// Constructor y destructor
Deck* bs_Deck_create(const char* name);
void bs_Deck_destroy(Deck* deck);

#endif // BS_DECK_H