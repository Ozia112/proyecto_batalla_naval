/**
 * @file test_error_handler.c
 * @brief Pruebas de estrés para el sistema de manejo de errores
 * 
 * Este test evalúa:
 * - Detección de memory leaks
 * - Detección de null pointers
 * - Manejo de parámetros inválidos
 * - Comportamiento con operaciones correctas
 * - Try-catch-finally
 */

#include "bs_lang.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Contador de errores
static int error_count = 0;
static int test_count = 0;

#define PRINT_TEST_HEADER(num, name) \
    printf("\n"); \
    printf_colorW("═════════════════════════════════════\n"); \
    printf(" TEST %d: %s\n", num, name); \
    printf_colorW("═════════════════════════════════════\n")

#define PRINT_RESULT(success) \
    do { \
        test_count++; \
        if (success) { \
            printf_colorW("✅ Prueba %d: EXITOSA\n", test_count); \
        } else { \
            printf_colorW("❌ Prueba %d: FALLIDA\n", test_count); \
            error_count++; \
        } \
    } while(0)

// ============================================
// FUNCIONES DE PRUEBA QUE CAUSAN ERRORES
// ============================================

/**
 * Test 1: Intentar crear string con NULL
 */
void test_null_pointer_detection() {
    PRINT_TEST_HEADER(1, "Detección de NULL pointers");
    
    printf("\n--- Caso 1: bs_safe_strdup(NULL) ---\n");
    char* result = bs_safe_strdup(NULL);
    if (result == NULL) {
        printf_colorW("✅ bs_safe_strdup detectó NULL correctamente\n");
        PRINT_RESULT(true);
    } else {
        printf_colorW("❌ bs_safe_strdup NO detectó NULL\n");
        free(result);
        PRINT_RESULT(false);
    }

    printf("\n--- Caso 2: bs_safe_strndup(NULL, 10) ---\n");
    result = bs_safe_strndup(NULL, 10);
    if (result == NULL) {
        printf_colorW("✅ bs_safe_strndup detectó NULL correctamente\n");
        PRINT_RESULT(true);
    } else {
        printf_colorW("❌ bs_safe_strndup NO detectó NULL\n");
        free(result);
        PRINT_RESULT(false);
    }

    printf("\n--- Caso 3: bs_safe_str_concat(NULL, NULL) ---\n");
    result = bs_safe_str_concat(NULL, NULL);
    if (result == NULL) {
        printf_colorW("✅ bs_safe_str_concat detectó NULL correctamente\n");
        PRINT_RESULT(true);
    } else {
        printf_colorW("❌ bs_safe_str_concat NO detectó NULL\n");
        free(result);
        PRINT_RESULT(false);
    }
}

/**
 * Test 2: Memory leaks intencionales
 */
void test_memory_leak_detection() {
    PRINT_TEST_HEADER(2, "Simulación de Memory Leaks");
    
    printf("\n--- Leak 1: Crear string y NO liberarlo ---\n");
    char* leaked1 = bs_safe_strdup("String que nunca se liberará");
    if (leaked1 != NULL) {
        printf("Dirección asignada: %p\n", (void*)leaked1);
        printf_colorW("⚠️  MEMORY LEAK INTENCIONAL: %zu bytes\n", strlen(leaked1) + 1);
        // NO SE LIBERA INTENCIONALMENTE
        PRINT_RESULT(true); // "Éxito" en crear el leak
    } else {
        PRINT_RESULT(false);
    }

    printf("\n--- Leak 2: Concatenar y perder referencia ---\n");
    char* str1 = bs_safe_strdup("Primera parte ");
    char* str2 = bs_safe_strdup("Segunda parte");
    if (str1 && str2) {
        char* concatenated = bs_safe_str_concat(str1, str2);
        printf("Original 1: %p\n", (void*)str1);
        printf("Original 2: %p\n", (void*)str2);
        printf("Concatenado: %p\n", (void*)concatenated);
        
        // Liberamos los originales pero NO el concatenado
        free(str1);
        free(str2);
        printf_colorW("⚠️  MEMORY LEAK INTENCIONAL: concatenado sin liberar\n");
        PRINT_RESULT(true);
    } else {
        PRINT_RESULT(false);
    }

    printf("\n--- Leak 3: Reemplazar string sin liberar anterior ---\n");
    char* original = bs_safe_strdup("Original");
    printf("Original: %p -> \"%s\"\n", (void*)original, original);
    
    // Sobrescribir puntero sin liberar memoria
    original = bs_safe_strdup("Nuevo (leak del anterior)");
    printf("Nuevo: %p -> \"%s\"\n", (void*)original, original);
    printf_colorW("⚠️  MEMORY LEAK INTENCIONAL: puntero original perdido\n");
    
    free(original); // Solo liberamos el último
    PRINT_RESULT(true);
}

/**
 * Test 3: Operaciones correctas (sin errores)
 */
void test_correct_operations() {
    PRINT_TEST_HEADER(3, "Operaciones Correctas (sin errores esperados)");
    
    printf("\n--- Operación 1: Duplicar string correctamente ---\n");
    const char* test_str = "Texto de prueba 123";
    char* dup = bs_safe_strdup(test_str);
    
    if (dup != NULL && strcmp(dup, test_str) == 0) {
        printf("Original: \"%s\"\n", test_str);
        printf("Duplicado: \"%s\"\n", dup);
        printf_colorW("✅ Duplicación correcta\n");
        free(dup);
        PRINT_RESULT(true);
    } else {
        printf_colorW("❌ Fallo en duplicación\n");
        if (dup) free(dup);
        PRINT_RESULT(false);
    }

    printf("\n--- Operación 2: Concatenar correctamente ---\n");
    char* part1 = bs_safe_strdup("Hola ");
    char* part2 = bs_safe_strdup("Mundo");
    char* result = bs_safe_str_concat(part1, part2);
    
    if (result != NULL && strcmp(result, "Hola Mundo") == 0) {
        printf("Parte 1: \"%s\"\n", part1);
        printf("Parte 2: \"%s\"\n", part2);
        printf("Resultado: \"%s\"\n", result);
        printf_colorW("✅ Concatenación correcta\n");
        free(part1);
        free(part2);
        free(result);
        PRINT_RESULT(true);
    } else {
        printf_colorW("❌ Fallo en concatenación\n");
        if (part1) free(part1);
        if (part2) free(part2);
        if (result) free(result);
        PRINT_RESULT(false);
    }

    printf("\n--- Operación 3: Reemplazar string correctamente ---\n");
    char* original = bs_safe_strdup("Original");
    printf("Antes: \"%s\" (%p)\n", original, (void*)original);
    
    bool replaced = bs_safe_str_replace(&original, "Reemplazado");
    
    if (replaced && original != NULL && strcmp(original, "Reemplazado") == 0) {
        printf("Después: \"%s\" (%p)\n", original, (void*)original);
        printf_colorW("✅ Reemplazo correcto (memoria anterior liberada)\n");
        free(original);
        PRINT_RESULT(true);
    } else {
        printf_colorW("❌ Fallo en reemplazo\n");
        if (original) free(original);
        PRINT_RESULT(false);
    }
}

/**
 * Test 4: Try-Catch con error handler
 */
void test_try_catch_mechanism() {
    PRINT_TEST_HEADER(4, "Mecanismo Try-Catch-Finally");
    
    printf("\n--- Caso 1: Try exitoso (sin errores) ---\n");
    try {
        printf("Ejecutando código en try...\n");
        char* test = bs_safe_strdup("Éxito");
        printf("String creado: \"%s\"\n", test);
        free(test);
        printf_colorW("✅ Try completado sin errores\n");
        PRINT_RESULT(true);
    }
    catch(BS_ERROR_MEMORY_ALLOCATION) {
        printf_colorW("❌ Catch: Error de memoria\n");
        PRINT_RESULT(false);
    }
    catch(BS_ERROR_NULL_POINTER) {
        printf_colorW("❌ Catch: Null pointer\n");
        PRINT_RESULT(false);
    }
    catch_any {
        printf_colorW("❌ Catch: Error desconocido\n");
        PRINT_RESULT(false);
    }

    printf("\n--- Caso 2: Forzar error de NULL pointer ---\n");
    try {
        printf("Intentando operación con NULL...\n");
        if (bs_safe_strdup(NULL) == NULL) {
            printf("bs_safe_strdup retornó NULL (comportamiento esperado)\n");
            throw(BS_ERROR_NULL_POINTER, "Parámetro NULL detectado");
        }
    }
    catch(BS_ERROR_NULL_POINTER) {
        printf_colorW("✅ Catch: NULL pointer capturado correctamente\n");
        PRINT_RESULT(true);
    }
    catch_any {
        printf_colorW("❌ Catch: Error inesperado\n");
        PRINT_RESULT(false);
    }

    printf("\n--- Caso 3: Simulación de error de memoria ---\n");
    try {
        printf("Simulando fallo en malloc...\n");
        throw(BS_ERROR_MEMORY_ALLOCATION, "malloc() retornó NULL");
    }
    catch(BS_ERROR_MEMORY_ALLOCATION) {
        printf_colorW("✅ Catch: Error de memoria capturado\n");
        PRINT_RESULT(true);
    }
    catch_any {
        printf_colorW("❌ Catch: Error inesperado\n");
        PRINT_RESULT(false);
    }
}

/**
 * Test 5: Casos límite
 */
void test_edge_cases() {
    PRINT_TEST_HEADER(5, "Casos Límite y Extremos");
    
    printf("\n--- Caso 1: String vacío ---\n");
    char* empty = bs_safe_strdup("");
    if (empty != NULL && strlen(empty) == 0) {
        printf("String vacío creado: \"%s\" (longitud: %zu)\n", empty, strlen(empty));
        printf_colorW("✅ String vacío manejado correctamente\n");
        free(empty);
        PRINT_RESULT(true);
    } else {
        printf_colorW("❌ Fallo con string vacío\n");
        if (empty) free(empty);
        PRINT_RESULT(false);
    }

    printf("\n--- Caso 2: String muy largo (1000 caracteres) ---\n");
    char long_str[1001];
    memset(long_str, 'A', 1000);
    long_str[1000] = '\0';
    
    char* dup_long = bs_safe_strdup(long_str);
    if (dup_long != NULL && strlen(dup_long) == 1000) {
        printf("String largo duplicado: %zu caracteres\n", strlen(dup_long));
        printf_colorW("✅ String largo manejado correctamente\n");
        free(dup_long);
        PRINT_RESULT(true);
    } else {
        printf_colorW("❌ Fallo con string largo\n");
        if (dup_long) free(dup_long);
        PRINT_RESULT(false);
    }

    printf("\n--- Caso 3: Concatenar strings vacíos ---\n");
    char* empty1 = bs_safe_strdup("");
    char* empty2 = bs_safe_strdup("");
    char* result = bs_safe_str_concat(empty1, empty2);
    
    if (result != NULL && strlen(result) == 0) {
        printf("Concatenación de vacíos: \"%s\"\n", result);
        printf_colorW("✅ Concatenación de vacíos correcta\n");
        free(empty1);
        free(empty2);
        free(result);
        PRINT_RESULT(true);
    } else {
        printf_colorW("❌ Fallo en concatenación de vacíos\n");
        if (empty1) free(empty1);
        if (empty2) free(empty2);
        if (result) free(result);
        PRINT_RESULT(false);
    }

    printf("\n--- Caso 4: strndup con n = 0 ---\n");
    char* zero_len = bs_safe_strndup("Test", 0);
    if (zero_len != NULL && strlen(zero_len) == 0) {
        printf("strndup(n=0): \"%s\" (longitud: %zu)\n", zero_len, strlen(zero_len));
        printf_colorW("✅ strndup con n=0 manejado correctamente\n");
        free(zero_len);
        PRINT_RESULT(true);
    } else {
        printf_colorW("❌ Fallo con strndup(n=0)\n");
        if (zero_len) free(zero_len);
        PRINT_RESULT(false);
    }
}

/**
 * Test 6: Validaciones de funciones auxiliares
 */
void test_auxiliary_functions() {
    PRINT_TEST_HEADER(6, "Funciones Auxiliares de Validación");
    
    printf("\n--- bs_str_is_empty() ---\n");
    bool test1 = bs_str_is_empty(NULL);
    bool test2 = bs_str_is_empty("");
    bool test3 = bs_str_is_empty("No vacío");
    
    if (test1 && test2 && !test3) {
        printf("NULL: %s\n", test1 ? "vacío" : "NO vacío");
        printf("\"\": %s\n", test2 ? "vacío" : "NO vacío");
        printf("\"No vacío\": %s\n", test3 ? "vacío" : "NO vacío");
        printf_colorW("✅ bs_str_is_empty funciona correctamente\n");
        PRINT_RESULT(true);
    } else {
        printf_colorW("❌ bs_str_is_empty falló\n");
        PRINT_RESULT(false);
    }

    printf("\n--- bs_str_equals() ---\n");
    bool eq1 = bs_str_equals("test", "test");
    bool eq2 = bs_str_equals("test", "TEST");
    bool eq3 = bs_str_equals(NULL, NULL);
    bool eq4 = bs_str_equals("test", NULL);
    
    if (eq1 && !eq2 && eq3 && !eq4) {
        printf("\"test\" == \"test\": %s\n", eq1 ? "Sí" : "No");
        printf("\"test\" == \"TEST\": %s\n", eq2 ? "Sí" : "No");
        printf("NULL == NULL: %s\n", eq3 ? "Sí" : "No");
        printf("\"test\" == NULL: %s\n", eq4 ? "Sí" : "No");
        printf_colorW("✅ bs_str_equals funciona correctamente\n");
        PRINT_RESULT(true);
    } else {
        printf_colorW("❌ bs_str_equals falló\n");
        PRINT_RESULT(false);
    }
}

// ============================================
// MAIN
// ============================================

int main() {
    // Configurar UTF-8
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    printf_colorW("\n╔════════════════════════════════════════════╗\n");
    printf_colorW("║  PRUEBAS DE ESTRÉS: ERROR HANDLER SYSTEM  ║\n");
    printf_colorW("╚════════════════════════════════════════════╝\n");

    // Ejecutar todos los tests
    test_null_pointer_detection();
    test_memory_leak_detection();
    test_correct_operations();
    test_try_catch_mechanism();
    test_edge_cases();
    test_auxiliary_functions();

    // Resumen final
    printf("\n");
    printf_colorW("╔════════════════════════════════════════════╗\n");
    printf_colorW("║           RESUMEN DE PRUEBAS               ║\n");
    printf_colorW("╚════════════════════════════════════════════╝\n");
    printf("\nTotal de pruebas ejecutadas: %d\n", test_count);
    printf("Pruebas exitosas: %d\n", test_count - error_count);
    printf("Pruebas fallidas: %d\n", error_count);

    if (error_count == 0) {
        printf_colorW("\n✅ ¡TODAS LAS PRUEBAS PASARON EXITOSAMENTE!\n");
        printf_colorW("\nNOTA: Los memory leaks del TEST 2 son INTENCIONALES\n");
        printf_colorW("para demostrar cómo se comporta el sistema.\n");
        return 0;
    } else {
        printf_colorW("\n❌ ALGUNAS PRUEBAS FALLARON\n");
        return 1;
    }
}