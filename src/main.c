#include "master.h"

void impresion_ascii();

void impresion_colores_ansi();

int main() {
    debugMode = false;
    
	struct player player1, player2;
    srand((unsigned int)time(NULL));

    init_struct_player(&player1);
    init_struct_player(&player2);
    player1.player_index = 1;
    player2.player_index = 2;

    _mkdir("configs");
    set_CP_config(65001); // Set console code page to UTF-8

    createCardsFile();
    createShipsFile();

    //clear_screen();

    // Debug functions
	//impresion_ascii();
    //impresion_colores_ansi();
    
    // Flow of the game
	tittle_screen();
    enter_to_message("continuar", true); // Enter to continue
	main_menu(&player1, &player2);

	return 0;
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

	enter_to_message("continuar", true); // Espera a que el usuario presione ENTER para continuar y limpia la pantalla.
}

void impresion_colores_ansi() {
    // Colores regulares
    const char* color_names[] = {
        "Black", "Red", "Green", "Yellow", "Blue", "Magenta", "Cyan", "White"
    };
    int i;
    printf("Colores ANSI estándar (foreground):\n");
    for (i = 0; i < 8; i++) {
        printf("\033[0;3%dm%-10s\033[0m\n", i, color_names[i]);
    }
    printf("\nColores ANSI en negrita (bold):\n");
    for (i = 0; i < 8; i++) {
        printf("\033[1;3%dmBold %-8s\033[0m\n", i, color_names[i]);
    }
    printf("\nColores ANSI alta intensidad:\n");
    for (i = 0; i < 8; i++) {
        printf("\033[0;9%dmHigh Intensity %-6s\033[0m\n", i, color_names[i]);
    }
    printf("\nColores ANSI fondo estándar (background):\n");
    for (i = 0; i < 8; i++) {
        printf("\033[4%dmBackground %-7s\033[0m\n", i, color_names[i]);
    }
    printf("\nColores ANSI fondo alta intensidad:\n");
    for (i = 0; i < 8; i++) {
        printf("\033[10%dmHigh Intensity BG %-6s\033[0m\n", i, color_names[i]);
    }
    printf("\n\033[0mReset\033[0m\n");
    // Colores extendidos 256 ordenados por tono (hue)
    printf("\nColores ANSI extendidos (16-231) ordenados por tono:\n");
    // La paleta 16-231 es una matriz 6x6x6 (R,G,B), ordenada por HUE
    int r, g, b, idx = 16;
    // Orden: primero rojo, luego amarillo, verde, cian, azul, magenta
    for (g = 0; g < 6; g++) { // Verde
        for (r = 0; r < 6; r++) { // Rojo
            for (b = 0; b < 6; b++) { // Azul
                int color = 16 + 36*r + 6*g + b;
                printf("\033[38;5;%dm%3d\033[0m ", color, color);
                if ((b + 1) % 6 == 0) printf(" ");
            }
            printf(" ");
        }
        printf("\n");
    }
    // Grises
    printf("\nColores grises (232-255):\n");
    for (i = 232; i < 256; i++) {
        printf("\033[38;5;%dm%3d\033[0m ", i, i);
    }
    printf("\n\033[0m");
    enter_to_message("continuar", true); // Espera a que el usuario presione ENTER para continuar y limpia la pantalla.
}