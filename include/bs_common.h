#ifndef BS_COMMON_H
#define BS_COMMON_H

#include <stdbool.h> // Incluye definiciones de tipo booleano estándar

// Error codes
#define INVALID_INPUT 0 // Entrada vacía o inválida
#define INVALID_FORMAT 1 // Formato de entrada inválido
#define OUT_OF_RANGE 2 // Coordenadas fuera de rango
#define INVALID_DIRECTION 3 // Dirección no válida para el barco
#define INVALID_DIMENSION 4 // Dimensiones del barco no coinciden con las coordenadas
#define OVERLAP_DETECTED 5 // Solapamiento de barcos detectado
#define PREVIOUSLY_SHOT 6 // Casilla ya disparada previamente

// Definiciones comunes compartidas en todo el proyecto
#define BOARD_SIZE 10
#define MAX_NAME_LENGTH 21
#define NUM_SHIPS 5
#define VICTORY_COND 17 // Número de partes de barco que se deben hundir para ganar
#define CARD_QTY 11
#define SHIP_CELLS_QTY VICTORY_COND // Número total de partes de barco
#define UNSET -1 // Valor para indicar que una celda no está establecida
#define NONE 0
#define EMPTY '\0' // Valor para indicar que una cadena está vacía
#define EMPTY_STR "" // Cadena vacía para uso general

// Definiciones array status
#define CC_STATUS 2 // Columna de estado en la matriz de barcos
#define CC_ROW 0 // Columna de fila en la matriz de barcos
#define CC_COLUMN 1 // Columna de columna en la matriz de barcos

// Valores numericos para interpretar el tablero
#define WATER 0
#define FAILED_SHOT UNSET // Disparo fallido
#define SHIP_STER 1
#define SHIP_BODY 2
#define SHIP_STER_D 3
#define SHIP_BODY_D 4

// Valores para interpretar los numeros como caracteres

#define STER_PRINT 207 // '¤'
#define BODY_PRINT 254 // '■'
#define WATER_PRINT 126 // '~'
#define FAILED_SHOT_PRINT 158 // '×'

// Common structure definitions
struct board
{
    int ship_id; // ID del barco
    int ship_cell;
    bool is_water; // Indica si la celda es agua
    int status; // Estado de la celda: 0 = agua, 1 = punta intacta, 2 = cuerpo intacto, 3 = punta dañada, 4 = cuerpo dañado, -1 = disparo fallido

};


struct ship
{
    int ship_id; // ID del barco
    char ship_name[MAX_NAME_LENGTH]; // Nombre del barco
    int ship_size; // alojar la dimension del barco
    char ship_direction; // 'E' for east, 'W' for west, 'N' for north, 'S' for south, 'U' for undefined
    int **status; // status[ship_size][3]: [fila][0]=x, [fila][1]=y, [fila][2]=estado (1=punta, 2=cuerpo, 3=punta dañada, 4=cuerpo dañada)
    bool is_alive; // Indica si el barco está vivo
};

struct card
{
    int card_id;
    char card_name[64];
    char description[256];
    int peso;
};

struct player
{
    // Variables de innicializacion
    int player_index;
    char player_name[MAX_NAME_LENGTH];
    int placed_ships;
    int turn; // Turno del jugador
    int acc_turns; // Acumula los turnos del jugador
    // Variables para barcos
    struct ship ships[NUM_SHIPS]; // Array of ships for the player
    int enemy_hit_parts; // Numero de partes de barco enemigo alcanzadas
    int sunken_ships; // Numero de barcos enemigos hundidos
    int remain_ship_cells; // Numero de barcos restantes del jugador

    struct board board[BOARD_SIZE][BOARD_SIZE]; // Tablero del jugador


    // Variables para cartas
    struct card cards[CARD_QTY]; // Array de cartas
    int cards_total_weight; // Peso total de las cartas
    int original_cards_weight[CARD_QTY]; // Peso original de las cartas
    int acc_towers; // Numero de torres acumuladas
    bool input_valid; // Indica si la entrada del jugador es válida
    bool row_check[BOARD_SIZE]; // Variable para guardar la fila seleccionada por el jugador
    int cellCntRow[BOARD_SIZE]; // Contador para el chequeo de fila
    int prev_check_row; // Almacena la última fila chequeada
    bool col_check[BOARD_SIZE]; // Variable para guardar la columna seleccionada por el jugador
    int cellCntCol[BOARD_SIZE]; // Contador para el chequeo de columna
    int prev_check_col; // Almacena la última columna chequeada
    bool salvo_loaded;
    bool salvo_mode;
    bool upgrade_enable;

    // Buffer registers
    int prevRowInput; // Almacena la última coordenada de disparo
    int prevColInput; // Almacena la última coordenada de disparo
    int prevHitRow; // Almacena la última coordenada de disparo exitosa
    int prevHitCol; // Almacena la última coordenada de disparo exitosa
    int prevCard; // Almacena el ID de la última carta utilizada
    int hitsInTurn; // Almacena el número de aciertos por turno
};

static inline void reset_buffer_register(struct player *player) {
    // Reiniciar las variables de tipo buffer
    player->prevRowInput = UNSET;
    player->prevColInput = UNSET;
    player->prevHitRow = UNSET;
    player->prevHitCol = UNSET;
    player->hitsInTurn = 0;
    player->prevCard = UNSET;
}

#endif // BS_COMMON_H