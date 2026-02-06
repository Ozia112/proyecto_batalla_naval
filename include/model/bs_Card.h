#ifndef BS_CARD_H
#define BS_CARD_H

#include "bs_lang.h"

typedef struct Card Card;
typedef struct Context Context;
typedef struct Redraw Redraw;

typedef enum ActionType {
    ACTION_NONE = 0,
    ACTION_SHOOT,
    ACTION_SCAN,
    ACTION_IMPROVE
} ActionType;

typedef enum Pattern {
    PATTERN_NONE = 0,
    PATTERN_SINGLE,
    PATTERN_LINE,
    PATTERN_AREA
} Pattern;

typedef enum InputType {
    INPUT_CONFIRMATION,
    INPUT_COORDINATES,
    INPUT_ROW,
    INPUT_COLUMN
} InputType;

typedef struct Redraw {
    bool can_redraw;
    bool is_random;
    int redraw_card_id;
} Redraw;

typedef struct Context {
    ActionType type;
    Pattern pattern;
    InputType input_type;
    int action_times;
    Redraw redraw;
} Context;

typedef struct Card {
    int id;
    int qty;
    const char* name;
    const char* description;
    Context context;
    
    // Métodos (estilo OOP) - Solo getters, NO ejecución
    int (*get_id)(const struct Card* self);
    int (*get_qty)(const struct Card* self);
    const char* (*get_name)(const struct Card* self);
    const char* (*get_description)(const struct Card* self);
    const Context* (*get_context)(const struct Card* self);
    ActionType (*get_action_type)(const struct Card* self);
    Pattern (*get_pattern)(const struct Card* self);
    InputType (*get_input_type)(const struct Card* self);
    int (*get_action_times)(const struct Card* self);
    const Redraw* (*get_redraw)(const struct Card* self);
} Card;

// Constructor desde base de datos interna (temporal)
Card* bs_Card_create_from_database(int card_id);

// Constructor desde archivo
Card* bs_Card_create_from_data(int id, int qty, const char* name, const char* description, Context context);

// Destructor
void bs_Card_destroy(Card* card);

#endif // BS_CARD_H