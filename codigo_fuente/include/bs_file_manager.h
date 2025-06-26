#ifndef BS_FILE_MANAGER_H
#define BS_FILE_MANAGER_H
#include "master.h" // Include all project headers
#include "cJSON.h" // Include cJSON for JSON handling

static cJSON *load_json_file(const char *filename);

static cJSON *load_config_json_file(const char *filename);

static int getArraysize(cJSON *array);

char encode_value(int v);

int decode_value(char c);

void encode_cell(int status, int ship_id, char *out);

void decode_cell(char *in, int *status, int *ship_id);

void createCardsFile();

void createShipsFile();

void loadToStruct_cardProperties(struct player *player, cJSON *root);

void getCardsWeightState(struct player *player, cJSON *root);

void getBoardState(struct player *player, cJSON *root);

void getShipsState(struct player *player, cJSON *root);

void getRowColCheckState(struct player *player, cJSON *root);

void getLastInputsState(struct player *player, cJSON *root);

void getPlayerState(struct player *player);

void save_ships_state(struct player *player, cJSON *root, FILE *file);

void save_board_state(struct player *player, cJSON *root, FILE *file);

void save_card_state(struct player *player, cJSON *root, FILE *file);

void save_row_col_check_state(struct player *player, cJSON *root, FILE *file);

void save_last_inputs_state(struct player *player, cJSON *root, FILE *file);

void save_player_state(struct player *player);

void getBoardProperties(int *ship_ids, char ship_names[][MAX_NAME_LENGTH], int *ship_sizes);

void getCardsWeights(int *card_weights);

void createPlayerFile(struct player *player);

void delete_player_file(const struct player *player);

#endif