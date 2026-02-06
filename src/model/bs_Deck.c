#include "model/bs_Deck.h"

// Declaracion de prototipos de funciones internas
static bool deck_push(Deck* self, Card* card);
static Card* deck_pop(Deck* self);
static Card* deck_peek(const Deck* self);
static bool deck_is_empty(const Deck* self);
static bool deck_is_full(const Deck* self);
static int deck_get_size(const Deck* self);
static void deck_clear(Deck* self);
static void deck_shuffle(Deck* self);
static void deck_set_name(Deck* self, const char* name);
static const char* deck_get_name(const Deck* self);

// Funciones externas

Deck* bs_Deck_create(const char* name) {
    Deck* deck = NULL;

    try {
        deck = (Deck*)malloc(sizeof(Deck));
        if (deck == NULL) {
            throw(BS_ERROR_MEMORY_ALLOCATION, "No se pudo asignar memoria para el mazo");
        }

        deck->top = -1;
        memset(deck->cards, 0, sizeof(deck->cards));

        deck->push = deck_push;
        deck->pop = deck_pop;
        deck->peek = deck_peek;
        deck->is_empty = deck_is_empty;
        deck->is_full = deck_is_full;
        deck->get_size = deck_get_size;
        deck->clear = deck_clear;
        deck->shuffle = deck_shuffle;
        deck->set_name = deck_set_name;
        deck->get_name = deck_get_name;

        // Inicializar nombre usando bs_safe_strdup
        if (name != NULL) {
            deck->name = bs_safe_strdup(name);
            if (deck->name == NULL) {
                throw(BS_ERROR_MEMORY_ALLOCATION, "No se pudo asignar memoria para el nombre del mazo");
            }
        } else {
            deck->name = NULL;
        }

        return deck;
    }
    catch_any {
        if (deck != NULL) {
            if (deck->name != NULL) {
                free((void*)deck->name);
            }
            free(deck);
        }
        return NULL;
    }
}

/**
 * Nota: Este destructor no libera las cartas dentro del mazo, solo la estructura del mazo.
 * Al liberar deck se elimina de la memoria la referencia a las cartas, pero las cartas deben
 * ser gestionadas y liberadas por separado. Esto permite flexibilidad para compartir cartas
 * entre mazos o gestionar la memoria de las cartas de forma independiente.
 */
void bs_Deck_destroy(Deck* deck) {
    if (deck == NULL) return;
    if (deck->name != NULL) {
        free((void*)deck->name);
    }
    free(deck);
}

// Implementación de métodos

static bool deck_push(Deck* self, Card* card) {
    if (self == NULL || card == NULL) return false;

    if (deck_is_full(self)) {
        return false; // No se puede agregar más cartas, el mazo está lleno
    }

    self->top++;
    self->cards[self->top] = card; // Agrega el puntero a la carta al mazo
    return true;
}

static Card* deck_pop(Deck* self) {
    if (self == NULL || deck_is_empty(self)) {
        return NULL;
    }
    
    Card* card = self->cards[self->top];
    self->cards[self->top] = NULL;
    self->top--;

    return card; // Retorna el puntero a la carta, pero no la libera de memoria
}

static Card* deck_peek(const Deck* self) {
    if (self == NULL || deck_is_empty(self)) {
        return NULL;
    }
    return self->cards[self->top];
}

static bool deck_is_empty(const Deck* self) {
    return (self != NULL) ? self->top < 0 : true;
}

static bool deck_is_full(const Deck* self) {
    return (self != NULL) ? (self->top >= MAX_STACK_SIZE - 1) : false;
}

static int deck_get_size(const Deck* self) {
    return (self != NULL) ? (self->top + 1) : 0;
}

static void deck_clear(Deck* self) {
    if (self == NULL) return;

    for (int i = 0; i <= self->top; i++) {
        self->cards[i] = NULL; // Solo eliminamos las referencias a las cartas, no las liberamos
    }
    self->top = -1;
}

static void deck_shuffle(Deck* self) {
    if (self == NULL || deck_is_empty(self)) return;

    srand((unsigned int)time(NULL)); // Semilla para la aleatoriedad

    for (int i = self->top; i > 0; i--) {
        int j = rand() % (i + 1); // Índice aleatorio entre 0 y i
        // Intercambiar cartas en posiciones i y j
        Card* temp = self->cards[i];
        self->cards[i] = self->cards[j];
        self->cards[j] = temp;
    }
}

static void deck_set_name(Deck* self, const char* name) {
    if (self == NULL || name == NULL) return;
    
    if (self->name != NULL) {
        free((void*)self->name); // Liberar el nombre anterior si existe
    }
    
    // Usar bs_safe_strdup (implementación segura)
    self->name = bs_safe_strdup(name);
    if (self->name == NULL) {
        throw(BS_ERROR_MEMORY_ALLOCATION, "No se pudo copiar el nombre del mazo");
    }
}

static const char* deck_get_name(const Deck* self) {
    return (self != NULL) ? self->name : NULL;
}


// funciones de testeo
static void print_deck(const Deck* deck) {
    if (deck == NULL) {
        printf("Mazo nulo\n");
        return;
    }
    printf("Nombre del mazo: %s\n", deck->get_name(deck));
    int size = deck_get_size(deck);
    printf("Mazo (tamaño: %d):\n", size);
    for (int i = 0; i < size; i++) {
        Card* card = deck->cards[i];
        if (card != NULL) {
            printf("  Carta %d: ID=%d, Nombre=%s\n", i, card->get_id(card), card->get_name(card));
        } else {
            printf("  Carta %d: NULL\n", i);
        }
    }
}