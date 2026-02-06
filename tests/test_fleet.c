#include "bs_lang.h"
#include "model/bs_Card.h"
#include "model/bs_Fleet.h"

// Contador global de errores
static int error_count = 0;
static int test_count = 0;

void print_separator(const char* title) {
    printf("\n");
    printf("========================================\n");
    printf(" %s\n", title);
    printf("========================================\n");
}

void test_create_single_ship() {
    print_separator("TEST 1: Crear barco individual desde database");
    
    Ship* ship = bs_Ship_create();
    if (ship == NULL) {
        printf("❌ ERROR: No se pudo crear el barco\n");
        error_count++;
        return;
    }
    printf("✅ Ship creado\n");
    
    // Inicializar con datos de Portaviones
    ship->init(ship, 0, "Portaviones", 5);
    
    printf("\nDatos del barco:\n");
    printf("  ID: %d\n", ship->get_id(ship));
    printf("  Nombre: %s\n", ship->get_name(ship));
    printf("  Tamaño: %d casillas\n", ship->get_size(ship));
    printf("  Colocado: %s\n", ship->get_is_placed(ship) ? "Sí" : "No");
    printf("  Hundido: %s\n", ship->get_is_sunk(ship) ? "Sí" : "No");
    
    // Validaciones
    if (ship->get_id(ship) == 0) {
        printf("✅ ID correcto\n");
        test_count++;
    } else {
        printf("❌ ID incorrecto\n");
        error_count++;
    }
    
    if (ship->get_size(ship) == 5) {
        printf("✅ Tamaño correcto\n");
        test_count++;
    } else {
        printf("❌ Tamaño incorrecto\n");
        error_count++;
    }
    
    if (!ship->get_is_placed(ship) && !ship->get_is_sunk(ship)) {
        printf("✅ Estado inicial correcto\n");
        test_count++;
    } else {
        printf("❌ Estado inicial incorrecto\n");
        error_count++;
    }
    
    // Destruir
    bs_Ship_destroy(ship);
    printf("\n✅ Barco destruido\n");
}

void test_fleet_initialization() {
    print_separator("TEST 2: Inicialización de flota completa");
    
    Fleet* fleet = bs_Fleet_create();
    if (fleet == NULL) {
        printf("❌ ERROR: No se pudo crear la flota\n");
        error_count++;
        return;
    }
    printf("✅ Flota creada\n");
    
    // Inicializar flota (crea los 5 barcos automáticamente)
    fleet->init(fleet);
    printf("✅ Flota inicializada\n\n");
    
    printf("Estado de la flota:\n");
    printf("  Barcos colocados: %d/%d\n", 
           fleet->get_ships_placed(fleet), NUM_SHIPS);
    printf("  Barcos restantes: %d\n", 
           fleet->get_remaining_ships(fleet));
    printf("  Celdas totales: %d\n\n", 
           fleet->get_remaining_cells(fleet));
    
    // Validar contadores iniciales
    if (fleet->get_ships_placed(fleet) == 0) {
        printf("✅ Barcos colocados = 0 (correcto)\n");
        test_count++;
    } else {
        printf("❌ Barcos colocados debería ser 0\n");
        error_count++;
    }
    
    if (fleet->get_remaining_ships(fleet) == 5) {
        printf("✅ Barcos restantes = 5 (correcto)\n");
        test_count++;
    } else {
        printf("❌ Barcos restantes debería ser 5\n");
        error_count++;
    }
    
    if (fleet->get_remaining_cells(fleet) == 17) {
        printf("✅ Celdas totales = 17 (correcto: 5+4+3+3+2)\n");
        test_count++;
    } else {
        printf("❌ Celdas totales debería ser 17\n");
        error_count++;
    }
    
    // Verificar cada barco de la flota
    printf("\n--- Verificando barcos individuales ---\n");
    
    const char* expected_names[] = {"Portaaviones", "Acorazado", "Crucero", "Destructor", "Fragata"};
    int expected_sizes[] = {5, 4, 3, 3, 2};
    
    for (int i = 0; i < NUM_SHIPS; i++) {
        Ship* ship = fleet->get_ship(fleet, i);
        
        if (ship == NULL) {
            printf("❌ Barco %d es NULL\n", i);
            error_count++;
            continue;
        }
        
        printf("\nBarco %d:\n", i + 1);
        printf("  ID: %d\n", ship->get_id(ship));
        printf("  Nombre: %s\n", ship->get_name(ship));
        printf("  Tamaño: %d\n", ship->get_size(ship));
        printf("  Colocado: %s\n", ship->get_is_placed(ship) ? "Sí" : "No");
        
        // Validaciones
        bool id_ok = (ship->get_id(ship) == i);
        bool size_ok = (ship->get_size(ship) == expected_sizes[i]);
        bool not_placed = !ship->get_is_placed(ship);
        bool not_sunk = !ship->get_is_sunk(ship);
        
        if (id_ok && size_ok && not_placed && not_sunk) {
            printf("  ✅ Barco válido\n");
            test_count++;
        } else {
            printf("  ❌ Barco con errores\n");
            if (!id_ok) printf("     - ID incorrecto\n");
            if (!size_ok) printf("     - Tamaño incorrecto\n");
            if (!not_placed) printf("     - Debería estar sin colocar\n");
            if (!not_sunk) printf("     - Debería estar a flote\n");
            error_count++;
        }
    }
    
    // Destruir flota
    printf("\n--- Liberando memoria ---\n");
    bs_Fleet_destroy(fleet);
    printf("✅ Flota destruida correctamente\n");
}

void test_fleet_ship_access() {
    print_separator("TEST 3: Acceso y manipulación de barcos en flota");
    
    Fleet* fleet = bs_Fleet_create();
    if (fleet == NULL) {
        printf("❌ ERROR: No se pudo crear flota\n");
        error_count++;
        return;
    }
    
    fleet->init(fleet);
    printf("Flota inicializada\n\n");
    
    // Probar acceso a barcos específicos
    printf("Accediendo a barcos específicos:\n");
    
    // Portaviones (índice 0)
    Ship* carrier = fleet->get_ship(fleet, 0);
    if (carrier != NULL) {
        printf("  Barco 0: %s (%d casillas)\n", 
               carrier->get_name(carrier),
               carrier->get_size(carrier));
        if (carrier->get_size(carrier) == 5) {
            printf("  ✅ Portaviones correcto\n");
            test_count++;
        } else {
            printf("  ❌ Tamaño incorrecto\n");
            error_count++;
        }
    } else {
        printf("  ❌ No se pudo acceder al Portaviones\n");
        error_count++;
    }
    
    // Fragata (índice 4)
    Ship* frigate = fleet->get_ship(fleet, 4);
    if (frigate != NULL) {
        printf("  Barco 4: %s (%d casillas)\n",
               frigate->get_name(frigate),
               frigate->get_size(frigate));
        if (frigate->get_size(frigate) == 2) {
            printf("  ✅ Fragata correcta\n");
            test_count++;
        } else {
            printf("  ❌ Tamaño incorrecto\n");
            error_count++;
        }
    } else {
        printf("  ❌ No se pudo acceder a la Fragata\n");
        error_count++;
    }
    
    // Probar acceso fuera de rango
    printf("\nProbando acceso fuera de rango:\n");
    Ship* invalid = fleet->get_ship(fleet, 10);
    if (invalid == NULL) {
        printf("  ✅ Retorna NULL para índice inválido\n");
        test_count++;
    } else {
        printf("  ❌ Debería retornar NULL para índice inválido\n");
        error_count++;
    }
    
    // Probar contadores
    printf("\nVerificando contadores:\n");
    printf("  Barcos en flota: %d\n", fleet->get_remaining_ships(fleet));
    printf("  Celdas totales: %d\n", fleet->get_remaining_cells(fleet));
    
    int expected_cells = 5 + 4 + 3 + 3 + 2;
    if (fleet->get_remaining_cells(fleet) == expected_cells) {
        printf("  ✅ Celdas correctas\n");
        test_count++;
    } else {
        printf("  ❌ Conteo de celdas incorrecto\n");
        error_count++;
    }
    
    // Limpiar
    bs_Fleet_destroy(fleet);
    printf("\n✅ Memoria liberada\n");
}

int main() {
    // Configurar consola para UTF-8
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    
    printf("\n");
    printf("╔════════════════════════════════════════════╗\n");
    printf("║   PRUEBAS UNITARIAS: FLEET Y SHIP SYSTEM  ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    
    test_create_single_ship();
    test_fleet_initialization();
    test_fleet_ship_access();
    
    print_separator("RESUMEN DE PRUEBAS");
    printf("Validaciones exitosas: %d\n", test_count);
    printf("Errores encontrados: %d\n", error_count);
    
    if (error_count == 0) {
        printf("\n✅ ¡TODAS LAS PRUEBAS PASARON!\n\n");
        return 0;
    } else {
        printf("\n❌ SE ENCONTRARON %d ERRORES\n\n", error_count);
        return 1;
    }
}