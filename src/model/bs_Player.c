#include "model/bs_Player.h"

// ==================== DECLARACIONES FORWARD ====================
static int player_get_id(const Player* self);
static const char* player_get_name(const Player* self);
static Deck* player_get_deck(Player* self);
static Deck* player_get_active_deck(Player* self);
static Deck* player_get_discard_pile(Player* self);
static Board* player_get_board(Player* self);
static Fleet* player_get_fleet(Player* self);
static PlayerBuffer* player_get_buffer(Player* self);
static int player_get_tower_count(const Player* self);
static bool player_has_instant_kill(const Player* self);
static bool player_has_salvo_active(const Player* self);
static int player_get_salvo_multiplier(const Player* self);
static int player_get_total_hits(const Player* self);
static void player_set_last_input(Player* self, int row, int col);
static void player_set_last_hit(Player* self, int row, int col);
static void player_increment_hits(Player* self);
static void player_reset_turn_hits(Player* self);
static void player_set_row_check(Player* self, int row, int cells_found);
static void player_set_column_check(Player* self, int col, int cells_found);
static CheckData player_get_row_check(const Player* self, int row);
static CheckData player_get_column_check(const Player* self, int col);

// ==================== CONSTRUCTOR ====================
Player* bs_Player_create(int id, const char* name) {
    Player* player = NULL;
    
    try {
        player = (Player*)malloc(sizeof(Player));
        if (player == NULL) {
            throw(BS_ERROR_MEMORY_ALLOCATION, "No se pudo crear Player");
        }
        
        player->id = id;
        player->name = bs_safe_strdup(name);
        if (player->name == NULL) {
            throw(BS_ERROR_STRDUP_FAILED, "No se pudo copiar nombre del jugador");
        }
        
        // Crear mazos
        player->deck = bs_Deck_create("Deck Principal");
        player->active_deck = bs_Deck_create("Deck Activo");
        player->discard_pile = bs_Deck_create("Mazo de Descarte");
        
        if (player->deck == NULL || player->active_deck == NULL || player->discard_pile == NULL) {
            throw(BS_ERROR_MEMORY_ALLOCATION, "No se pudieron crear los decks");
        }
        
        // Inicializar board y fleet (NULL por ahora)
        player->board = NULL;
        player->fleet = NULL;
        
        // Crear buffer
        player->buffer = (PlayerBuffer*)calloc(1, sizeof(PlayerBuffer));
        if (player->buffer == NULL) {
            throw(BS_ERROR_MEMORY_ALLOCATION, "No se pudo crear buffer");
        }
        
        // Asignar métodos (OOP)
        player->get_id = player_get_id;
        player->get_name = player_get_name;
        player->get_deck = player_get_deck;
        player->get_active_deck = player_get_active_deck;
        player->get_discard_pile = player_get_discard_pile;
        player->get_board = player_get_board;
        player->get_fleet = player_get_fleet;
        player->get_buffer = player_get_buffer;
        player->get_tower_count = player_get_tower_count;
        player->has_instant_kill = player_has_instant_kill;
        player->has_salvo_active = player_has_salvo_active;
        player->get_salvo_multiplier = player_get_salvo_multiplier;
        player->get_total_hits = player_get_total_hits;
        player->set_last_input = player_set_last_input;
        player->set_last_hit = player_set_last_hit;
        player->increment_hits = player_increment_hits;
        player->reset_turn_hits = player_reset_turn_hits;
        player->set_row_check = player_set_row_check;
        player->set_column_check = player_set_column_check;
        player->get_row_check = player_get_row_check;
        player->get_column_check = player_get_column_check;
        
        return player;
    }
    catch_any {
        if (player != NULL) {
            free((void*)player->name);
            bs_Deck_destroy(player->deck);
            bs_Deck_destroy(player->active_deck);
            bs_Deck_destroy(player->discard_pile);
            free(player->buffer);
            free(player);
        }
        return NULL;
    }
}

void bs_Player_destroy(Player* player) {
    if (player == NULL) return;
    
    free((void*)player->name);
    
    // Destruir mazos (NO destruyen cartas, solo referencias)
    bs_Deck_destroy(player->deck);
    bs_Deck_destroy(player->active_deck);
    bs_Deck_destroy(player->discard_pile);
    

    /**
     * TODO: Destruir board y fleet (implementar luego)
     * bs_Board_destroy(player->board);
     * bs_Fleet_destroy(player->fleet);
     */
    
    free(player->buffer);
    free(player);
}

// ==================== IMPLEMENTACIONES DE GETTERS ====================
static int player_get_id(const Player* self) {
    return (self != NULL) ? self->id : -1;
}

static const char* player_get_name(const Player* self) {
    return (self != NULL) ? self->name : NULL;
}

static Deck* player_get_deck(Player* self) {
    return (self != NULL) ? self->deck : NULL;
}

static Deck* player_get_active_deck(Player* self) {
    return (self != NULL) ? self->active_deck : NULL;
}

static Deck* player_get_discard_pile(Player* self) {
    return (self != NULL) ? self->discard_pile : NULL;
}

static Board* player_get_board(Player* self) {
    return (self != NULL) ? self->board : NULL;
}

static Fleet* player_get_fleet(Player* self) {
    return (self != NULL) ? self->fleet : NULL;
}

static PlayerBuffer* player_get_buffer(Player* self) {
    return (self != NULL) ? self->buffer : NULL;
}

static int player_get_tower_count(const Player* self) {
    return (self != NULL && self->buffer != NULL) ? self->buffer->tower_advantage_count : 0;
}

static bool player_has_instant_kill(const Player* self) {
    return (self != NULL && self->buffer != NULL) ? self->buffer->instant_kill_active : false;
}

static bool player_has_salvo_active(const Player* self) {
    return (self != NULL && self->buffer != NULL) ? self->buffer->salvo_active : false;
}

static int player_get_salvo_multiplier(const Player* self) {
    return (self != NULL && self->buffer != NULL) ? self->buffer->salvo_multiplier : 0;
}

static int player_get_total_hits(const Player* self) {
    return (self != NULL && self->buffer != NULL) ? self->buffer->total_hits : 0;
}

// ==================== IMPLEMENTACIONES DE SETTERS ====================
static void player_set_last_input(Player* self, int row, int col) {
    if (self != NULL && self->buffer != NULL) {
        self->buffer->last_row_input = row;
        self->buffer->last_col_input = col;
    }
}

static void player_set_last_hit(Player* self, int row, int col) {
    if (self != NULL && self->buffer != NULL) {
        self->buffer->last_row_hit = row;
        self->buffer->last_col_hit = col;
    }
}

static void player_increment_hits(Player* self) {
    if (self != NULL && self->buffer != NULL) {
        self->buffer->hits_in_turn++;
        self->buffer->total_hits++;
    }
}

static void player_reset_turn_hits(Player* self) {
    if (self != NULL && self->buffer != NULL) {
        self->buffer->hits_in_turn = 0;
    }
}

// ==================== GESTIÓN DE CHECKS ====================
static void player_set_row_check(Player* self, int row, int cells_found) {
    if (self != NULL && self->buffer != NULL && row >= 0 && row < BOARD_SIZE) {
        self->buffer->row_checks[row].checked = true;
        self->buffer->row_checks[row].cells_found = cells_found;
    }
}

static void player_set_column_check(Player* self, int col, int cells_found) {
    if (self != NULL && self->buffer != NULL && col >= 0 && col < BOARD_SIZE) {
        self->buffer->column_checks[col].checked = true;
        self->buffer->column_checks[col].cells_found = cells_found;
    }
}

static CheckData player_get_row_check(const Player* self, int row) {
    CheckData empty = {false, 0};
    if (self == NULL || self->buffer == NULL || row < 0 || row >= BOARD_SIZE) {
        return empty;
    }
    return self->buffer->row_checks[row];
}

static CheckData player_get_column_check(const Player* self, int col) {
    CheckData empty = {false, 0};
    if (self == NULL || self->buffer == NULL || col < 0 || col >= BOARD_SIZE) {
        return empty;
    }
    return self->buffer->column_checks[col];
}