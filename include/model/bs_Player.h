#ifndef BS_PLAYER_H
#define BS_PLAYER_H

#include "bs_lang.h"
#include "bs_Deck.h"
#include "bs_Fleet.h"
#include "bs_Board.h"

typedef struct CheckData {
    bool checked;
    int cells_found;
}CheckData;

typedef struct PlayerBuffer {
    // Chequeos de escaneo
    CheckData row_checks[BOARD_SIZE];
    CheckData column_checks[BOARD_SIZE];

    // Ultimos inputs
    int last_col_input;
    int last_row_input;

    // Hits
    int last_col_hit;
    int last_row_hit;
    int hits_in_turn;
    int total_hits;

    // Torre de ventaja
    int tower_advantage_count;
    bool instant_kill_active;
    bool salvo_active;
    int salvo_multiplier;
}PlayerBuffer;

typedef struct Player {
    int id;
    const char* name;

    // Mazos
    Deck* deck;         // Mazo de robo
    Deck* active_deck;  // Mazo activo (cartas boca arriba)
    Deck* discard_pile; // Mazo de descarte

    // Estado del juego
    Board* board;
    Fleet* fleet;

    // Buffer de datos temporales
    PlayerBuffer* buffer;

    // Metodos Getters y Setters basicos
    int (*get_id)(const struct Player* self);
    const char* (*get_name)(const struct Player* self);
    Deck* (*get_deck)(const struct Player* self);
    Deck* (*get_active_deck)(const struct Player* self);
    Deck* (*get_discard_pile)(const struct Player* self);
    Board* (*get_board)(const struct Player* self);
    Fleet* (*get_fleet)(const struct Player* self);
    PlayerBuffer* (*get_buffer)(const struct Player* self);

    // Getters de buffer
    int (*get_tower_count)(const struct Player* self);
    bool (*has_instant_kill)(const struct Player* self);
    bool (*has_salvo_active)(const struct Player* self);
    int (*get_salvo_multiplier)(const struct Player* self);
    int (*get_total_hits)(const struct Player* self);
    
    // Setters de buffer
    void (*set_last_input)(struct Player* self, int row, int col);
    void (*set_last_hit)(struct Player* self, int row, int col);
    void (*increment_hits)(struct Player* self);
    void (*reset_turn_hits)(struct Player* self);
    
    // Gestión de checks
    void (*set_row_check)(struct Player* self, int row, int cells_found);
    void (*set_column_check)(struct Player* self, int col, int cells_found);
    CheckData (*get_row_check)(const struct Player* self, int row);
    CheckData (*get_column_check)(const struct Player* self, int col);
} Player;

Player* bs_Player_create(int id, const char* name);
void bs_Player_destroy(Player* player);

#endif // BS_PLAYER_H