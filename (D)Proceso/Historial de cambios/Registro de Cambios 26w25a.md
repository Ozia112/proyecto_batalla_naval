# 🛠️ **Changelog Técnico - Actualización de Batalla Naval**

---

## Resumen General

Durante esta actualización, el proyecto Batalla Naval recibió una profunda refactorización y mejoras en la lógica de juego, la estructura de datos y la experiencia de usuario. Se eliminaron funciones obsoletas, se optimizó la gestión de disparos y cartas, y se mejoró la visualización y validación de entradas. Además, se incorporó la persistencia de datos en formato JSON y se estandarizó la documentación técnica y funcional.

---

## Cambios Principales

### 1. **Estandarización y Refactorización de Funciones y Estructuras**

#### a) **Renombrado y estandarización de funciones y campos**

Se realizó un esfuerzo sistemático para estandarizar los nombres de funciones y campos de estructuras, migrando de nombres en español a inglés y usando convenciones consistentes. Esto facilita la mantenibilidad, la colaboración y la comprensión del código, especialmente en equipos mixtos o para futuras integraciones.

**Lista de funciones y campos renombrados:**

| Anterior (Español)           | Nuevo (Inglés)                |
|------------------------------|-------------------------------|
| `barcos_en_fila`             | `getEnemyCellsInRow`          |
| `barcos_en_columna`          | `getEnemyCellsInCol`          |
| `validar_cc_rango`           | `in_range`                    |
| `pantalla_bienvenida`        | `tittle_screen`               |
| `menu_principal`             | `main_menu`                   |
| `vivo` (en struct ship)      | `is_alive`                    |
| `name` (en struct ship)      | `ship_name`                   |
| `failed_shooted_coordinates` | `failed_hits_cc`              |
| `salvo`                      | `salvo_mode`                  |
| `buff`                       | `upgrade_enable`              |
| `remain_ship_cells` (nuevo)  | (nuevo campo, ver explicación)|
| `cards` (array)              | ahora se inicializa desde JSON|
| `ships` (array)              | ahora se inicializa desde JSON|

**Ejemplo de cambio en estructura:**

```c
// Antes
struct ship {
    char name[20];
    bool vivo;
    // ...
};

// Después
struct ship {
    char ship_name[20];
    bool is_alive;
    // ...
};
```

**Motivación y mejoras:**

- **Claridad:** Los nombres en inglés y descriptivos facilitan la comprensión para cualquier desarrollador.
- **Consistencia:** Usar una sola convención evita confusiones y errores.
- **Escalabilidad:** Facilita la integración con librerías externas y la documentación automática.
- **Internacionalización:** Permite que el proyecto sea más accesible a colaboradores internacionales.

#### b) **Estructura de Jugador y Barco**

- **Estructura `player`:**
  - Se reorganizaron los campos para reflejar mejor el estado del jugador.
  - Se añadió el campo `remain_ship_cells` para llevar el conteo actualizado de partes de barcos restantes, lo que permite una verificación más precisa de la condición de victoria.
  - Se migró la inicialización de cartas y barcos a partir de archivos JSON, eliminando la inicialización rígida en código.
  - Se renombraron campos para mayor claridad y consistencia, como `failed_shooted_coordinates` → `failed_hits_cc`.

**Código relevante:**

```c
struct player {
    char player_name[20];
    int placedShips;
    int remain_ship_cells;
    struct ship ships[MAX_SHIPS];
    bool failed_hits_cc[BOARD_SIZE][BOARD_SIZE];
    // ...otros campos...
};
```

- **Estructura `ship`:**
  - Se renombraron campos (`vivo` → `is_alive`, `name` → `ship_name`) y se adaptó la inicialización para leer desde `ships_config.json`.

**Código relevante:**

```c
struct ship {
    char ship_name[20];
    bool is_alive;
    int ship_size;
    // ...otros campos...
};
```

---

### 2. **Nuevas Funciones y Bibliotecas**

- Se agregaron nuevas funciones para la gestión de ataques, chequeos de filas y columnas, y validación de celdas:
  - `in_range`, `first_coord_is_free`, `is_straight`, `dim_match_index`, `end_cell_is_water`, `get_remain_fleet_cells`, `attack_in_range`, `getEnemyCellsInRow`, `getEnemyCellsInCol`, `cell_is_valid_to_shot`, entre otras.

**Código relevante:**

```c
bool in_range(int cc_row, int cc_column);
bool first_coord_is_free(struct player *player, int index, int rowIn, int columnIn);
bool is_straight(struct player *player, int index, int rowIn, int rowEnd, int columnIn, int columnEnd);
bool dim_match_index(struct player *player, int index, int rowIn, int rowEnd, int columnIn, int columnEnd);
bool end_cell_is_water(struct player *player, int index, int rowIn, int rowEnd, int columnIn, int columnEnd);
int getEnemyCellsInRow(struct player *player, struct player *enemy, int cc_row);
int getEnemyCellsInCol(struct player *player, struct player *enemy, int cc_columna);
```

- Se renombraron y modularizaron funciones de cartas:
  - `shoot_func`, `bomb_row`, `bomb_col`, `activateCheckInRow`, `activateCheckInCol`.

**Código relevante:**

```c
void shoot_func(struct player *player, struct player *enemy);
void bomb_row(struct player *player, struct player *enemy);
void bomb_col(struct player *player, struct player *enemy);
void activateCheckInRow(struct player *player, struct player *enemy);
void activateCheckInCol(struct player *player, struct player *enemy);
```

- Se añadieron y actualizaron archivos de cabecera (`bs_board.h`, `bs_cards.h`, `bs_common.h`) para reflejar la nueva estructura y nomenclatura.
- Se agregaron nuevas constantes y códigos de error en `bs_common.h` para mejorar la gestión de errores y condiciones de juego (`INVALID_INPUT`, `OUT_OF_RANGE`, `INVALID_DIRECTION`, etc.).

**Código relevante:**

```c
#define INVALID_INPUT 0
#define OUT_OF_RANGE 2
#define INVALID_DIRECTION 3
#define VICTORY_COND 17
#define CARD_QTY 11
```

- Se integró la biblioteca `cJSON` para la manipulación de archivos JSON, permitiendo la serialización y deserialización de estados de juego, cartas y barcos.

**Código relevante:**

```c
#include "cJSON.h"
// Uso de cJSON para parsear y guardar archivos de configuración y estado
```

---

### 3. **Persistencia y Configuración en JSON**

Se implementó un sistema robusto de persistencia usando archivos JSON para guardar y cargar el estado del juego, cartas y barcos. Esto permite:

- **Configuración flexible:** Los archivos `cards_config.json` y `ships_config.json` permiten modificar las reglas y características del juego sin recompilar el código.
- **Guardado de partidas:** Los archivos `player1_save.json` y `player2_save.json` almacenan el estado completo de cada jugador, incluyendo barcos, cartas, disparos fallidos y variables de juego.
- **Interoperabilidad:** El uso de JSON facilita la integración con otras herramientas y la inspección manual de los datos.

**Archivos agregados:**

- `cards_config.json`: Configuración de cartas, nombres, descripciones y pesos.
- `ships_config.json`: Configuración de barcos, tamaños, estado inicial y tablero base.
- `player1_save.json` y `player2_save.json`: Estado completo de cada jugador, incluyendo barcos, cartas, tablero, chequeos y variables de juego.

**Código relevante:**

```c
// Ejemplo de inicialización de cartas desde JSON
FILE *file = fopen("cards_config.json", "r");
cJSON *json = cJSON_ParseFromFile(file);
// ...parseo y asignación a estructuras...
cJSON_Delete(json);
fclose(file);
```

```json
{
    "cards_config": [
        {
            "card_id": 0,
            "card_name": "Dispara 1 tiro",
            "description": "Inserta coordenadas donde desees disparar.",
            "weight": 7
        },
        // ...
    ]
}
```

**Ejemplo de guardado de estado de jugador:**

```json
{
    "player_name": "a",
    "placedShips": 5,
    "turn": 1,
    "acumulatedTurns": 4,
    "ships": [
        {
            "ship_id": 0,
            "ship_name": "Portaaviones",
            "ship_direction": "O",
            "ship_size": 5,
            "is_alive": true,
            "status": [
                {"fila": 0, "columna": 4, "estado": 1},
                // ...
            ]
        }
        // ...
    ],
    // ...
}
```

---

### 4. **Mejoras en la Lógica de Juego**

- Se mejoró la gestión de disparos fallidos, marcando cada disparo fallido en la matriz `failed_hits_cc` y reflejándolo visualmente en el tablero con el símbolo `×`.

**Código relevante:**

```c
if (!disparo_exitoso) {
    player->failed_hits_cc[player->prevRowInput][player->prevColInput] = true;
}
```

- Se actualizó la lógica de conteo de partes de barcos restantes y la condición de victoria, asegurando que el juego solo termine cuando realmente se hayan hundido todos los barcos enemigos. Se añadió la función `get_remain_fleet_cells` para actualizar el conteo tras cada disparo.

**Código relevante:**

```c
int get_remain_fleet_cells(struct player *player) {
    int count = 0;
    for (int i = 0; i < player->placedShips; ++i) {
        if (player->ships[i].is_alive) {
            for (int j = 0; j < player->ships[i].ship_size; ++j) {
                if (player->ships[i].status[j].estado != 4) {
                    count++;
                }
            }
        }
    }
    return count;
}
```

- Se eliminaron funciones y lógica asociadas al movimiento de barcos, consolidando el enfoque del juego en disparos y cartas.

---

### 5. **Mejoras en la Interfaz y Validación de Entradas**

- Validación flexible de coordenadas: ahora se aceptan formatos como "A1", "A 1", "JJ", "J 10", etc., mejorando la experiencia de usuario y reduciendo errores de entrada.

**Código relevante:**

```c
if ((sscanf(input, " %c %c", &filaIn_c, &colLetra) == 2
    || sscanf(input, " %c%c", &filaIn_c, &colLetra) == 2)
    && isalpha((unsigned char)colLetra)) {
    // ...
}
```

- Visualización mejorada del tablero: los disparos fallidos y los chequeos de filas/columnas se muestran de forma clara y diferenciada.

**Código relevante:**

```c
if (player->failed_hits_cc[i][j] == true) {
    print_status = FAILED_SHOT;
}
switch (print_status) {
    case FAILED_SHOT:
        printf_color(INFO_COLOR, "%c ", FAILED_SHOT_PRINT);
        break;
    // ...
}
```

- Pantallas y menús refactorizados: se modularizaron funciones de UI y se mejoró la navegación y mensajes de error.

---

### 6. **Documentación y Estandarización**

- Se añadieron y reorganizaron archivos Markdown para detallar los requerimientos funcionales, la estructura del menú principal, el sistema de cartas y la inicialización de jugadores.
- Se crearon plantillas para changelogs técnicos y de repositorio, facilitando el seguimiento y la trazabilidad de los cambios.

---

## Cosas Pendientes

- Preparar el entorno para poder modificar la cantidad de cartas y barcos posibles en el juego según el tamaño del array.
- Terminar de reparar la opción de continuar partida. Agregar opción de salir de la partida actual.
- Agregar opción de eliminar partida.
- Preparar entorno para poder elegir modo de juego (clásico o salvo).
- Modificar el apartado de Reglas, darle una actualización de aspecto. Explicar de manera visual y a detalle cada carta y aspecto del juego.

### Bugs conocidos

- El programa se congela en momentos inesperados al intentar reanudar partida.
- Mensajes de bombardeo se comportan de manera no deseada.

---

## Resumen Ejecutivo

- **Estandarización y claridad:** Se migraron todos los nombres de funciones y campos a inglés y se siguieron convenciones consistentes, facilitando la colaboración y el mantenimiento.
- **Persistencia avanzada:** El juego ahora guarda y restaura partidas completas en formato JSON, incluyendo barcos, cartas, disparos fallidos y estado de cada jugador, lo que permite análisis externo y restauración precisa.
- **Configuración flexible:** Las reglas y características de cartas y barcos se definen en archivos JSON, permitiendo ajustes rápidos sin modificar el código fuente.
- **Mejoras visuales y de experiencia:** La interfaz es más clara y flexible, con validaciones robustas y retroalimentación visual inmediata para el usuario.
- **Código más limpio y modular:** Se eliminaron funciones obsoletas y se modularizó la lógica de juego, reduciendo la complejidad y mejorando la mantenibilidad.
- **Documentación profesional:** Se estandarizó la documentación técnica y funcional, facilitando el onboarding de nuevos desarrolladores y la escalabilidad del proyecto.

---

> **Con esta actualización, Batalla Naval se consolida como un proyecto robusto, flexible y profesional, preparado para futuras expansiones y fácil de mantener y adaptar a nuevas reglas o requisitos.**
