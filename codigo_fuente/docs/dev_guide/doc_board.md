# bs_board Functions Documentation

## English

- `bool in_range(int cc_Row, int cc_Col)`

    Checks if the given row and column are within the board limits. Calls `codeError` if out of range.

- `bool first_coord_is_free(struct player *player, int ship_idx, int cc_RowIn, int cc_ColIn)`

    Checks if the specified cell is water (free) for ship placement. Calls `codeError` if occupied.

- `bool is_straight(struct player *player, int ship_idx, int cc_RowIn, int cc_RowEnd, int cc_ColIn, int cc_ColEnd)`

    Checks if the ship placement is straight (horizontal or vertical). Sets ship direction. Calls `codeError` if not straight.

- `bool dim_match_index(struct player *player, int ship_idx, int cc_RowIn, int cc_RowEnd, int cc_ColIn, int cc_ColEnd)`

    Checks if the ship's length matches its size. Calls `codeError` if not.

- `bool end_cell_is_water(struct player *player, int ship_idx, int cc_RowIn, int cc_RowEnd, int cc_ColIn, int cc_ColEnd)`

    Checks if all cells along the ship's path are water (free). Calls `codeError` if overlap detected.

- `void get_remain_fleet_cells(struct player *player)`

    Counts and updates the number of remaining ship cells for the player.

- `bool attack_in_range(struct player *player, int cc_Row, int cc_Col)`

    Checks if an attack is within valid range, considering special card effects. Calls `codeError` if out of range.

- `int getEnemyCellsInRow(struct player *player, struct player *enemy, int cc_Row)`

    Counts the number of enemy ship cells in a given row.

- `int getEnemyCellsInCol(struct player *player, struct player *enemy, int cc_Col)`

    Counts the number of enemy ship cells in a given column.

- `bool cell_is_valid_to_shot(struct player *player, struct player *enemy)`

    Checks if the targeted cell can be shot (not previously shot or destroyed). Calls `codeError` if already shot.

## Español

- `bool in_range(int cc_Row, int cc_Col)`

    Verifica si la fila y columna dadas están dentro de los límites del tablero. Llama a `codeError` si está fuera de rango.

- `bool first_coord_is_free(struct player *player, int ship_idx, int cc_RowIn, int cc_ColIn)`

    Verifica si la celda especificada es agua (libre) para colocar un barco. Llama a `codeError` si está ocupada.

- `bool is_straight(struct player *player, int ship_idx, int cc_RowIn, int cc_RowEnd, int cc_ColIn, int cc_ColEnd)`

    Verifica si la colocación del barco es recta (horizontal o vertical). Establece la dirección del barco. Llama a `codeError` si no es recta.

- `bool dim_match_index(struct player *player, int ship_idx, int cc_RowIn, int cc_RowEnd, int cc_ColIn, int cc_ColEnd)`

    Verifica si la longitud del barco coincide con su tamaño. Llama a `codeError` si no coincide.

- `bool end_cell_is_water(struct player *player, int ship_idx, int cc_RowIn, int cc_RowEnd, int cc_ColIn, int cc_ColEnd)`

    Verifica si todas las celdas a lo largo del camino del barco son agua (libres). Llama a `codeError` si se detecta superposición.

- `void get_remain_fleet_cells(struct player *player)`

    Cuenta y actualiza el número de celdas de barco restantes para el jugador.

- `bool attack_in_range(struct player *player, int cc_Row, int cc_Col)`

    Verifica si un ataque está dentro del rango válido, considerando efectos especiales de cartas. Llama a `codeError` si está fuera de rango.

- `int getEnemyCellsInRow(struct player *player, struct player *enemy, int cc_Row)`

    Cuenta el número de celdas de barco enemigo en una fila dada.

- `int getEnemyCellsInCol(struct player *player, struct player *enemy, int cc_Col)`

    Cuenta el número de celdas de barco enemigo en una columna dada.

- `bool cell_is_valid_to_shot(struct player *player, struct player *enemy)`

    Verifica si la celda objetivo puede ser disparada (no ha sido disparada o destruida previamente). Llama a `codeError` si ya fue disparada.
