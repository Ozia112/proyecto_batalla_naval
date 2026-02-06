#include "controller/bs_CardManager.h"
#include "utils/bs_error_handler.h"
#include <stdio.h>

// ==================== DECLARACIONES FORWARD ====================
static bool manager_load_cards_from_database(CardManager* self);
static bool manager_load_cards_from_file(CardManager* self, const char* filename);
static void manager_initialize_deck(CardManager* self, Deck* deck);
static bool manager_transfer_card(CardManager* self, Deck* source, Deck* target);
static Card* manager_find_card_by_id(CardManager* self, int card_id);
static void manager_print_card_info(const CardManager* self, const Card* card);

// ==================== CONSTRUCTOR ====================
CardManager* bs_CardManager_create(void) {
    CardManager* manager = NULL;
    
    try {
        manager = (CardManager*)malloc(sizeof(CardManager));
        if (manager == NULL) {
            throw(BS_ERROR_MEMORY_ALLOCATION, "No se pudo asignar memoria para CardManager");
        }

        // Inicializar pool
        for (int i = 0; i < TOTAL_CARDS_IN_GAME; i++) {
            manager->card_pool[i] = NULL;
        }
        manager->total_cards = 0;

        // Asignar métodos
        manager->load_cards_from_database = manager_load_cards_from_database;
        manager->load_cards_from_file = manager_load_cards_from_file;
        manager->initialize_deck = manager_initialize_deck;
        manager->transfer_card = manager_transfer_card;
        manager->find_card_by_id = manager_find_card_by_id;
        manager->print_card_info = manager_print_card_info;

        return manager;
    }
    catch_any {
        if (manager != NULL) {
            free(manager);
        }
        return NULL;
    }
}

// ==================== DESTRUCTOR ====================
void bs_CardManager_destroy(CardManager* manager) {
    if (manager == NULL) return;

    // ✅ Destruir todas las cartas del pool
    for (int i = 0; i < manager->total_cards; i++) {
        if (manager->card_pool[i] != NULL) {
            bs_Card_destroy(manager->card_pool[i]);
            manager->card_pool[i] = NULL;
        }
    }

    free(manager);
}

// ==================== CARGAR DESDE BASE DE DATOS ====================
static bool manager_load_cards_from_database(CardManager* self) {
    if (self == NULL) return false;

    try {
        self->total_cards = 0;

        // Crear cartas según su cantidad (qty)
        // Card ID 1: qty=7 → crear 7 instancias
        // Card ID 2: qty=5 → crear 5 instancias
        // etc.
        
        int card_definitions[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
        int card_quantities[] = {7, 5, 2, 4, 1, 1, 1, 3, 2, 1, 1};
        int num_definitions = 11;

        int pool_index = 0;

        for (int i = 0; i < num_definitions; i++) {
            int card_id = card_definitions[i];
            int qty = card_quantities[i];

            for (int j = 0; j < qty; j++) {
                if (pool_index >= TOTAL_CARDS_IN_GAME) {
                    throw(BS_ERROR_INVALID_PARAMETER, "Demasiadas cartas para el pool");
                }

                self->card_pool[pool_index] = bs_Card_create_from_database(card_id);
                if (self->card_pool[pool_index] == NULL) {
                    throw(BS_ERROR_MEMORY_ALLOCATION, "No se pudo crear carta");
                }

                pool_index++;
                self->total_cards++;
            }
        }

        return true;
    }
    catch_any {
        return false;
    }
}

// ==================== CARGAR DESDE ARCHIVO ====================
static bool manager_load_cards_from_file(CardManager* self, const char* filename) {
    if (self == NULL || filename == NULL) return false;

    FILE* file = NULL;
    
    try {
        file = fopen(filename, "r");
        if (file == NULL) {
            throw(BS_ERROR_FILE_NOT_FOUND, "No se pudo abrir archivo de cartas");
        }

        self->total_cards = 0;
        char line[512];
        int pool_index = 0;

        while (fgets(line, sizeof(line), file) != NULL && pool_index < TOTAL_CARDS_IN_GAME) {
            // Ignorar comentarios
            if (line[0] == '#' || line[0] == '\n') continue;

            // Parsear línea (formato simplificado)
            // id,qty,name,description,action_type,pattern,input_type,action_times
            int id, qty;
            char name[64], description[256];
            int action_type, pattern, input_type, action_times;

            if (sscanf(line, "%d,%d,%63[^,],%255[^,],%d,%d,%d,%d",
                      &id, &qty, name, description,
                      &action_type, &pattern, &input_type, &action_times) == 8) {
                
                // Crear Context
                Context context = {
                    .type = (ActionType)action_type,
                    .pattern = (Pattern)pattern,
                    .input_type = (InputType)input_type,
                    .action_times = action_times,
                    .redraw = {false, false, NONE}
                };

                // Crear qty instancias de esta carta
                for (int j = 0; j < qty && pool_index < TOTAL_CARDS_IN_GAME; j++) {
                    self->card_pool[pool_index] = bs_Card_create_from_data(id, qty, name, description, context);
                    if (self->card_pool[pool_index] == NULL) {
                        throw(BS_ERROR_MEMORY_ALLOCATION, "No se pudo crear carta desde archivo");
                    }
                    pool_index++;
                    self->total_cards++;
                }
            }
        }

        fclose(file);
        return true;
    }
    catch_any {
        if (file != NULL) {
            fclose(file);
        }
        return false;
    }
}

// ==================== INICIALIZAR DECK ====================
static void manager_initialize_deck(CardManager* self, Deck* deck) {
    if (self == NULL || deck == NULL) return;

    // Limpiar deck
    deck->clear(deck);

    // Agregar todas las cartas del pool al deck
    for (int i = 0; i < self->total_cards; i++) {
        deck->push(deck, self->card_pool[i]);
    }

    // Barajar
    deck->shuffle(deck);
}

// ==================== TRANSFERIR CARTA ====================
static bool manager_transfer_card(CardManager* self, Deck* source, Deck* target) {
    if (self == NULL || source == NULL || target == NULL) {
        return false;
    }

    if (source->is_empty(source)) {
        return false;  // No hay cartas para transferir
    }

    if (target->is_full(target)) {
        return false;  // Deck destino lleno
    }

    Card* card = source->pop(source);
    if (card == NULL) {
        return false;
    }

    return target->push(target, card);
}

// ==================== BUSCAR CARTA ====================
static Card* manager_find_card_by_id(CardManager* self, int card_id) {
    if (self == NULL) return NULL;

    for (int i = 0; i < self->total_cards; i++) {
        if (self->card_pool[i] != NULL && 
            self->card_pool[i]->get_id(self->card_pool[i]) == card_id) {
            return self->card_pool[i];
        }
    }

    return NULL;
}

// ==================== IMPRIMIR INFO ====================
static void manager_print_card_info(const CardManager* self, const Card* card) {
    if (self == NULL || card == NULL) return;

    printf("=== CARTA ===\n");
    printf("ID: %d\n", card->get_id(card));
    printf("Nombre: %s\n", card->get_name(card));
    printf("Descripcion: %s\n", card->get_description(card));
    printf("Accion: %d\n", card->get_action_type(card));
    printf("Patron: %d\n", card->get_pattern(card));
    printf("=============\n");
}