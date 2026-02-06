/**
 * ========================================
 * BS_STRING_MNGR.H - Gestión Segura de Strings
 * ========================================
 * 
 * Proporciona funciones seguras para manejo de strings que reemplazan
 * funciones problemáticas de la biblioteca estándar como strdup().
 * 
 * NOTA IMPORTANTE:
 * strdup() causa cuelgues en algunas implementaciones de MinGW/Windows.
 * Este módulo proporciona reemplazos seguros con validación exhaustiva.
 */

#ifndef BS_STRING_MNGR_H
#define BS_STRING_MNGR_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * ========================================
 * bs_safe_strdup - Duplicación segura de cadenas
 * ========================================
 * 
 * Reemplazo seguro de strdup() que evita cuelgues en Windows.
 * 
 * @param src: Cadena fuente a duplicar (puede ser NULL)
 * @return: Puntero a nueva cadena duplicada, o NULL si falla
 * 
 * CARACTERÍSTICAS:
 * - Validación de entrada (NULL-safe)
 * - Asignación manual de memoria (evita strdup problemático)
 * - Copia segura con strcpy (buffer garantizado del tamaño correcto)
 * - Retorno NULL en caso de error (fácil de verificar)
 * 
 * IMPORTANTE: El llamador es responsable de liberar la memoria con free()
 */
static inline char* bs_safe_strdup(const char* src) {
    // Validación de entrada
    if (src == NULL) {
        return NULL;
    }
    
    // Calcular longitud necesaria (incluyendo '\0')
    size_t len = strlen(src);
    
    // Asignar memoria manualmente (evita strdup problemático)
    char* dest = (char*)malloc(len + 1);
    
    // Verificar asignación exitosa
    if (dest == NULL) {
        return NULL;
    }
    
    // Copiar cadena de forma segura
    strcpy(dest, src);
    
    return dest;
}

/**
 * ========================================
 * bs_safe_strndup - Duplicación limitada de cadenas
 * ========================================
 * 
 * Duplica hasta 'n' caracteres de una cadena.
 * 
 * @param src: Cadena fuente
 * @param n: Máximo número de caracteres a copiar
 * @return: Nueva cadena duplicada, o NULL si falla
 */
static inline char* bs_safe_strndup(const char* src, size_t n) {
    if (src == NULL) {
        return NULL;
    }
    
    // Calcular longitud real a copiar
    size_t src_len = strlen(src);
    size_t copy_len = (src_len < n) ? src_len : n;
    
    // Asignar memoria
    char* dest = (char*)malloc(copy_len + 1);
    if (dest == NULL) {
        return NULL;
    }
    
    // Copiar caracteres
    memcpy(dest, src, copy_len);
    dest[copy_len] = '\0';
    
    return dest;
}

/**
 * ========================================
 * bs_safe_str_concat - Concatenación segura
 * ========================================
 * 
 * Concatena dos cadenas creando una nueva.
 * 
 * @param str1: Primera cadena (puede ser NULL)
 * @param str2: Segunda cadena (puede ser NULL)
 * @return: Nueva cadena con la concatenación, o NULL si falla
 */
static inline char* bs_safe_str_concat(const char* str1, const char* str2) {
    // Manejar casos NULL
    if (str1 == NULL && str2 == NULL) {
        return NULL;
    }
    
    if (str1 == NULL) {
        return bs_safe_strdup(str2);
    }
    
    if (str2 == NULL) {
        return bs_safe_strdup(str1);
    }
    
    // Calcular longitud total
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t total_len = len1 + len2;
    
    // Asignar memoria
    char* result = (char*)malloc(total_len + 1);
    if (result == NULL) {
        return NULL;
    }
    
    // Copiar ambas cadenas
    strcpy(result, str1);
    strcpy(result + len1, str2);
    
    return result;
}

/**
 * ========================================
 * bs_safe_str_replace - Reemplazar en cadena existente
 * ========================================
 * 
 * Actualiza un puntero de cadena con una nueva copia.
 * Libera la cadena anterior si existe.
 * 
 * @param dest: Puntero a la cadena destino (será modificado)
 * @param src: Cadena fuente a copiar
 * @return: true si tuvo éxito, false si falló
 * 
 * EJEMPLO:
 *   char* name = NULL;
 *   bs_safe_str_replace(&name, "Juan");  // name = "Juan"
 *   bs_safe_str_replace(&name, "Pedro"); // libera "Juan", name = "Pedro"
 *   free(name); // limpiar al final
 */
static inline bool bs_safe_str_replace(char** dest, const char* src) {
    if (dest == NULL) {
        return false;
    }
    
    // Liberar cadena anterior si existe
    if (*dest != NULL) {
        free(*dest);
        *dest = NULL;
    }
    
    // Si src es NULL, dejar dest como NULL
    if (src == NULL) {
        return true;
    }
    
    // Duplicar nueva cadena
    *dest = bs_safe_strdup(src);
    
    return (*dest != NULL);
}

/**
 * ========================================
 * bs_str_is_empty - Verificar si cadena está vacía
 * ========================================
 * 
 * @param str: Cadena a verificar
 * @return: true si es NULL o vacía ("")
 */
static inline bool bs_str_is_empty(const char* str) {
    return (str == NULL || str[0] == '\0');
}

/**
 * ========================================
 * bs_str_equals - Comparación segura de cadenas
 * ========================================
 * 
 * @param str1: Primera cadena
 * @param str2: Segunda cadena
 * @return: true si ambas son iguales (o ambas NULL)
 */
static inline bool bs_str_equals(const char* str1, const char* str2) {
    // Ambas NULL
    if (str1 == NULL && str2 == NULL) {
        return true;
    }
    
    // Una NULL y otra no
    if (str1 == NULL || str2 == NULL) {
        return false;
    }
    
    // Comparar contenido
    return strcmp(str1, str2) == 0;
}

#endif // BS_STRING_MNGR_H
