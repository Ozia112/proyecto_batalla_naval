#include "master.h" // Incluye todas las cabeceras necesarias para el proyecto

void impresion_ascii();

int main() {
	struct player player1, player2; // Declaración de dos jugadores.
    
    init_struct_player(&player1); // Inicializa el jugador 1.
    init_struct_player(&player2); // Inicializa el jugador 2.
    player1.player_index = 1;
    player2.player_index = 2;

    _mkdir("configs"); // Asegurarse de que el directorio configs existe
    set_CP_config(850); // Configura la consola para usar la codificación CP850.
    srand((unsigned int)time(NULL)); // Inicializa la semilla para números aleatorios.

    createCardsFile(); // Crea el archivo de configuración de cartas si no existe.
    createShipsFile(); // Crea el archivo de configuración de barcos si no existe.

    clear_screen(); // Limpia la pantalla al inicio del programa.
    
	//impresion_ascii(); // Imprime la tabla de caracteres ASCII.
	tittle_screen(); // Pantalla de bienvenida.
    enter_continuar_cls(); // Espera a que el usuario presione ENTER para continuar.
    
	main_menu(&player1, &player2); // Menu principal.

	return 0; // Fin del programa.
}

void impresion_ascii() {
	printf("Caracter\tValor\n");
    for (int i = 0; i <= 255; i++) {
        // Lista de caracteres no definidos en CP1252
        if (i == 127 || i == 129 || i == 141 || i == 143 || i == 144 || i == 157)
            printf(" (ND)\t\t%d\n", i);
        else if (i >= 32 && i <= 255)
            printf("   %c\t\t%d\n", i, i);
        else
            printf("  ( )\t\t%d\n", i);
    }

	enter_continuar_cls(); // Espera a que el usuario presione ENTER para continuar y limpia la pantalla.
}