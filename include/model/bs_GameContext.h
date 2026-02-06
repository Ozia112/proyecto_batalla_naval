#ifndef BS_GAMECONTEXT_H
#define BS_GAMECONTEXT_H

#include "bs_lang.h"
#include "bs_Player.h"

typedef enum GamePhase {
    PHASE_SETUP,
    PHASE_PLAYING,
    PHASE_GAMEOVER
} GamePhase;

typedef struct GameContext {
    Player* player1;
    Player* player2;
    Player* attacker; // Referencia al jugador en turno
    Player* attacked; // Referencia al jugador en espera
    GamePhase phase;
    int turn_number;
} GameContext;

// Funciones para manejar el contexto del juego
GameContext* bs_GameContext_create(void);
void bs_GameContext_destroy(GameContext* context);
void bs_GameContext_switch_turn(GameContext* context);
Player* bs_GameContext_get_attacker(GameContext* context);
Player* bs_GameContext_get_attacked(GameContext* context);

#endif // BS_GAMECONTEXT_H