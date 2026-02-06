#include "utils/bs_error_handler.h"
#include "utils/bs_console.h"
#include <time.h>

jmp_buf bs_error_jump_buffer;
ErrorCode bs_last_error = BS_ERROR_NONE;

static const char* error_messages[] = {
    "Sin error",
    "Error de asignacion de memoria",
    "Parametro nulo invalido",
    "Parametro invalido",
    "Archivo no encontrado",
    "Error al duplicar cadena (strdup fallo)"
};

// Obtener timestamp formateado
static void get_time_stamp(char* buffer, size_t size) {
    time_t now;
    time(&now);
    struct tm* local_time = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", local_time);
}

void bs_error_log(ErrorCode code, const char* context) {
    char time_str[20];
    get_time_stamp(time_str, sizeof(time_str));
    
    // Construir el mensaje completo
    char full_message[512];
    if (context != NULL) {
        snprintf(full_message, sizeof(full_message), 
                "[%s] ERROR: %s - %s\n", 
                time_str, error_messages[code], context);
    } else {
        snprintf(full_message, sizeof(full_message), 
                "[%s] ERROR: %s\n", 
                time_str, error_messages[code]);
    }
    
    // Imprimir a stderr con color (la palabra ERROR se coloreará automáticamente de rojo)
    HANDLE hStderr = GetStdHandle(STD_ERROR_HANDLE);
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Temporalmente usar stderr para el error
    SetStdHandle(STD_OUTPUT_HANDLE, hStderr);
    printf_colorW("%s", full_message);
    SetStdHandle(STD_OUTPUT_HANDLE, hStdout);
}

const char* bs_error_get_message(ErrorCode code) {
    if (code >= BS_ERROR_NONE && code < sizeof(error_messages)/sizeof(error_messages[0])) {
        return error_messages[code];
    }
    return "Codigo de error desconocido";
}