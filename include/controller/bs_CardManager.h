#ifndef BS_CARD_MANAGER_H
#define BS_CARD_MANAGER_H

#include "model/bs_Card.h"
#include "model/bs_Deck.h"
#include "bs_lang.h"

// Asumiendo que el total de cartas únicas es igual al tamaño máximo del mazo
#define TOTAL_CARDS_IN_GAME MAX_STACK_SIZE 

typedef struct CardManager {
    Card* card_pool[TOTAL_CARDS_IN_GAME];  // Pool de 28 cartas únicas
    int total_cards;
    
    // Métodos
    bool (*load_cards_from_database)(struct CardManager* self);
    bool (*load_cards_from_file)(struct CardManager* self, const char* filename);
    void (*initialize_deck)(struct CardManager* self, Deck* deck);
    bool (*transfer_card)(struct CardManager* self, Deck* source, Deck* target);
    Card* (*find_card_by_id)(struct CardManager* self, int card_id);
    void (*print_card_info)(const struct CardManager* self, const Card* card);
} CardManager;

// Constructor y destructor
CardManager* bs_CardManager_create(void);
void bs_CardManager_destroy(CardManager* manager);

#endif // BS_CARD_MANAGER_H