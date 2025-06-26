#ifndef BS_CONSOLE_UTILS_H
#define BS_CONSOLE_UTILS_H

#include "master.h" // Incluye todas las cabeceras del proyecto
#include <windows.h> // Para funciones de consola en Windows
#include <stdio.h> // Para printf y getchar
#include <stdarg.h> // Para manejo de argumentos variables

/* Códigos ANSI básicos */
#define NEGRO           0
#define AZUL            1
#define VERDE           2
#define AQUA            3
#define ROJO            4
#define PURPUR          5
#define AMARILLO        6
#define GRIS_CLARO      7
#define GRIS_OSCURO     8
#define AZUL_CLARO      9
#define VERDE_CLARO    10
#define AQUA_CLARO     11
#define ROJO_CLARO     12
#define FUCSIA         13
#define AMARILLO_C     14
#define BLANCO         15

/* Colores específicos para el juego */
#define WATER_COLOR      AZUL_CLARO
#define ERROR_COLOR      ROJO
#define INFO_COLOR       AMARILLO
#define SHIP_COLOR       BLANCO
#define DEFAULT_COLOR    GRIS_CLARO
#define SUCCESS_COLOR    VERDE
#define INV_COLOR        NEGRO

static inline void set_CP_config(int page) {

    system("chcp page > nul"); // Cambia la codificación de la consola a page para evitar problemas con caracteres especiales.
    SetConsoleCP(page);
    SetConsoleOutputCP(page);
}

static inline void clear_screen(){
	system("cls");
}

static inline void color_txt(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

static inline void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static inline void pause_timer(float seconds) {
    Sleep(seconds * 1000);
}

static inline void printf_color(int color, const char *format, ...) {
    color_txt(color); // Cambiar el color del texto
    va_list args; // Inicializar la lista de argumentos variables
    va_start(args, format); // Iniciar la lista de argumentos
    vprintf(format, args); // Imprimir el texto formateado
    va_end(args); // Finalizar la lista de argumentos
    color_txt(DEFAULT_COLOR); // Restaurar el color por defecto
}

static inline void enter_continuar_cls() {
    printf("Presione enter para ");  printf_color(INFO_COLOR, "continuar.\n");
    while (getchar() != '\n'); // Esperar a que el usuario presione enter.
    clear_screen(); // Limpiar la pantalla después de mostrar el mensaje.
}

static inline void enter_continuar() {
    printf("Presione enter para ");  printf_color(INFO_COLOR, "continuar.\n");
    while (getchar() != '\n'); // Esperar a que el usuario presione enter.
}

static inline void codeError(struct player *player, int code, int ship_index, int row, int column) {
    switch(code) {
        case INVALID_INPUT:
        printf_color(ERROR_COLOR, "Entrada vacia invalida.\n");
        break;
        case INVALID_FORMAT:
        printf_color(ERROR_COLOR, "Formato de entrada invalido.\n");
        break;
        case OUT_OF_RANGE:
        if (player->prevCard == 3 || player->prevCard == 6) {
            printf_color(ERROR_COLOR, "Fila fuera de rango.\n");
        } else if (player->prevCard == 4 || player->prevCard == 7) {
            printf_color(ERROR_COLOR, "Columna fuera de rango.\n");
        }
        if (row < 0 || row >= BOARD_SIZE) {
            printf_color(ERROR_COLOR, "Coordenada fuera de rango(");
            printf_color(INFO_COLOR, "Fila");
            printf_color(ERROR_COLOR, ")");
        }
        if (column < 0 || column >= BOARD_SIZE) {
            printf_color(ERROR_COLOR, "Coordenada fuera de rango(");
            printf_color(INFO_COLOR, "Columna");
            printf_color(ERROR_COLOR, ")");
        }
        break;
        case INVALID_DIRECTION:
        printf_color(ERROR_COLOR,"Orientacion no permitida, el barco debe ser horizontal o vertical.");
        break;
        case INVALID_DIMENSION:
        printf_color(ERROR_COLOR,"Las coordenadas no son validas. El barco debe tener"); 
        printf_color(INFO_COLOR," %d", player->ships[ship_index].ship_size);
        printf_color(ERROR_COLOR," celdas de longitud.", player->ships[ship_index].ship_size);
        break;
        case OVERLAP_DETECTED:
        printf_color(ERROR_COLOR,"Solapamiento detectado en");
        printf_color(INFO_COLOR," %d,%d", row, column);
        printf_color(DEFAULT_COLOR," con el barco %d", ship_index + 1);
        break;
        case PREVIOUSLY_SHOT:
        printf_color(ERROR_COLOR,"Casilla ya disparada previamente");
        break;
    }
    printf_color(INFO_COLOR, " CodeError: %d\n", code);
    printf_color(ERROR_COLOR, "Intente de nuevo.\n");
}

#endif