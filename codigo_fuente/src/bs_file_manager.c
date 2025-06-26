#include "bs_file_manager.h"
#include "cJSON.h" // Include cJSON for JSON handling

// Reemplazar macros UPDATE_JSON_* para modificar valores in-place y conservar orden
#undef UPDATE_JSON_NUM
#define UPDATE_JSON_NUM(name, value) \
    do { \
        cJSON *item = cJSON_GetObjectItem(root, name); \
        if (!item || !cJSON_IsNumber(item) || item->valueint != (value)) \
            fprintf(file, "\"%s\": %d", name, (value)); \
        else \
            fprintf(file, "\"%s\": %d", name, item->valueint); \
    } while (0)
#undef UPDATE_JSON_BOOL
#define UPDATE_JSON_BOOL(name, value) \
    do { \
        cJSON *item = cJSON_GetObjectItem(root, name); \
        if (!item || !cJSON_IsBool(item) || item->valueint != (value)) \
            fprintf(file, "\"%s\": %s", name, (value) ? "true" : "false"); \
        else \
            fprintf(file, "\"%s\": %s", name, item->valueint ? "true" : "false"); \
    } while (0)
#undef UPDATE_JSON_STR
#define UPDATE_JSON_STR(name, value) \
    do { \
        cJSON *item = cJSON_GetObjectItem(root, name); \
        if (!item || !cJSON_IsString(item) || strcmp(item->valuestring, (value)) != 0) \
            fprintf(file, "\"%s\": \"%s\"", name, (value)); \
        else \
            fprintf(file, "\"%s\": \"%s\"", name, item->valuestring); \
    } while (0)
#undef UPDATE_JSON_NUM_OBJ
#define UPDATE_JSON_NUM_OBJ(obj, name, value) \
    do { \
        cJSON *item = cJSON_GetObjectItem(obj, name); \
        if (!item || !cJSON_IsNumber(item) || item->valueint != (value)) \
            fprintf(file, "\"%s\": %d", name, (value)); \
        else \
            fprintf(file, "\"%s\": %d", name, item->valueint); \
    } while (0)
#undef UPDATE_JSON_BOOL_OBJ
#define UPDATE_JSON_BOOL_OBJ(obj, name, value) \
    do { \
        cJSON *item = cJSON_GetObjectItem(obj, name); \
        if (!item || !cJSON_IsBool(item) || item->valueint != (value)) \
            fprintf(file, "\"%s\": %s", name, (value) ? "true" : "false"); \
        else \
            fprintf(file, "\"%s\": %s", name, item->valueint ? "true" : "false"); \
    } while (0)
#undef UPDATE_JSON_STR_OBJ
#define UPDATE_JSON_STR_OBJ(obj, name, value) \
    do { \
        cJSON *item = cJSON_GetObjectItem(obj, name); \
        if (!item || !cJSON_IsString(item) || strcmp(item->valuestring, (value)) != 0) \
            fprintf(file, "\"%s\": \"%s\"", name, (value)); \
        else \
            fprintf(file, "\"%s\": \"%s\"", name, item->valuestring); \
    } while (0)
#undef UPDATE_JSON_SINGLE_STR
#define UPDATE_JSON_SINGLE_STR(obj, name, value) \
    do { \
        cJSON *item = cJSON_GetObjectItem(obj, name); \
        if (!item || !cJSON_IsString(item) || strcmp(item->valuestring, (value)) != 0) \
            fprintf(file, "\"%s\"", (value)); \
        else \
            fprintf(file, "\"%s\"", item->valuestring); \
    } while (0)
#undef UPDATE_STRUCT_NUM
#define UPDATE_STRUCT_NUM(struct, field, json_obj, key) \
    do { \
        cJSON *item = cJSON_GetObjectItem(json_obj, key); \
        if (item && cJSON_IsNumber(item)) \
            (struct)->field = item->valueint; \
    } while (0)
#undef UPDATE_STRUCT_BOOL
#define UPDATE_STRUCT_BOOL(struct, field, json_obj, key) \
    do { \
        cJSON *item = cJSON_GetObjectItem(json_obj, key); \
        if (item && cJSON_IsBool(item)) \
            (struct)->field = item->valueint ? 1 : 0; \
    } while (0)
#undef UPDATE_STRUCT_STR
#define UPDATE_STRUCT_STR(struct, field, json_obj, key) \
    do { \
        cJSON *item = cJSON_GetObjectItem(json_obj, key); \
        if (item && cJSON_IsString(item)) { \
            strncpy((struct)->field, item->valuestring, sizeof((struct)->field)); \
            (struct)->field[sizeof((struct)->field) - 1] = '\0'; \
        } \
    } while (0)
#undef UPDATE_STRUCT_NUM_OBJ
#define UPDATE_STRUCT_NUM_OBJ(struct, field, json_obj, key) \
    do { \
        cJSON *item = cJSON_GetObjectItem(json_obj, key); \
        if (item && cJSON_IsNumber(item)) \
            (struct)->field = item->valueint; \
    } while (0)
#undef UPDATE_STRUCT_BOOL_OBJ
#define UPDATE_STRUCT_BOOL_OBJ(struct, field, json_obj, key) \
    do { \
        cJSON *item = cJSON_GetObjectItem(json_obj, key); \
        if (item && cJSON_IsBool(item)) \
            (struct)->field = cJSON_IsTrue(item) ? 1 : 0; \
    } while (0)
#undef UPDATE_STRUCT_STR_OBJ
#define UPDATE_STRUCT_STR_OBJ(struct, field, json_obj, key) \
    do { \
        cJSON *item = cJSON_GetObjectItem(json_obj, key); \
        if (item && cJSON_IsString(item)) { \
            strncpy((struct)->field, item->valuestring, sizeof((struct)->field)); \
            (struct)->field[sizeof((struct)->field) - 1] = '\0'; \
        } \
    } while (0)

static cJSON *load_json_file(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror(filename);
        return EMPTY;
    }
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    rewind(f);

    char *buf = malloc(len + 1);
    if (!buf) {
        fclose(f);
        return EMPTY;
    }
    if (fread(buf, 1, len, f) != (size_t)len) {
        free(buf);
        fclose(f);
        return EMPTY;
    }
    buf[len] = '\0';
    fclose(f);

    cJSON *root = cJSON_Parse(buf);
    free(buf);
    return root; 
}

static cJSON *load_config_json_file(const char *filename) {
    cJSON *root = EMPTY;
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "configs/%s", filename);

    FILE *file = fopen(filepath, "r");
    if (!file) {
        printf("No se pudo abrir %s\n", filepath);
        return EMPTY;
    }
    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    rewind(file);
    char *data = (char*)malloc(len + 1);
    if (!data) {
        fclose(file);
        printf("No hay memoria para leer %s\n", filepath);
        return EMPTY;
    }
    fread(data, 1, len, file);
    data[len] = '\0';
    fclose(file);

    root = cJSON_Parse(data);
    free(data);
    if (!root) {
        printf("Error al parsear %s\n", filepath);
        return EMPTY;
    }
    return root;
}

static int getArraysize(cJSON *array) {
    if (!cJSON_IsArray(array)) return 0;
    return cJSON_GetArraySize(array);
}

char encode_value(int v) {
    if(v == UNSET) return 'F';
    return '0' + v; // Convertir a carácter numérico
}

int decode_value(char c) {
    if (c == 'F') return UNSET;
    return c - '0'; // Convertir de carácter numérico a entero
}

void encode_cell(int status, int ship_id, char* out) {
    out[0] = encode_value(status);
    out[1] = encode_value(ship_id);
    out[2] = '\0'; // Asegurar que la cadena esté terminada
}

void decode_cell(char* in, int* status, int* ship_id) {
    *status = decode_value(in[0]);
    *ship_id = decode_value(in[1]);
}

void createCardsFile() {
    FILE *test, *file;
    
    test = fopen("configs/cards_config.json", "r");
    if (test) {
        fclose(test);
        return; // El archivo ya existe, no es necesario crearlo de nuevo
    }

    file = fopen("configs/cards_config.json", "w");
    if (!file) {
        printf("No se pudo crear cards_config.json\n");
        return;
    }
    fprintf(file,
        "{\n"
        "\t\"cards_config\": [\n"
        "\t\t{\n"
        "\t\t\t\"card_id\": 0,\n"
        "\t\t\t\"card_name\": \"Dispara 1 tiro\",\n"
        "\t\t\t\"description\": \"Inserta coordenadas donde desees disparar.\",\n"
        "\t\t\t\"weight\": 7\n"
        "\t\t},{\n"
        "\t\t\t\"card_id\": 1,\n"
        "\t\t\t\"card_name\": \"Dispara 2 tiros\",\n"
        "\t\t\t\"description\": \"Inserta coordenadas donde desees disparar. Hazlo de nuevo.\",\n"
        "\t\t\t\"weight\": 5\n"
        "\t\t},{\n"
        "\t\t\t\"card_id\": 2,\n"
        "\t\t\t\"card_name\": \"Dispara 3 tiros\",\n"
        "\t\t\t\"description\": \"Inserta coordenadas donde desees disparar. Hazlo dos veces mas.\",\n"
        "\t\t\t\"weight\": 2\n"
        "\t\t},{\n"
        "\t\t\t\"card_id\": 3,\n"
        "\t\t\t\"card_name\": \"Bombardea una fila\",\n"
        "\t\t\t\"description\": \"Si tienes tu buque de 4 casillas a flote, elige un numero y bombardea esa fila entera.\",\n"
        "\t\t\t\"weight\": 6\n"
        "\t\t},{\n"
        "\t\t\t\"card_id\": 4,\n"
        "\t\t\t\"card_name\": \"Bombardea una columna\",\n"
        "\t\t\t\"description\": \"Si tienes tu buque de 5 casillas a flote, elige un numero y bombardea esa columna entera.\",\n"
        "\t\t\t\"weight\": 6\n"
        "\t\t},{\n"
        "\t\t\t\"card_id\": 5,\n"
        "\t\t\t\"card_name\": \"Revela\",\n"
        "\t\t\t\"description\": \"Tu equipo de inteligencia ha logrado decifrar la ubicación de una celda enemiga.\",\n"
        "\t\t\t\"weight\": 1\n"
        "\t\t},{\n"
        "\t\t\t\"card_id\": 6,\n"
        "\t\t\t\"card_name\": \"Chequeo fila\",\n"
        "\t\t\t\"description\": \"Tu equipo logro activar el sonar. Elige un numero y podras ver el numero de casillas enemigas en esa fila.\",\n"
        "\t\t\t\"weight\": 1\n"
        "\t\t},{\n"
        "\t\t\t\"card_id\": 7,\n"
        "\t\t\t\"card_name\": \"Chequeo columna\",\n"
        "\t\t\t\"description\": \"Tu equipo logro activar el sonar. Elige una letra y podras ver el numero de casillas enemigas en esa columna.\",\n"
        "\t\t\t\"weight\": 1\n"
        "\t\t},{\n"
        "\t\t\t\"card_id\": 8,\n"
        "\t\t\t\"card_name\": \"Salvo\",\n"
        "\t\t\t\"description\": \"Esta carta activa el modo Salvo. Dispara los mismos tiros que tengas en flota.\",\n"
        "\t\t\t\"weight\": 1\n"
        "\t\t},{\n"
        "\t\t\t\"card_id\": 9,\n"
        "\t\t\t\"card_name\": \"Torre de ventaja\",\n"
        "\t\t\t\"description\": \"Acumula 4 tarjetas como esta para hundir instantaneamente los barcos.\",\n"
        "\t\t\t\"weight\": 4\n"
        "\t\t},{\n"
        "\t\t\t\"card_id\": 10,\n"
        "\t\t\t\"card_name\": \"Tira y toma\",\n"
        "\t\t\t\"description\": \"Inserta coordenadas donde desees disparar y vuelve a sacar una carta.\",\n"
        "\t\t\t\"weight\": 1\n"
        "\t\t}\n"
        "\t]\n"
        "}"
    );
    fclose(file);
}

void createShipsFile() {
    FILE *test,*file;
    test = fopen("configs/ships_config.json", "r");
    
    if (test) {
        fclose(test);
        return; // El archivo ya existe, no es necesario crearlo de nuevo
    }

    file = fopen("configs/ships_config.json", "w");
    if (!file) {
        printf("No se pudo crear ships_config.json\n");
        return;
    }

    fprintf(file,
        "{\n"
        "\t\"ships_config\": [\n"
        "\t\t{\n"
        "\t\t\t\"ship_id\": 0,\n"
        "\t\t\t\"ship_name\": \"Portaaviones\",\n"
        "\t\t\t\"ship_direction\": \"U\",\n"
        "\t\t\t\"ship_size\": 5,\n"
        "\t\t\t\"is_alive\": true,\n"
        "\t\t\t\"status\": [\n"
        "\t\t\t\t{\"fila\": -1, \"columna\": -1, \"estado\": 0},\n"
        "\t\t\t\t{\"fila\": -1, \"columna\": -1, \"estado\": 0},\n"
        "\t\t\t\t{\"fila\": -1, \"columna\": -1, \"estado\": 0},\n"
        "\t\t\t\t{\"fila\": -1, \"columna\": -1, \"estado\": 0},\n"
        "\t\t\t\t{\"fila\": -1, \"columna\": -1, \"estado\": 0}\n"
        "\t\t\t]\n"
        "\t\t},{\n"
        "\t\t\t\"ship_id\": 1,\n"
        "\t\t\t\"ship_name\": \"Acorazado\",\n"
        "\t\t\t\"ship_direction\": \"U\",\n"
        "\t\t\t\"ship_size\": 4,\n"
        "\t\t\t\"is_alive\": true,\n"
        "\t\t\t\"status\": [\n"
        "\t\t\t\t{\"fila\": -1, \"columna\": -1, \"estado\": 0},\n"
        "\t\t\t\t{\"fila\": -1, \"columna\": -1, \"estado\": 0},\n"
        "\t\t\t\t{\"fila\": -1, \"columna\": -1, \"estado\": 0},\n"
        "\t\t\t\t{\"fila\": -1, \"columna\": -1, \"estado\": 0}\n"
        "\t\t\t]\n"
        "\t\t},{\n"
        "\t\t\t\"ship_id\": 2,\n"
        "\t\t\t\"ship_name\": \"Crucero\",\n"
        "\t\t\t\"ship_direction\": \"U\",\n"
        "\t\t\t\"ship_size\": 3,\n"
        "\t\t\t\"is_alive\": true,\n"
        "\t\t\t\"status\": [\n"
        "\t\t\t\t{\"fila\": -1, \"columna\": -1, \"estado\": 0},\n"
        "\t\t\t\t{\"fila\": -1, \"columna\": -1, \"estado\": 0},\n"
        "\t\t\t\t{\"fila\": -1, \"columna\": -1, \"estado\": 0}\n"
        "\t\t\t]\n"
        "\t\t},{\n"
        "\t\t\t\"ship_id\": 3,\n"
        "\t\t\t\"ship_name\": \"Destructor\",\n"
        "\t\t\t\"ship_direction\": \"U\",\n"
        "\t\t\t\"ship_size\": 3,\n"
        "\t\t\t\"is_alive\": true,\n"
        "\t\t\t\"status\": [\n"
        "\t\t\t\t{\"fila\": -1, \"columna\": -1, \"estado\": 0},\n"
        "\t\t\t\t{\"fila\": -1, \"columna\": -1, \"estado\": 0},\n"
        "\t\t\t\t{\"fila\": -1, \"columna\": -1, \"estado\": 0}\n"
        "\t\t\t]\n"
        "\t\t},{\n"
        "\t\t\t\"ship_id\": 4,\n"
        "\t\t\t\"ship_name\": \"Fragata\",\n"
        "\t\t\t\"ship_direction\": \"U\",\n"
        "\t\t\t\"ship_size\": 2,\n"
        "\t\t\t\"is_alive\": true,\n"
        "\t\t\t\"status\": [\n"
        "\t\t\t\t{\"fila\": -1, \"columna\": -1, \"estado\": 0},\n"
        "\t\t\t\t{\"fila\": -1, \"columna\": -1, \"estado\": 0}\n"
        "\t\t\t]\n"
        "\t\t}\n"
        "\t],\n"
    );
    fprintf(file, "\t\"board_config\": [\n");
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; ++i) {
        fprintf(file, "%s\"0F\"%s",
                    (i == 0) ? "\t\t" : "",
                    (i == BOARD_SIZE * BOARD_SIZE - 1) ? "\n" : ",\n\t\t");
    }
    fprintf(file, "\t]\n}");
}

void loadToStruct_cardProperties(struct player *player, cJSON *root) {
    cJSON *cards_array, *card_obj;
    int i;

    cards_array = cJSON_GetObjectItem(root, "cards_config");
    if (!cards_array || !cJSON_IsArray(cards_array)) {
        printf("No se encontró el array 'cards_config' en el archivo JSON\n");
        return;
    }

    for (i = 0; i < getArraysize(cards_array); i++) {
        struct card *card = &player->cards[i];
        card_obj = cJSON_GetArrayItem(cards_array, i);
        if (card_obj && cJSON_IsObject(card_obj)) {
            UPDATE_STRUCT_NUM_OBJ(card, card_id, card_obj, "card_id");
            UPDATE_STRUCT_STR_OBJ(card, card_name, card_obj, "card_name");
            UPDATE_STRUCT_STR_OBJ(card, description, card_obj, "description");
        }
    }
}

void getCardsWeightState(struct player *player, cJSON *root) {
    cJSON *cards_array, *card_obj;
    int i;

    cards_array = cJSON_GetObjectItem(root, "cards");
    if (!cards_array || !cJSON_IsArray(cards_array)) {
        printf("No se encontró el array 'cards' en el archivo JSON\n");
        return;
    }

    for (i = 0; i < getArraysize(cards_array); i++) {
        struct card *card = &player->cards[i];
        card_obj = cJSON_GetArrayItem(cards_array, i);
        if (card_obj && cJSON_IsObject(card_obj)) {
            UPDATE_STRUCT_NUM_OBJ(card, peso, card_obj, "weight");
        }
    }
}

void getBoardState(struct player *player, cJSON *root) {
    cJSON *board_array, *cell;
    int i, j;

    board_array = cJSON_GetObjectItem(root, "board");
    if (!board_array || !cJSON_IsArray(board_array)) {
        printf("No se encontró el array 'board' en el archivo JSON\n");
        return;
    }

    int expected_size = BOARD_SIZE * BOARD_SIZE;
    int actual_size = cJSON_GetArraySize(board_array);
    
    if (actual_size != expected_size) {
        printf("Tamaño de board incorrecto: esperado %d, encontrado %d\n", expected_size, actual_size);
        return;
    }

    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            cell = cJSON_GetArrayItem(board_array, i * BOARD_SIZE + j);
            if (!cell || !cJSON_IsString(cell)) {
                player->board[i][j].status = WATER;
                player->board[i][j].ship_id = UNSET;
                player->board[i][j].is_water = true;
                player->board[i][j].ship_cell = UNSET;
            } else {
                char *cell_str = cell->valuestring;
                if (cell_str && strlen(cell_str) >= 2) {
                    decode_cell(cell_str, &player->board[i][j].status, &player->board[i][j].ship_id);
                    player->board[i][j].is_water = (player->board[i][j].status == WATER);
                    player->board[i][j].ship_cell = UNSET;
                } else {
                    // String inválida, inicializar como agua
                    player->board[i][j].status = WATER;
                    player->board[i][j].ship_id = UNSET;
                    player->board[i][j].is_water = true;
                    player->board[i][j].ship_cell = UNSET;
                }
            }
        }
    }
}

void getShipState(struct player *player, cJSON *root) {
    cJSON *ships_array, *ship_obj, *status_array, *status_obj;
    int i, ship_cell;

    ships_array = cJSON_GetObjectItem(root, "ships");
    if (!ships_array || !cJSON_IsArray(ships_array)) {
        printf("No se encontró el array 'ships' en el archivo JSON\n");
        return;
    }

    for (i = 0; i < NUM_SHIPS; i++) {
        struct ship *ship = &player->ships[i];
        ship_obj = cJSON_GetArrayItem(ships_array, i);
        if (ship_obj && cJSON_IsObject(ship_obj)) {
            UPDATE_STRUCT_NUM_OBJ(ship, ship_id, ship_obj, "ship_id");
            UPDATE_STRUCT_STR(ship, ship_name, ship_obj, "ship_name");
            UPDATE_STRUCT_NUM_OBJ(ship, ship_size, ship_obj, "ship_size");
            
            // Validar que ship_size sea válido antes de asignar memoria
            if (ship->ship_size <= 0 || ship->ship_size > 5) {
                printf("Tamaño de barco inválido para ship_id %d: %d\n", i, ship->ship_size);
                continue;
            }
            
            // ship_direction es un char, así que tomamos el primer carácter de la cadena JSON
            cJSON *dir_item = cJSON_GetObjectItem(ship_obj, "ship_direction");
            if (dir_item && cJSON_IsString(dir_item) && dir_item->valuestring && strlen(dir_item->valuestring) > 0)
                ship->ship_direction = dir_item->valuestring[0];
                
            UPDATE_STRUCT_BOOL_OBJ(ship, is_alive, ship_obj, "is_alive");
            
            // Procesar el array de status
            status_array = cJSON_GetObjectItem(ship_obj, "status");
            if (status_array && cJSON_IsArray(status_array)) {
                // Verificar si el status ya está inicializado, si no, inicializarlo
                if (!ship->status) {
                    ship->status = (int **)malloc(ship->ship_size * sizeof(int *));
                    if (!ship->status) {
                        printf("Error al asignar memoria para ship->status\n");
                        continue;
                    }
                    for (ship_cell = 0; ship_cell < ship->ship_size; ship_cell++) {
                        ship->status[ship_cell] = (int *)malloc(3 * sizeof(int));
                        if (!ship->status[ship_cell]) {
                            printf("Error al asignar memoria para ship->status[%d]\n", ship_cell);
                            // Liberar memoria ya asignada
                            for (int k = 0; k < ship_cell; k++) {
                                free(ship->status[k]);
                            }
                            free(ship->status);
                            ship->status = EMPTY;
                            break;
                        }
                    }
                }
                
                // Solo procesar si la asignación de memoria fue exitosa
                if (ship->status) {
                    for (ship_cell = 0; ship_cell < ship->ship_size; ship_cell++) {
                        status_obj = cJSON_GetArrayItem(status_array, ship_cell);
                        if (status_obj && cJSON_IsObject(status_obj)) {
                            UPDATE_STRUCT_NUM_OBJ(ship, status[ship_cell][CC_ROW], status_obj, "fila");
                            UPDATE_STRUCT_NUM_OBJ(ship, status[ship_cell][CC_COLUMN], status_obj, "columna");
                            UPDATE_STRUCT_NUM_OBJ(ship, status[ship_cell][CC_STATUS], status_obj, "estado");
                        }
                    }
                }
            }
        }
    }
}

void getRowColCheckState(struct player *player, cJSON *root) {
    cJSON *row_check_array, *cellCntRow_array, *col_check_array, *cellCntCol_array;
    int i;

    row_check_array = cJSON_GetObjectItem(root, "row_check");
    col_check_array = cJSON_GetObjectItem(root, "col_check");
    cellCntRow_array = cJSON_GetObjectItem(root, "cellCntRow");
    cellCntCol_array = cJSON_GetObjectItem(root, "cellCntCol");
    if (!row_check_array || !cJSON_IsArray(row_check_array) ||
        !col_check_array || !cJSON_IsArray(col_check_array) ||
        !cellCntRow_array || !cJSON_IsArray(cellCntRow_array) ||
        !cellCntCol_array || !cJSON_IsArray(cellCntCol_array)) {
        printf("No se encontraron los arrays necesarios en el archivo JSON\n");
        return;
    }

    for (i = 0; i < BOARD_SIZE; i++) {
        cJSON *row_check_obj = cJSON_GetArrayItem(row_check_array, i);
        cJSON *col_check_obj = cJSON_GetArrayItem(col_check_array, i);
        cJSON *cellCntRow_obj = cJSON_GetArrayItem(cellCntRow_array, i);
        cJSON *cellCntCol_obj = cJSON_GetArrayItem(cellCntCol_array, i);

        if (row_check_obj) {
            UPDATE_STRUCT_BOOL_OBJ(player, row_check[i], row_check_obj, "chequeo");
        }
        if (cellCntRow_obj) {
            UPDATE_STRUCT_NUM_OBJ(player, cellCntRow[i], cellCntRow_obj, "contador");
        }
        if (col_check_obj) {
            UPDATE_STRUCT_BOOL_OBJ(player, col_check[i], col_check_obj, "chequeo");
        }
        if (cellCntCol_obj) {
            UPDATE_STRUCT_NUM_OBJ(player, cellCntCol[i], cellCntCol_obj, "contador");
        }
    }
}

void getLastInputsState(struct player *player, cJSON *root) {
    cJSON *last_inputs_obj = cJSON_GetObjectItem(root, "last_inputs");
    if (!last_inputs_obj || !cJSON_IsObject(last_inputs_obj)) {
        printf("No se encontró el objeto 'last_inputs' en el archivo JSON\n");
        return;
    }

    UPDATE_STRUCT_NUM(player, prevRowInput, last_inputs_obj, "fila");
    UPDATE_STRUCT_NUM(player, prevColInput, last_inputs_obj, "columna");
    UPDATE_STRUCT_NUM(player, prevHitRow, last_inputs_obj, "success_full_shot_fila");
    UPDATE_STRUCT_NUM(player, prevHitCol, last_inputs_obj, "success_full_shot_columna");
    UPDATE_STRUCT_NUM(player, prevCard, last_inputs_obj, "prevCard");
    UPDATE_STRUCT_NUM(player, hitsInTurn, last_inputs_obj, "hitsInTurn");
}

void getPlayerState(struct player *player) {
    char filename[256];
    cJSON *root;
    
    snprintf(filename, sizeof(filename), "configs/player%d_save.json", player->player_index);
    root = load_json_file(filename);
    if (!root) {
        printf("No se pudo cargar el estado del jugador %d\n", player->player_index);
        return;
    }

    // Luego cargar el estado guardado
    UPDATE_STRUCT_STR(player, player_name, root, "player_name");
    UPDATE_STRUCT_NUM(player, placed_ships, root, "placedShips");
    UPDATE_STRUCT_NUM(player, turn, root, "turn");
    UPDATE_STRUCT_NUM(player, acc_turns, root, "acumulatedTurns");
    UPDATE_STRUCT_NUM(player, enemy_hit_parts, root, "enemy_hit_parts");
    UPDATE_STRUCT_NUM(player, sunken_ships, root, "sunken_ships");
    UPDATE_STRUCT_NUM(player, remain_ship_cells, root, "remain_ship_cells");
    UPDATE_STRUCT_NUM(player, cards_total_weight, root, "cards_total_weight");
    UPDATE_STRUCT_NUM(player, acc_towers, root, "acc_towers");
    UPDATE_STRUCT_NUM(player, prev_check_row, root, "prev_check_row");
    UPDATE_STRUCT_NUM(player, prev_check_col, root, "prev_check_col");
    UPDATE_STRUCT_BOOL(player, salvo_mode, root, "salvo_mode");
    UPDATE_STRUCT_BOOL(player, upgrade_enable, root, "buff");
    
    // Funciones para obtener los arrays (ahora que las estructuras están inicializadas)
    getCardsWeightState(player, root);
    getBoardState(player, root);
    getShipState(player, root);
    getRowColCheckState(player, root);
    getLastInputsState(player, root);
    
    cJSON_Delete(root);
    printf_color(SUCCESS_COLOR, "Estado del jugador %d cargado correctamente.\n", player->player_index);
}

void save_ships_state(struct player *player, cJSON *root, FILE *file) {
    cJSON *ship_json, *ships_json, *status_array, *status_item;
    int i, j, ship_qty, part_qty;
    char dir[2];

    if (!player->ships[0].status) {
        getShipState(player, root);
    }

    ships_json = cJSON_GetObjectItem(root, "ships");
    ship_qty   = cJSON_GetArraySize(ships_json);
    for (i = 0; i < ship_qty; ++i) {
        struct ship *ship = &player->ships[i];
        ship_json    = cJSON_GetArrayItem(ships_json, i);
        status_array = cJSON_GetObjectItem(ship_json, "status");
        part_qty     = cJSON_GetArraySize(status_array);

        dir[0] = ship->ship_direction;
        dir[1] = '\0';

        fprintf(file, "%s{\n", (i > 0) ? " " : "\t\t");

        fprintf(file, "\t\t\t"); UPDATE_JSON_NUM("ship_id", ship->ship_id); fprintf(file, ",\n");
        fprintf(file, "\t\t\t"); UPDATE_JSON_STR("ship_name", ship->ship_name); fprintf(file, ",\n");
        fprintf(file, "\t\t\t"); UPDATE_JSON_STR("ship_direction", dir); fprintf(file, ",\n");
        fprintf(file, "\t\t\t"); UPDATE_JSON_NUM("ship_size", ship->ship_size); fprintf(file, ",\n");
        fprintf(file, "\t\t\t"); UPDATE_JSON_BOOL("is_alive", ship->is_alive); fprintf(file, ",\n");
        fprintf(file, "\t\t\t\"status\": [\n");
        
        
        for (j = 0; j < part_qty; ++j) {
            status_item = cJSON_GetArrayItem(status_array, j);
            fprintf(file, "\t\t\t\t{");
            UPDATE_JSON_NUM_OBJ(status_item, "fila", ship->status[j][CC_ROW]); fprintf(file, ", ");
            UPDATE_JSON_NUM_OBJ(status_item, "columna", ship->status[j][CC_COLUMN]); fprintf(file, ", ");
            UPDATE_JSON_NUM_OBJ(status_item, "estado", ship->status[j][CC_STATUS]);
            fprintf(file,"}%s\n", (j < part_qty - 1) ? "," : "");
        }
        fprintf(file, "\t\t\t]\n");
        fprintf(file, "\t\t%s", (i < NUM_SHIPS - 1) ? "}," : "}\n");
    }
}

void save_board_state(struct player *player, cJSON *root, FILE *file) {
    cJSON *board_array, *board_json,*cell_json;
    char cell_code[3];
    int row, col, idx = 0;

    board_json = cJSON_GetObjectItem(root, "board");
    for (row = 0; row < BOARD_SIZE; row++) {
        fprintf(file, "\t\t");
        for (col = 0; col < BOARD_SIZE; col++) {
            idx = row * BOARD_SIZE + col;
            cell_json = cJSON_GetArrayItem(board_json, idx);
            encode_cell(player->board[row][col].status, player->board[row][col].ship_id, cell_code);
            UPDATE_JSON_SINGLE_STR(cell_json, "cell_code", cell_code);
            if (col < BOARD_SIZE - 1) fprintf(file, ", ");
        }
        fprintf(file, row < BOARD_SIZE - 1 ? ",\n" : "\n");
    }
}

void save_card_state(struct player *player, cJSON *root, FILE *file) {
    cJSON *cards_array, *card_obj;
    int i;

    cards_array = cJSON_GetObjectItem(root, "cards");

    for (i = 0; i < CARD_QTY; ++i) {
        card_obj = cJSON_GetArrayItem(cards_array, i);
        fprintf(file, "\t\t{");
        UPDATE_JSON_NUM_OBJ(card_obj, "card_id", i); fprintf(file, ", ");
        UPDATE_JSON_NUM_OBJ(card_obj, "weight", player->cards[i].peso); fprintf(file, "}%s\n",(i < CARD_QTY - 1) ? "," : "");
    }
}

void save_row_col_check_state(struct player *player, cJSON *root, FILE *file) {
    cJSON *row_check_array, *cellCntRow_array, *col_check_array, *cellCntCol_array;
    cJSON *row_check_obj, *cellCntRow_obj, *col_check_obj, *cellCntCol_obj;
    int i;

    row_check_array = cJSON_GetObjectItem(root, "row_check");
    cellCntRow_array = cJSON_GetObjectItem(root, "cellCntRow");
    col_check_array = cJSON_GetObjectItem(root, "col_check");
    cellCntCol_array = cJSON_GetObjectItem(root, "cellCntCol");

    // row_check
    fprintf(file, "\t\"row_check\": [");
    for (i = 0; i < BOARD_SIZE; ++i) {
        row_check_obj = cJSON_GetArrayItem(row_check_array, i);
        if (i % 2 == 0) {
            fprintf(file, "\n\t\t");
        }
        fprintf(file, "{");
        UPDATE_JSON_NUM_OBJ(row_check_obj, "fila", i); fprintf(file, ", ");
        UPDATE_JSON_BOOL_OBJ(row_check_obj, "chequeo", player->row_check[i]);
        fprintf(file, "}%s", (i < BOARD_SIZE - 1) ? ", " : "\n");
    }
    fprintf(file, "\t],\n");
    
    // cellCntRow
    fprintf(file, "\t\"cellCntRow\": [");
    for (i = 0; i < BOARD_SIZE; ++i) {
        cellCntRow_obj = cJSON_GetArrayItem(cellCntRow_array, i);
        if (i % 2 == 0) {
            fprintf(file, "\n\t\t");
        }
        fprintf(file, "{");
        UPDATE_JSON_NUM_OBJ(cellCntRow_obj, "fila", i); fprintf(file, ", ");
        UPDATE_JSON_NUM_OBJ(cellCntRow_obj, "contador", player->cellCntRow[i]);
        fprintf(file, "}%s", (i < BOARD_SIZE - 1) ? ", " : "\n");
    }
    fprintf(file, "\t],\n");

    UPDATE_JSON_NUM("prev_check_row", player->prev_check_row); fprintf(file, ",\n");

    // col_check
    fprintf(file, "\t\"col_check\": [");
    for (i = 0; i < BOARD_SIZE; ++i) {
        col_check_obj = cJSON_GetArrayItem(col_check_array, i);
        if (i % 2 == 0) {
            fprintf(file, "\n\t\t");
        }
        fprintf(file, "{");
        UPDATE_JSON_NUM_OBJ(col_check_obj, "columna", i); fprintf(file, ", ");
        UPDATE_JSON_BOOL_OBJ(col_check_obj, "chequeo", player->col_check[i]);
        fprintf(file, "}%s", (i < BOARD_SIZE - 1) ? ", " : "\n");
    }
    fprintf(file, "\t],\n");

    // cellCntCol
    fprintf(file, "\t\"cellCntCol\": [");
    for (i = 0; i < BOARD_SIZE; ++i) {
        cellCntCol_obj = cJSON_GetArrayItem(cellCntCol_array, i);
        if (i % 2 == 0) {
            fprintf(file, "\n\t\t");
        }
        fprintf(file, "{");
        UPDATE_JSON_NUM_OBJ(cellCntCol_obj, "columna", i); fprintf(file, ", ");
        UPDATE_JSON_NUM_OBJ(cellCntCol_obj, "contador", player->cellCntCol[i]);
        fprintf(file, "}%s", (i < BOARD_SIZE - 1) ? ", " : "\n");
    }
    fprintf(file, "\t],\n");

    UPDATE_JSON_NUM("prev_check_col", player->prev_check_col); fprintf(file, ",\n");
}

void save_last_inputs_state(struct player *player, cJSON *root, FILE *file) {
    cJSON *obj;
    
    obj = cJSON_GetObjectItem(root, "last_inputs");
    fprintf(file, "\t\"last_inputs\": {\n");
    UPDATE_JSON_NUM_OBJ(obj, "fila",                      player->prevRowInput); fprintf(file, ",\n");
    UPDATE_JSON_NUM_OBJ(obj, "columna",                   player->prevColInput); fprintf(file, ",\n");
    UPDATE_JSON_NUM_OBJ(obj, "success_full_shot_fila",    player->prevHitRow);   fprintf(file, ",\n");
    UPDATE_JSON_NUM_OBJ(obj, "success_full_shot_columna", player->prevHitCol);   fprintf(file, ",\n");
    UPDATE_JSON_NUM_OBJ(obj, "prevCard",                  player->prevCard);     fprintf(file, ",\n");
    UPDATE_JSON_NUM_OBJ(obj, "hitsInTurn",                player->hitsInTurn);    fprintf(file, "\n");
    fprintf(file, "\t}\n");
}

void save_player_state(struct player *player) {
    char player_filename[256]; FILE *file; cJSON *root;

    snprintf(player_filename, sizeof(player_filename), "configs/player%d_save.json", player->player_index);

    root = load_json_file(player_filename);
    if (!root) {
        printf("Error al cargar el archivo JSON %s\n", player_filename);
        return;
    }

    file = fopen(player_filename, "w");
    if (!file) {
        printf("Error al sobreescribir el archivo JSON %s\n", player_filename);
        cJSON_Delete(root);
        return;
    }

    fprintf(file, "{\n");
    fprintf(file, "\t"); UPDATE_JSON_STR("player_name", player->player_name); fprintf(file,",\n");
    fprintf(file, "\t"); UPDATE_JSON_NUM("placedShips", player->placed_ships); fprintf(file,",\n");
    fprintf(file, "\t"); UPDATE_JSON_NUM("turn", player->turn); fprintf(file,",\n");
    fprintf(file, "\t"); UPDATE_JSON_NUM("acumulatedTurns", player->acc_turns); fprintf(file,",\n");

    // ships
    fprintf(file, "\t\"ships\": [\n");

    save_ships_state(player, root, file);

    fprintf(file, "\t],\n");

    fprintf(file, "\t"); UPDATE_JSON_NUM("enemy_hit_parts", player->enemy_hit_parts); fprintf(file, ",\n");
    fprintf(file, "\t"); UPDATE_JSON_NUM("sunken_ships", player->sunken_ships); fprintf(file, ",\n");

    // board
    fprintf(file, "\t\"board\": [\n");

    save_board_state(player, root, file);

    fprintf(file, "\t],\n");
    fprintf(file, "\t");  UPDATE_JSON_NUM("remain_ship_cells", player->remain_ship_cells); fprintf(file,",\n");

    // cards
    fprintf(file, "\t\"cards\": [\n");

    save_card_state(player, root, file);

    fprintf(file, "\t],\n");
    UPDATE_JSON_NUM("cards_total_weight", player->cards_total_weight); fprintf(file, ",\n");
    UPDATE_JSON_NUM("acc_towers", player->acc_towers); fprintf(file, ",\n");

    save_row_col_check_state(player, root, file);

    UPDATE_JSON_BOOL("salvo_mode", player->salvo_mode); fprintf(file, ",\n");
    UPDATE_JSON_BOOL("buff", player->upgrade_enable); fprintf(file, ",\n");

    save_last_inputs_state(player, root, file);
    fprintf(file, "}");

    fclose(file);
    cJSON_Delete(root);
}

void getBoardProperties(int *ship_ids,
                        char ship_names[][MAX_NAME_LENGTH],
                        int *ship_sizes) {
    cJSON *root, *array, *obj;
    int i, j, array_size;
    
    // Lectura de configuración de barcos
    root = load_config_json_file("ships_config.json");
    array = cJSON_GetObjectItem(root, "ships_config");
    array_size = getArraysize(array);
    for (i = 0; i < array_size; i++) {
        obj = cJSON_GetArrayItem(array, i);
        ship_ids[i] = cJSON_GetObjectItem(obj, "ship_id")->valueint;
        strncpy(ship_names[i],
                cJSON_GetObjectItem(obj, "ship_name")->valuestring,
                MAX_NAME_LENGTH);
        ship_names[i][MAX_NAME_LENGTH-1] = '\0';
        ship_sizes[i] = cJSON_GetObjectItem(obj, "ship_size")->valueint;
    }
    cJSON_Delete(root);
}

void getCardsWeights(int *card_weights) {
    cJSON *root, *array, *obj;
    int i, array_size;

    root = load_config_json_file("cards_config.json");
    array = cJSON_GetObjectItem(root, "cards_config");
    array_size = getArraysize(array);

    for (i = 0; i < array_size; i++) {
        obj = cJSON_GetArrayItem(array, i);
        card_weights[i] = cJSON_GetObjectItem(obj, "weight")->valueint;
    }
    cJSON_Delete(root);
}

void createPlayerFile(struct player *player) {
    char filename[256];
    FILE *test, *file;
    cJSON *root_ships, *root_cards;
    cJSON *ships_config, *cards_config;
    int i, j, arr_sz_s, arr_sz_c;


    snprintf(filename, sizeof(filename), "configs/player%d_save.json", player->player_index);

    test = fopen(filename, "r");
    if (test) {
        fclose(test);
        return;
    }

    root_ships = load_config_json_file("ships_config.json");
    ships_config = cJSON_GetObjectItem(root_ships, "ships_config");
    root_cards = load_config_json_file("cards_config.json");
    cards_config = cJSON_GetObjectItem(root_cards, "cards_config");
    arr_sz_s = getArraysize(ships_config);
    arr_sz_c = getArraysize(cards_config);
    
    int ship_ids[arr_sz_s];
    char ship_names[arr_sz_s][MAX_NAME_LENGTH];
    int ship_sizes[arr_sz_s];

    getBoardProperties(ship_ids, ship_names, ship_sizes);

    int card_weights[arr_sz_c];

    getCardsWeights(card_weights);

    file = fopen(filename, "w");
    if (!file) {
        printf("Error al sobreescribir el archivo JSON %s\n", filename);
        cJSON_Delete(root_ships);
        cJSON_Delete(root_cards);
        return;
    }

    fprintf(file, "{\n"
                    "\t\"player_name\": \"%s\",\n"
                    "\t\"placedShips\": %d,\n"
                    "\t\"turn\": %d,\n"
                    "\t\"acumulatedTurns\": %d,\n",
                    "", NONE, UNSET, NONE);

    // ships
    fprintf(file, "\t\"ships\": [\n");
    for (i = 0; i < arr_sz_s; ++i) {
        fprintf(file, "%s{\n", (i > 0) ? " " : "\t\t");
        fprintf(file,"\t\t\t\"ship_id\": %d,\n"
                     "\t\t\t\"ship_name\": \"%s\",\n"
                     "\t\t\t\"ship_direction\": \"U\",\n"
                     "\t\t\t\"ship_size\": %d,\n"
                     "\t\t\t\"is_alive\": true,\n", 
                     ship_ids[i], ship_names[i], ship_sizes[i]);

        fprintf(file, "\t\t\t\"status\": [\n");

        for (j = 0; j < ship_sizes[i]; ++j) {
            fprintf(file, "\t\t\t\t{\"fila\": %d, \"columna\": %d, \"estado\": %d}%s\n",
                UNSET, UNSET, NONE, (j < ship_sizes[i] - 1) ? "," : "");
        }
        fprintf(file, "\t\t\t]\n");
        fprintf(file, "\t\t%s", (i < arr_sz_s - 1) ? "}," : "}\n");
    }
    fprintf(file, "\t],\n");

    fprintf(file, "\t\"enemy_hit_parts\": %d,\n"
                  "\t\"sunken_ships\": %d,\n", 
                  NONE, NONE);

    // board
    fprintf(file, "\t\"board\": [\n");
    for (i = 0; i < BOARD_SIZE * BOARD_SIZE; ++i) {
        if (i < BOARD_SIZE * BOARD_SIZE - 1) {
            fprintf(file, "%s\"0F\"%s%s",
                    (i % BOARD_SIZE == 0) ? "\t\t" : "",
                    (i == BOARD_SIZE * BOARD_SIZE - 1) ? "," : "",
                    (i % BOARD_SIZE == BOARD_SIZE - 1) ? ",\n" : ", ");
        } else fprintf(file, "\"0F\"\n");
    }
    fprintf(file, "\t],\n");
    fprintf(file, "\t\"remain_ship_cells\": %d,\n", NONE);
    
    // cards
    loadToStruct_cardProperties(player, root_cards);

    fprintf(file, "\t\"cards\": [\n");
    for (int i = 0; i < arr_sz_c; ++i) {
        fprintf(file, "\t\t{\"card_id\": %d, \"weight\": %d}%s\n",
                i, card_weights[i], (i < arr_sz_c - 1) ? "," : "");
    }
    fprintf(file, "\t],\n");
    fprintf(file, "\t\"cards_total_weight\": %d,\n", NONE);
    fprintf(file, "\t\"acc_towers\": %d,\n", NONE);

    // row_check
    fprintf(file, "\t\"row_check\": [");
    for (i = 0; i < BOARD_SIZE; ++i) {
        if (i % 2 == 0) {
            fprintf(file, "\n\t\t");
        }
        fprintf(file, "{\"fila\": %d, \"chequeo\": false}%s", i, (i < BOARD_SIZE - 1) ? ", " : "\n");
    }
    fprintf(file, "\t],\n");

    // cellCntRow
    fprintf(file, "\t\"cellCntRow\": [");
    for (i = 0; i < BOARD_SIZE; ++i) {
        if (i % 2 == 0) {
            fprintf(file, "\n\t\t");
        }
        fprintf(file, "{\"fila\": %d, \"contador\": %d}%s", i, NONE, (i < BOARD_SIZE - 1) ? ", " : "\n");
    }
    fprintf(file, "\t],\n");

    fprintf(file, "\t\"prev_check_row\": %d,\n", UNSET);

    // col_check
    fprintf(file, "\t\"col_check\": [");
    for (i = 0; i < BOARD_SIZE; ++i) {
        if (i % 2 == 0) {
            fprintf(file, "\n\t\t");
        }
        fprintf(file, "{\"columna\": %d, \"chequeo\": false}%s", i, (i < BOARD_SIZE - 1) ? ", " : "\n");
    }
    fprintf(file, "\t],\n");

    // cellCntCol
    fprintf(file, "\t\"cellCntCol\": [");
    for (i = 0; i < BOARD_SIZE; ++i) {
        if (i % 2 == 0) {
            fprintf(file, "\n\t\t");
        }
        fprintf(file, "{\"columna\": %d, \"contador\": %d}%s", i, NONE, (i < BOARD_SIZE - 1) ? ", " : "\n");
    }
    fprintf(file, "\t],\n");

    fprintf(file, "\t\"prev_check_col\": %d,\n", UNSET);
    fprintf(file, "\t\"salvo_mode\": false,\n");
    fprintf(file, "\t\"buff\": false,\n");

    // last_inputs
    fprintf(file, "\t\"last_inputs\": {\n");
    fprintf(file, "\t\t\"fila\": %d,\n", UNSET);
    fprintf(file, "\t\t\"columna\": %d,\n", UNSET);
    fprintf(file, "\t\t\"success_full_shot_fila\": %d,\n", UNSET);
    fprintf(file, "\t\t\"success_full_shot_columna\": %d,\n", UNSET);
    fprintf(file, "\t\t\"prevCard\": %d,\n", UNSET);
    fprintf(file, "\t\t\"hitsInTurn\": %d\n", NONE);
    fprintf(file, "\t}\n");
    fprintf(file, "}");
    fclose(file);

    // Limpiar la memoria del JSON
    cJSON_Delete(root_cards);
    cJSON_Delete(root_ships);

    printf_color(SUCCESS_COLOR, "Archivo de jugador %d creado exitosamente.\n", player->player_index);
    
}

void delete_player_file(const struct player *player) {
    char player_filename[256];
    snprintf(player_filename, sizeof(player_filename), "configs/player%d_save.json", player->player_index);
    remove(player_filename);
}