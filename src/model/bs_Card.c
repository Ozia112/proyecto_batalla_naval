#include "model/bs_Card.h"

/**
 * TODO: Este directorio de cantidad de cartas, nombres y
 * descripciones debe pasarse desde un archivo de configuración
 * la extracción de datos se hará mas adelante.
*/
#define TOTAL_CARD_IDS 11

// Estructura interna para la base de datos
typedef struct {
    int id;
    int qty;
    const char* name;
    const char* description;
    Context context;
} CardData;

// ==================== DECLARACIONES FORWARD ====================
static int card_get_id(const Card* self);
static int card_get_qty(const Card* self);
static const char* card_get_name(const Card* self);
static const char* card_get_description(const Card* self);
static const Context* card_get_context(const Card* self);
static ActionType card_get_action_type(const Card* self);
static Pattern card_get_pattern(const Card* self);
static InputType card_get_input_type(const Card* self);
static int card_get_action_times(const Card* self);
static const Redraw* card_get_redraw(const Card* self);

// Auxiliares
static void assign_card_methods(Card* card);
Card* bs_Card_create_from_data(int id, int qty, const char* name, const char* description, Context context);

// Lifecycle
Card* bs_Card_create_from_database(int card_id);
void bs_Card_destroy(Card* card);

// Array de datos de cartas (temporal, debe ser reemplazado por lectura desde archivo)
static CardData card_database[TOTAL_CARD_IDS] = {
    {1, 7, "Dispara 1 tiro", "Elije tus coordenadas y dispara.",
        {ACTION_SHOOT, PATTERN_SINGLE, INPUT_COORDINATES, 1, {false, false, NONE}}},
    
    {2, 5, "Dispara 2 tiros", "Elije tus coordenadas y dispara. Hazlo de nuevo.",
        {ACTION_SHOOT, PATTERN_SINGLE, INPUT_COORDINATES, 2, {false, false, NONE}}},
    
    {3, 2, "Disparo 3 tiros", "Elije tus coordenadas y dispara. Hazlo dos veces más.",
        {ACTION_SHOOT, PATTERN_SINGLE, INPUT_COORDINATES, 3, {false, false, NONE}}},
    
    {4, 4, "Torre de ventaja", "Coloca la tarjeta en el mazo activo, toma otra tarjeta y sigue las instrucciones.",
        {ACTION_IMPROVE, PATTERN_NONE, INPUT_CONFIRMATION, 1, {true, true, NONE}}},
    
    {5, 1, "Tira y toma", "Elije tus coordenadas y dispara. Después toma una tarjeta y sigue las instrucciones.",
        {ACTION_SHOOT, PATTERN_SINGLE, INPUT_COORDINATES, 1, {true, true, NONE}}},
    
    {6, 1, "Revela", "Tu flota logró revelar una coordenada enemiga, ¡de la orden de disparo!.",
        {ACTION_SCAN, PATTERN_SINGLE, INPUT_CONFIRMATION, 1, {true, false, 1}}},
    
    {7, 1, "Bombardea una fila", "Nuestro buque de 4 casillas logró activar sus sistemas de ataque masivo, elige una letra para bombardear esa fila.",
        {ACTION_SHOOT, PATTERN_LINE, INPUT_ROW, 1, {false, false, NONE}}},
    
    {8, 3, "Bombardea una columna", "Nuestro buque de 5 casillas logró activar sus sistemas de ataque masivo, elige un numero para bombardear esa columna.",
        {ACTION_SHOOT, PATTERN_LINE, INPUT_COLUMN, 1, {false, false, NONE}}},
    
    {9, 2, "Chequeo fila", "Nuestro buque de 4 casillas logro activar sus radares, indiquenos la letra para escanear esa fila.",
        {ACTION_SCAN, PATTERN_LINE, INPUT_ROW, 1, {false, false, NONE}}},
    
    {10, 1, "Chequeo columna", "Nuestro buque de 4 casillas logro activar sus radares, indiquenos la columna para escanear esa columna.",
        {ACTION_SCAN, PATTERN_LINE, INPUT_COLUMN, 1, {false, false, NONE}}},
    
    {11, 1, "Salvo", "Coloca la tarjeta en el mazo activo, Cada vez que tomes una tarjeta de Dispara 1 tiro, dispara los mismos tiros que tengas en tu flota. Si un buque tuyo se hunde, esta tarjeta pierde todo el poder",
        {ACTION_IMPROVE, PATTERN_NONE, INPUT_CONFIRMATION, 1, {false, false, NONE}}}
};

static void assign_card_methods(Card* card) {
    card->get_id = card_get_id;
    card->get_qty = card_get_qty;
    card->get_name = card_get_name;
    card->get_description = card_get_description;
    card->get_context = card_get_context;
    card->get_action_type = card_get_action_type;
    card->get_pattern = card_get_pattern;
    card->get_input_type = card_get_input_type;
    card->get_action_times = card_get_action_times;
    card->get_redraw = card_get_redraw;
}

// ==================== CONSTRUCTOR ====================
Card* bs_Card_create_from_database(int card_id) {
    Card* card = NULL;
    
    try {
        if (card_id < 0 || card_id > TOTAL_CARD_IDS) {
            throw(BS_ERROR_INVALID_PARAMETER, "ID de carta fuera de rango");
        }

        CardData* data = NULL;

        // Buscar carta en la base de datos
        for (int i = 0; i < TOTAL_CARD_IDS; i++) {
            if (card_database[i].id == card_id) {
                data = &card_database[i];
                break;
            }
        }

        if (data == NULL) {
            throw(BS_ERROR_INVALID_PARAMETER, "ID de carta no encontrado en la base de datos");
        }

        card = bs_Card_create_from_data(data->id, data->qty, data->name, data->description, data->context);

        return card;
    } 
    catch_any {
        return NULL;
    }
}

Card* bs_Card_create_from_data(int id, int qty, const char* name, const char* description, Context context) {
    Card* card = NULL;

    try {
        if (name == NULL || description == NULL || qty < 0) {
            throw(BS_ERROR_INVALID_PARAMETER, "Parámetros inválidos para crear la carta");
        }

        card = (Card*)malloc(sizeof(Card));
        if (card == NULL) {
            throw(BS_ERROR_MEMORY_ALLOCATION, "No se pudo asignar memoria para la carta");
        }

        card->id = id;
        card->qty = qty;
        card->name = bs_safe_strdup(name);
        if (card->name == NULL) {
            throw(BS_ERROR_STRDUP_FAILED, "No se pudo copiar el nombre de la carta");
        }
        card->description = bs_safe_strdup(description);
        if (card->description == NULL) {
            throw(BS_ERROR_STRDUP_FAILED, "No se pudo copiar la descripción de la carta");
        }

        assign_card_methods(card);

        return card;
    }
    catch(BS_ERROR_STRDUP_FAILED) {
        if (card != NULL) {
            free((void*)card->name);
            free((void*)card->description);
            free(card);
        }
        return NULL;
    }
    catch_any {
        if (card != NULL) {
            free(card);
        }
        return NULL;
    }
}

// ==================== DESTRUCTOR ====================
void bs_Card_destroy(Card* card) {
    if (card == NULL) return;
    
    free((void*)card->name);
    free((void*)card->description);
    free(card);
}

// ==================== IMPLEMENTACIONES DE GETTERS ====================
static int card_get_id(const Card* self) {
    return (self != NULL) ? self->id : UNSET;
}

static int card_get_qty(const Card* self) {
    return (self != NULL) ? self->qty : NONE;
}

static const char* card_get_name(const Card* self) {
    return (self != NULL) ? self->name : NULL;
}

static const char* card_get_description(const Card* self) {
    return (self != NULL) ? self->description : NULL;
}

static const Context* card_get_context(const Card* self) {
    return (self != NULL) ? &self->context : NULL;
}

static ActionType card_get_action_type(const Card* self) {
    return (self != NULL) ? self->context.type : ACTION_NONE;
}

static Pattern card_get_pattern(const Card* self) {
    return (self != NULL) ? self->context.pattern : PATTERN_NONE;
}

static InputType card_get_input_type(const Card* self) {
    return (self != NULL) ? self->context.input_type : INPUT_CONFIRMATION;
}

static int card_get_action_times(const Card* self) {
    return (self != NULL) ? self->context.action_times : NONE;
}

static const Redraw* card_get_redraw(const Card* self) {
    return (self != NULL) ? &self->context.redraw : NULL;
}