
/**
 * @bug Console output row index is incorrect, it should be between A and J 
 * (Observed: row index displayed as '@', Expected: row index between 'A' and 'J')
 * @details:
 * Tablero de guerra actualizado:
 * No habian barcos en la fila: @.
 * Presione ENTER para continuar
 * @cond: Despues de realizar un bombardeo en la fila 0
 * Fix: Ensure that when there are no ships in the row, the output displays prevRowInput + 'A' instead of prevHitRow + 'A'.
 */

/** 
 * @bug Function 'bomb_column' does not handle the board modification correctly
 * @details: CONSOLE OUTPUT
 * Disparo fallido en F,1! Disparaste al agua.
 * Disparo fallido en G,1! Disparaste al agua.
 * Disparo fallido en H,1! Disparaste al agua.
 * Disparo fallido en J,1! Disparaste al agua.
 * Presione ENTER para continuar
 * @brief: The bomb report is not printed correctly; rows D and E, which contain ships that have not been shot at, 
 * are missing from the output. This may be due to a logic error in the reporting function or an output formatting 
 * issue—please verify the implementation.
 * @cond: Torre de ventaja mode is active.
 * 
 * @details: Later output console:
 * Tablero de guerra actualizado:
 * Bombardeo exitoso en la columna: 1
 * Casillas enemigas destruidas: 5
 * Presione ENTER para continuar
 * @see @brief above: The bomb report is not printed correctly, see earlier description.
 */