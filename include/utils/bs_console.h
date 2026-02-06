#ifndef BS_CONSOLE_H
#define BS_CONSOLE_H

#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/*  ===================================
    DEFINICIONES DE COLORES BASE
    =================================== */
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

/*  ===================================
    COLORES SEMANTICOS DEL JUEGO
    =================================== */
#define WATER_COLOR      AZUL_CLARO
#define ERROR_COLOR      ROJO
#define INFO_COLOR       AMARILLO
#define SHIP_COLOR       BLANCO
#define SUCCESS_COLOR    VERDE
#define HIGHLIGHT_COLOR  AMARILLO_C
#define INV_COLOR        NEGRO
#define DEFAULT_COLOR    -1

/*  ===================================
    CONFIGURACION DE KEYWORDS
    =================================== */
typedef struct {
    int color;
    const char** keywords;
    int keyword_count;
} ColorKeywordGroup;


// Keywords para WATER_COLOR
static const char* WATER_KEYWORDS[] = {
    "agua", "water"
};

// Keywords para ERROR_COLOR
static const char* ERROR_KEYWORDS[] = {
    "error", "fallo", "failed", "incorrecto", "incorrect", "invalid", "invalido"
};

static const char* INFO_KEYWORDS[] = {
    "info", "informacion", "information", "nota", "note", "aviso", "notice"
};

// Keywords para SHIP_COLOR
static const char* SHIP_KEYWORDS[] = {
    "barco", "ship", "nave", "vessel", "flota", "fleet", "buque", "crucero"
};

// Keywords para SUCCESS_COLOR
static const char* SUCCESS_KEYWORDS[] = {
    "exito", "success", "impacto", "hit", "acierto", "correcto", "correct", "victoria", "victory", "ganaste", "won"
};

// Keywords para HIGHLIGHT_COLOR
static const char* HIGHLIGHT_KEYWORDS[] = {
    "carta", "card", "enter", "turno", "turn", "jugador", "player",
    "coordenadas", "coordinates", "disparar", "shoot", "escanear", "scan"
};

// Mapa global de colores
static ColorKeywordGroup COLOR_MAP[] = {
    {WATER_COLOR, WATER_KEYWORDS, sizeof(WATER_KEYWORDS) / sizeof(WATER_KEYWORDS[0])},
    {ERROR_COLOR, ERROR_KEYWORDS, sizeof(ERROR_KEYWORDS) / sizeof(ERROR_KEYWORDS[0])},
    {INFO_COLOR, INFO_KEYWORDS, sizeof(INFO_KEYWORDS) / sizeof(INFO_KEYWORDS[0])},
    {SHIP_COLOR, SHIP_KEYWORDS, sizeof(SHIP_KEYWORDS) / sizeof(SHIP_KEYWORDS[0])},
    {SUCCESS_COLOR, SUCCESS_KEYWORDS, sizeof(SUCCESS_KEYWORDS) / sizeof(SUCCESS_KEYWORDS[0])},
    {HIGHLIGHT_COLOR, HIGHLIGHT_KEYWORDS, sizeof(HIGHLIGHT_KEYWORDS) / sizeof(HIGHLIGHT_KEYWORDS[0])}
};

static const int COLOR_MAP_SIZE = sizeof(COLOR_MAP) / sizeof(COLOR_MAP[0]);

/* ========================================
   FUNCIONES AUXILIARES PRIVADAS
   ======================================== */
// Obtener el ancho de la consola
static inline int get_console_width(void) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    return 120; // Ancho por defecto
}

// Convertir texto a minúsculas
static inline void to_lowercase_buffer(char* dest, const char* src, size_t max_len) {
    size_t i = 0;
    while (src[i] != '\0' && i < max_len - 1) {
        dest[i] = tolower((unsigned char)src[i]);
        i++;
    }
    dest[i] = '\0';
}

// Verificar si una posición inicia con una keyword
static inline bool match_keyword_at(const char* text, int* out_color, int* out_length) {
    char lower_buffer[256];
    to_lowercase_buffer(lower_buffer, text, sizeof(lower_buffer));
    
    for (int i = 0; i < COLOR_MAP_SIZE; i++) {
        ColorKeywordGroup* group = &COLOR_MAP[i];
        
        for (int j = 0; j < group->keyword_count; j++) {
            const char* keyword = group->keywords[j];
            size_t kw_len = strlen(keyword);
            
            if (strncmp(lower_buffer, keyword, kw_len) == 0) {
                // Verificar que sea una palabra completa
                char next_char = text[kw_len];
                if (next_char == '\0' || isspace(next_char) || ispunct(next_char)) {
                    *out_color = group->color;
                    *out_length = (int)kw_len;
                    return true;
                }
            }
        }
    }
    
    return false;
}

// Obtener la longitud de la palabra actual
static inline int get_word_length(const char* text) {
    int len = 0;
    while (text[len] != '\0' && !isspace(text[len]) && text[len] != '\n') {
        len++;
    }
    return len;
}

// Formateador de color interno (lo que antes era printf_color)
static inline void format_color(int text_hex, int bg_hex, const char* format, ...) {
    if (text_hex != DEFAULT_COLOR) {
        printf("\033[38;2;%d;%d;%dm", 
               (text_hex >> 16) & 0xFF, 
               (text_hex >> 8) & 0xFF, 
               text_hex & 0xFF);
    }

    if (bg_hex != DEFAULT_COLOR) {
        printf("\033[48;2;%d;%d;%dm", 
               (bg_hex >> 16) & 0xFF, 
               (bg_hex >> 8) & 0xFF, 
               bg_hex & 0xFF);
    }

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("\033[0m"); // Restaurar colores
}

/* ========================================
   FUNCIÓN PRINCIPAL: printf_colorW
   ======================================== */
static inline void printf_colorW(const char* format, ...) {
    // Obtener el texto formateado
    char buffer[4096];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    int console_width = get_console_width();
    int current_line_length = 0;
    const char* ptr = buffer;
    
    while (*ptr != '\0') {
        int keyword_color = DEFAULT_COLOR;
        int keyword_length = 0;
        
        // Verificar si hay una keyword
        if (match_keyword_at(ptr, &keyword_color, &keyword_length)) {
            // Verificar si cabe en la línea actual
            if (current_line_length + keyword_length > console_width - 1) {
                printf("\n");
                current_line_length = 0;
            }
            
            // Imprimir keyword con color
            format_color(keyword_color, DEFAULT_COLOR, "%.*s", keyword_length, ptr);
            ptr += keyword_length;
            current_line_length += keyword_length;
        } 
        else if (*ptr == '\n') {
            // Salto de línea explícito
            printf("\n");
            ptr++;
            current_line_length = 0;
        }
        else if (isspace(*ptr)) {
            // Espacio
            if (current_line_length >= console_width - 1) {
                printf("\n");
                current_line_length = 0;
            } else {
                printf("%c", *ptr);
                current_line_length++;
            }
            ptr++;
        }
        else {
            // Palabra normal
            int word_length = get_word_length(ptr);
            
            // Verificar si la palabra cabe en la línea actual
            if (current_line_length > 0 && current_line_length + word_length > console_width - 1) {
                printf("\n");
                current_line_length = 0;
            }
            
            // Imprimir palabra
            printf("%.*s", word_length, ptr);
            ptr += word_length;
            current_line_length += word_length;
        }
    }
    
    fflush(stdout);
}

/* ========================================
            FUNCIONES DE UTILIDAD
   ======================================== */
static inline void set_CP_config(int page) {
    SetConsoleCP(page);
    SetConsoleOutputCP(page);
}

static inline void clear_screen(void) {
    system("cls");
}

static inline void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static inline void pause_timer(float seconds) {
    Sleep((DWORD)(seconds * 1000));
}

static inline void enter_to_message(const char *message, bool clear_screen_after) {
    printf_colorW("Presione enter para %s\n", message);
    while (getchar() != '\n'); // Esperar a que el usuario presione enter.
    if (clear_screen_after) clear_screen();
}

// REFACTORIZADA: Mantiene lógica de borrado de líneas
static inline void return_line(int line_qty) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int width = 120;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    
    for (int i = 0; i < line_qty; i++) {
        printf("\033[A\r%*s\r", width, "");
    }
    fflush(stdout);
}

#endif // BS_CONSOLE_H