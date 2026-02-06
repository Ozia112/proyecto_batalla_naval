#include "utils/bs_input_parser.h"

// Convertir letra a numero
static int letter_to_number(char letter) {
    letter = toupper(letter);
    try {
        if (letter < 'A' || letter > 'J') {
            throw(BS_ERROR_INVALID_PARAMETER, "La letra debe estar entre A y J");
        }
        return letter - 'A';
    } catch(BS_ERROR_INVALID_PARAMETER) {
        return -1;
    }
}

