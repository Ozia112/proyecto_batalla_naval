#ifndef BS_CONSOLE_UTILS_H
#define BS_CONSOLE_UTILS_H

#include "master.h" // Incluye todas las cabeceras del proyecto
#include <windows.h> // Para funciones de consola en Windows
#include <stdio.h> // Para printf y getchar
#include <stdarg.h> // Para manejo de argumentos variables

bool debugMode; // Declaración de la variable global

/* Color code in hexadecimal */
#define NEGRO       0x000000
#define AZUL        0x0037DA
#define VERDE       0x14990D
#define AQUA        0x00FFFF
#define ROJO        0xC50F1F
#define PURPUR      0x9635A5
#define AMARILLO    0xE2B500
#define GRIS_CLARO  0xCCCCCC
#define GRIS_OSCURO 0x767676
#define AZUL_CLARO  0x3B78FF
#define VERDE_CLARO 0x1FF210
#define AQUA_CLARO  0x70FFFF
#define ROJO_CLARO  0xE54454
#define FUCSIA      0xE500CA
#define AMARILLO_C  0xF7E971
#define BLANCO      0xFFFFFF


/* Colores específicos para el juego */
#define WATER_COLOR      AZUL_CLARO
#define ERROR_COLOR      ROJO
#define INFO_COLOR       AMARILLO
#define SHIP_COLOR       BLANCO
#define SUCCESS_COLOR    VERDE
#define INV_COLOR        NEGRO
#define DEFAULT          -1

static inline void set_CP_config(int page) {
    char command[32];
    sprintf(command, "chcp %d > nul", page); // Cambia la codificación de la consola a page para evitar problemas con caracteres especiales.
    system(command);
    SetConsoleCP(page);
    SetConsoleOutputCP(page);
}

static inline void clear_screen(){
	system("cls");
}

static inline void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static inline void pause_timer(float seconds) {
    Sleep(seconds * 1000);
}

static inline void printf_color(int text_hex, int bg_hex, const char *format, ...) {
    int tr = 255, tg = 255, tb = 255; // Blanco por defecto
    
    if (text_hex != DEFAULT) printf("\033[38;2;%d;%d;%dm", (text_hex >> 16) & 0xFF, (text_hex >> 8) & 0xFF, text_hex & 0xFF);

    if (bg_hex != DEFAULT) printf("\033[48;2;%d;%d;%dm", (bg_hex >> 16) & 0xFF, (bg_hex >> 8) & 0xFF, bg_hex & 0xFF);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("\033[0m"); // Restaurar los colores por defecto
}

static inline void enter_to_message(const char *message, bool clear_screen_after) {
    printf("Presione "); printf_color(INFO_COLOR, DEFAULT, "ENTER"); printf(" para ");
    printf_color(INFO_COLOR, DEFAULT, message);
    printf("\n");
    while (getchar() != '\n'); // Esperar a que el usuario presione enter.
    if (clear_screen_after) clear_screen();
}

static inline void return_line(int line_qty) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int width = 120;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        width = csbi.srWindow.Right - csbi.srWindow.Left + 1; // Obtener el ancho de la consola
    }
    for (int i = 0; i < line_qty; i++) {
        printf("\033[A\r%*s\r", width, ""); // Mover el cursor hacia arriba y limpiar la línea
    }
    fflush(stdout); // Asegurarse de que el buffer se vacíe
}

static inline void codeError(struct player *player, int code, int ship_index, int row, int column) {
    int line_qty = 0;

    switch(code) {
        case INVALID_INPUT:
            printf_color(ERROR_COLOR, DEFAULT, "Entrada vacia invalida. ");
            printf_color(INFO_COLOR, DEFAULT, " CodeError: %d\n", code);
            line_qty ++;
            break;
        case INVALID_FORMAT:
            printf_color(ERROR_COLOR, DEFAULT, "Formato de entrada invalido. ");
            printf_color(INFO_COLOR, DEFAULT, " CodeError: %d\n", code);
            line_qty ++;
            break;
        case OUT_OF_RANGE:
            if (player && (player->prevCard == 3 || player->prevCard == 6)) {
                printf_color(ERROR_COLOR, DEFAULT, "Fila fuera de rango. ");
                printf_color(INFO_COLOR, DEFAULT, " CodeError: %d\n", code);
                line_qty ++;
            } else if (player && (player->prevCard == 4 || player->prevCard == 7)) {
                printf_color(ERROR_COLOR, DEFAULT, "Columna fuera de rango. ");
                printf_color(INFO_COLOR, DEFAULT, " CodeError: %d\n", code);
                line_qty ++;
            }
            if (row < 0 || row >= BOARD_SIZE) {
                printf_color(ERROR_COLOR, DEFAULT, "Coordenada fuera de rango(");
                printf_color(INFO_COLOR, DEFAULT, "Fila");
                printf_color(ERROR_COLOR, DEFAULT, ")");
                printf_color(INFO_COLOR, DEFAULT, " CodeError: %d\n", code);
                line_qty ++;
            }
            if (column < 0 || column >= BOARD_SIZE) {
                printf_color(ERROR_COLOR, DEFAULT, "Coordenada fuera de rango(");
                printf_color(INFO_COLOR, DEFAULT, "Columna");
                printf_color(ERROR_COLOR, DEFAULT, ")");
                printf_color(INFO_COLOR, DEFAULT, " CodeError: %d\n", code);
                line_qty ++;
            }
            break;
        case INVALID_DIRECTION:
            printf_color(ERROR_COLOR, DEFAULT,"Orientacion no permitida, el barco debe ser horizontal o vertical.");
            printf_color(INFO_COLOR, DEFAULT, " CodeError: %d\n", code);
            line_qty ++;
            break;
        case INVALID_DIMENSION:
            printf_color(ERROR_COLOR, DEFAULT,"Las coordenadas no son validas. El barco debe tener");
            printf_color(INFO_COLOR, DEFAULT," %d", player->ships[ship_index].ship_size);
            printf_color(ERROR_COLOR, DEFAULT," celdas de longitud.", player->ships[ship_index].ship_size);
            printf_color(INFO_COLOR, DEFAULT, " CodeError: %d\n", code);
            line_qty ++;
            break;
        case OVERLAP_DETECTED:
            printf_color(ERROR_COLOR, DEFAULT,"Solapamiento detectado en");
            printf_color(INFO_COLOR, DEFAULT," %c,%d", row + 'A', column + 1);
            printf_color(DEFAULT, DEFAULT," con el barco %d", ship_index + 1);
            printf_color(INFO_COLOR, DEFAULT, " CodeError: %d\n", code);
            line_qty ++;
            break;
        case PREVIOUSLY_SHOT:
            printf_color(ERROR_COLOR, DEFAULT,"Casilla ya disparada previamente");
            printf_color(INFO_COLOR, DEFAULT, " CodeError: %d\n", code);
            line_qty ++;
            break;
    }
    
    line_qty += 3; // For the "Press ENTER to continue" message and the extra line before it
    enter_to_message("reintentar", false); // Enter to continue
    return_line(line_qty); // Return to the previous lines
}

static inline void deBugPrint(int index, int debug_value1, int debug_value2, char debug_char, char *debug_str) {
    if (debugMode) {
        printf_color(AMARILLO, DEFAULT, "[DEBUG]: ");
        switch (index)
        {
        case 0:
            printf_color(INFO_COLOR, DEFAULT,"Peso carta 1: %d", debug_value1);
            break;
        case 1:
            printf_color(INFO_COLOR, DEFAULT,"Peso carta 10: %d", debug_value1);
            break;
        case 2:
            printf_color(INFO_COLOR, DEFAULT, "Peso de la carta %d restaurado a %d", debug_value1, debug_value2);
            break;
        case 3:
            printf_color(INFO_COLOR, DEFAULT, "Cartas restantes: %d", debug_value1);
            break;
            case 4:
            printf_color(INFO_COLOR, DEFAULT, "Coordenadas ingresadas: %c, %d", debug_value1 + 'A',debug_value2 + 1);
            break;
        case 5:
            printf_color(INFO_COLOR, DEFAULT, "Printing code error for invalid input.");
            break;
        case 6:
            printf_color(INFO_COLOR, DEFAULT, "Row character parsed: %c", debug_char);
            break;
        case 7:
            printf_color(INFO_COLOR, DEFAULT, "Printing code error for invalid input.");
            break;
        case 8:
            printf_color(INFO_COLOR, DEFAULT, "Input received: %s", debug_str);
            break;
        case 9:
            printf_color(INFO_COLOR, DEFAULT, "Input after newline removal: %s", debug_str);
            break;
        case 10:
            printf_color(INFO_COLOR, DEFAULT, "Column number parsed: %d", debug_value1);
            break;

        default:
            break;
        }
    }
}

#endif