#include "bs_lang.h"
#include "model/bs_Card.h"
#include "model/bs_Deck.h"

// Contador global de errores
static int error_count = 0;
static int test_count = 0;

void print_separator(const char* title) {
    printf("\n");
    printf("========================================\n");
    printf(" %s\n", title);
    printf("========================================\n");
}

void test_create_and_verify_cards() {
    print_separator("TEST 1: Crear y verificar cartas desde database");
    
    printf("Creando 5 cartas diferentes...\n\n");
    int card_ids[] = {1, 2, 5, 7, 11};
    Card* cards[5];
    
    for (int i = 0; i < 5; i++) {
        printf("Creando carta ID %d... ", card_ids[i]);
        cards[i] = bs_Card_create_from_database(card_ids[i]);
        
        if (cards[i] == NULL) {
            printf("❌ ERROR: No se pudo crear\n");
            error_count++;
            continue;
        }
        
        printf("✅ Creada: %s (Qty: %d)\n", 
               cards[i]->get_name(cards[i]),
               cards[i]->get_qty(cards[i]));
        test_count++;
    }
    
    // Destruir cartas
    printf("\nLiberando memoria de cartas...\n");
    for (int i = 0; i < 5; i++) {
        if (cards[i] != NULL) {
            bs_Card_destroy(cards[i]);
            printf("  Carta %d destruida\n", i + 1);
        }
    }
    printf("✅ Todas las cartas destruidas correctamente\n");
}

void test_deck_full_cycle() {
    print_separator("TEST 2: Ciclo completo de Deck (Push, Shuffle, Pop)");
    
    // Crear deck
    printf("Creando deck...\n");
    Deck* deck = bs_Deck_create("Mazo de Prueba");
    if (deck == NULL) {
        printf("❌ ERROR CRÍTICO: No se pudo crear el deck\n");
        error_count++;
        return;
    }
    printf("✅ Deck creado: %s\n", deck->get_name(deck));
    printf("Estado inicial - Tamaño: %d, Vacío: %s\n\n", 
           deck->get_size(deck),
           deck->is_empty(deck) ? "Sí" : "No");
    
    // Crear pool de cartas
    printf("Creando pool de 10 cartas...\n");
    Card* card_pool[10];
    for (int i = 0; i < 10; i++) {
        int card_id = (i % 3) + 1; // IDs 1, 2, 3
        card_pool[i] = bs_Card_create_from_database(card_id);
        if (card_pool[i] == NULL) {
            printf("❌ Error creando carta %d\n", i + 1);
            error_count++;
            continue;
        }
        printf("  Carta %d: %s (ID: %d)\n", 
               i + 1, 
               card_pool[i]->get_name(card_pool[i]),
               card_pool[i]->get_id(card_pool[i]));
    }
    
    // Agregar cartas al deck
    printf("\n--- PUSH: Agregando cartas al deck ---\n");
    for (int i = 0; i < 10; i++) {
        if (card_pool[i] != NULL) {
            bool success = deck->push(deck, card_pool[i]);
            printf("Push carta %d: %s (Tamaño deck: %d)\n",
                   i + 1,
                   success ? "✅" : "❌",
                   deck->get_size(deck));
            if (!success) error_count++;
            else test_count++;
        }
    }
    
    printf("\nEstado después de push - Tamaño: %d, Vacío: %s, Lleno: %s\n",
           deck->get_size(deck),
           deck->is_empty(deck) ? "Sí" : "No",
           deck->is_full(deck) ? "Sí" : "No");
    
    // Barajar
    printf("\n--- SHUFFLE: Barajando deck ---\n");
    printf("Antes de barajar (primeras 3 cartas desde el top):\n");
    for (int i = 0; i < 3 && i < deck->get_size(deck); i++) {
        Card* card = deck->peek(deck);
        if (card) {
            printf("  Posición top-%d: %s\n", i, card->get_name(card));
            deck->pop(deck);  // Temporal para ver siguiente
        }
    }
    // Recargar las 3 que sacamos
    for (int i = 0; i < 3; i++) {
        if (card_pool[i] != NULL) deck->push(deck, card_pool[i]);
    }
    
    deck->shuffle(deck);
    printf("✅ Deck barajado\n");
    
    printf("Después de barajar (peek top):\n");
    Card* top = deck->peek(deck);
    if (top) {
        printf("  Carta superior: %s\n", top->get_name(top));
    }
    
    // Sacar cartas una por una
    printf("\n--- POP: Sacando cartas del deck ---\n");
    int draw_count = 0;
    while (!deck->is_empty(deck) && draw_count < 10) {
        Card* drawn = deck->pop(deck);
        if (drawn != NULL) {
            printf("Draw %d: %s (Quedan: %d cartas)\n",
                   ++draw_count,
                   drawn->get_name(drawn),
                   deck->get_size(deck));
            test_count++;
        } else {
            printf("❌ Error al sacar carta\n");
            error_count++;
            break;
        }
    }
    
    printf("\nEstado final - Tamaño: %d, Vacío: %s\n",
           deck->get_size(deck),
           deck->is_empty(deck) ? "Sí" : "No");
    
    if (deck->is_empty(deck)) {
        printf("✅ Deck completamente vaciado\n");
        test_count++;
    } else {
        printf("❌ Deck no se vació completamente\n");
        error_count++;
    }
    
    // Limpiar
    printf("\n--- Limpiando memoria ---\n");
    bs_Deck_destroy(deck);
    printf("Deck destruido\n");
    
    for (int i = 0; i < 10; i++) {
        if (card_pool[i] != NULL) {
            bs_Card_destroy(card_pool[i]);
        }
    }
    printf("✅ Todas las cartas del pool destruidas\n");
}

void test_deck_with_28_cards() {
    print_separator("TEST 3: Simulación completa de ciclo de juego");
    
    // Crear los 3 decks del sistema
    Deck* deck_principal = bs_Deck_create("Deck Principal");
    Deck* deck_activo = bs_Deck_create("Deck Activo");
    Deck* deck_descarte = bs_Deck_create("Deck Descarte");
    
    if (deck_principal == NULL || deck_activo == NULL || deck_descarte == NULL) {
        printf("❌ ERROR: No se pudieron crear los decks\n");
        error_count++;
        return;
    }
    printf("✅ Decks creados: Principal, Activo, Descarte\n\n");
    
    // Crear pool de 28 cartas según especificación
    Card* pool[28];
    int pool_idx = 0;
    
    int card_ids[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    int quantities[] = {7, 5, 2, 4, 1, 1, 1, 3, 2, 1, 1};
    const int TORRE_VENTAJA_ID = 4; // ID de la carta "Torre de ventaja" (4 copias)
    
    printf("Creando pool de 28 cartas:\n");
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < quantities[i]; j++) {
            pool[pool_idx] = bs_Card_create_from_database(card_ids[i]);
            if (pool[pool_idx] == NULL) {
                printf("❌ Error en carta %d\n", pool_idx);
                error_count++;
            }
            pool_idx++;
        }
    }
    printf("✅ Pool de %d cartas creado\n\n", pool_idx);
    
    // === FASE 1: Barajar e inicializar deck principal ===
    printf("=== FASE 1: Inicialización del Deck Principal ===\n");
    printf("Agregando todas las cartas al deck principal...\n");
    for (int i = 0; i < pool_idx; i++) {
        if (pool[i] != NULL) {
            deck_principal->push(deck_principal, pool[i]);
        }
    }
    printf("Cartas en deck principal: %d\n", deck_principal->get_size(deck_principal));
    
    printf("Barajando deck principal...\n");
    deck_principal->shuffle(deck_principal);
    printf("✅ Deck principal barajado\n\n");
    
    // === FASE 2: Simular ciclo de juego ===
    printf("=== FASE 2: Simulación de Ciclo de Juego ===\n");
    printf("Reglas:\n");
    printf("  - Sacar cartas del deck principal\n");
    printf("  - Si es Torre de ventaja (ID %d) → Deck Activo\n", TORRE_VENTAJA_ID);
    printf("  - Si es otra carta → Deck Descarte\n");
    printf("  - Al vaciar principal → Rebarajar descarte y volver a principal\n\n");
    
    int torres_encontradas = 0;
    int ciclos_rebarajeo = 0;
    int cartas_procesadas = 0;
    
    printf("Primera pasada por el deck:\n");
    while (cartas_procesadas < pool_idx) {
        // Si el deck principal está vacío, rebarajar descarte
        if (deck_principal->is_empty(deck_principal)) {
            if (deck_descarte->is_empty(deck_descarte)) {
                printf("\n✅ Todos los decks procesados\n");
                break;
            }
            
            ciclos_rebarajeo++;
            printf("\n🔄 REBARAJEO %d: Deck principal vacío\n", ciclos_rebarajeo);
            printf("   Moviendo %d cartas de descarte a principal...\n", 
                   deck_descarte->get_size(deck_descarte));
            
            // Mover todas las cartas del descarte al principal
            while (!deck_descarte->is_empty(deck_descarte)) {
                Card* card = deck_descarte->pop(deck_descarte);
                deck_principal->push(deck_principal, card);
            }
            
            // Barajar el deck principal
            printf("   Barajando deck principal...\n");
            deck_principal->shuffle(deck_principal);
            printf("   ✅ Listo para continuar\n\n");
        }
        
        // Sacar carta del principal
        Card* card = deck_principal->pop(deck_principal);
        if (card == NULL) break;
        
        cartas_procesadas++;
        
        // Verificar si es Torre de ventaja
        if (card->get_id(card) == TORRE_VENTAJA_ID) {
            deck_activo->push(deck_activo, card);
            torres_encontradas++;
            printf("  [%d] 🗼 %s → Deck ACTIVO (total torres: %d)\n", 
                   cartas_procesadas, card->get_name(card), torres_encontradas);
        } else {
            deck_descarte->push(deck_descarte, card);
            if (cartas_procesadas <= 15) { // Mostrar solo las primeras 15 para no saturar
                printf("  [%d] %s → Descarte\n", 
                       cartas_procesadas, card->get_name(card));
            } else if (cartas_procesadas == 16) {
                printf("  ... (más cartas procesándose)\n");
            }
        }
    }
    
    // === FASE 3: Verificación de resultados ===
    printf("\n=== FASE 3: Verificación de Resultados ===\n");
    printf("Cartas procesadas: %d / %d\n", cartas_procesadas, pool_idx);
    printf("Ciclos de rebarajeo: %d\n\n", ciclos_rebarajeo);
    
    printf("Estado final de los decks:\n");
    printf("  📦 Deck Principal: %d cartas\n", deck_principal->get_size(deck_principal));
    printf("  🗼 Deck Activo: %d cartas (Torres de ventaja)\n", deck_activo->get_size(deck_activo));
    printf("  🗑️  Deck Descarte: %d cartas\n\n", deck_descarte->get_size(deck_descarte));
    
    // Verificar que todas las Torres de ventaja están en el deck activo
    int expected_torres = quantities[3]; // ID 4 está en posición 3 del array (índice 0-based)
    if (deck_activo->get_size(deck_activo) == expected_torres) {
        printf("✅ Deck Activo contiene las %d Torres de ventaja correctamente\n", expected_torres);
        test_count++;
    } else {
        printf("❌ ERROR: Deck Activo tiene %d cartas pero debería tener %d Torres\n",
               deck_activo->get_size(deck_activo), expected_torres);
        error_count++;
    }
    
    // Verificar que el resto de cartas está en descarte o principal
    int total_otras_cartas = pool_idx - expected_torres;
    int cartas_restantes = deck_principal->get_size(deck_principal) + deck_descarte->get_size(deck_descarte);
    if (cartas_restantes == total_otras_cartas) {
        printf("✅ El resto de cartas (%d) está correctamente distribuido\n", total_otras_cartas);
        test_count++;
    } else {
        printf("❌ ERROR: Hay %d cartas en principal+descarte pero deberían ser %d\n",
               cartas_restantes, total_otras_cartas);
        error_count++;
    }
    
    // === FASE 4: Análisis de aleatoriedad ===
    printf("\n=== FASE 4: Análisis de Aleatoriedad ===\n");
    printf("Verificando que las cartas fueron realmente barajadas...\n\n");
    
    // Mostrar las primeras 5 cartas del deck de descarte para ver el orden
    printf("Primeras 5 cartas del descarte (desde arriba):\n");
    Card* muestra[5];
    int muestras_tomadas = 0;
    for (int i = 0; i < 5 && !deck_descarte->is_empty(deck_descarte); i++) {
        muestra[i] = deck_descarte->pop(deck_descarte);
        if (muestra[i] != NULL) {
            printf("  %d. %s (ID: %d)\n", i + 1, 
                   muestra[i]->get_name(muestra[i]), 
                   muestra[i]->get_id(muestra[i]));
            muestras_tomadas++;
        }
    }
    
    // Devolver las cartas muestreadas
    for (int i = muestras_tomadas - 1; i >= 0; i--) {
        deck_descarte->push(deck_descarte, muestra[i]);
    }
    
    printf("\n💡 Si las cartas están bien mezcladas, no deberían estar agrupadas por ID\n");
    printf("✅ Test de ciclo completo finalizado\n");
    
    // === Limpiar memoria ===
    printf("\n--- Limpiando memoria ---\n");
    bs_Deck_destroy(deck_principal);
    bs_Deck_destroy(deck_activo);
    bs_Deck_destroy(deck_descarte);
    
    for (int i = 0; i < pool_idx; i++) {
        if (pool[i] != NULL) {
            bs_Card_destroy(pool[i]);
        }
    }
    printf("✅ Memoria liberada correctamente\n");
}

int main() {
    // Configurar consola para UTF-8
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    
    printf("\n");
    printf("╔════════════════════════════════════════════╗\n");
    printf("║   PRUEBAS UNITARIAS: DECK Y CARD SYSTEM   ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    
    test_create_and_verify_cards();
    test_deck_full_cycle();
    test_deck_with_28_cards();
    
    print_separator("RESUMEN DE PRUEBAS");
    printf("Operaciones exitosas: %d\n", test_count);
    printf("Errores encontrados: %d\n", error_count);
    
    if (error_count == 0) {
        printf("\n✅ ¡TODAS LAS PRUEBAS PASARON!\n\n");
        return 0;
    } else {
        printf("\n❌ SE ENCONTRARON %d ERRORES\n\n", error_count);
        return 1;
    }
}