#ifndef BS_INPUT_PARSER_H
#define BS_INPUT_PARSER_H

#include "bs_lang.h"

typedef struct Coordinates {
    int row;
    int col;
} Coordinates;

bool bs_InputParser_parse_coordinates(const char* input, Coordinates* coords);

bool bs_InputParser_parse_row(const char* input, int* row);

bool bs_InputParser_parse_column(const char* input, int* col);

bool bs_InputParser_validate_coordinates(const Coordinates* coords);

// Obtener input del jugador
bool bs_InputParser_get_coordinates_input(Coordinates* coord);
bool bs_InputParser_get_row_input(int* row);
bool bs_InputParser_get_column_input(int* col);
bool bs_InputParser_get_confirmation_input(bool* confirmation);
#endif // BS_INPUT_PARSER_H