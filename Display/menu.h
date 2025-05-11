#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"

// Fonction pour lire une touche sans appuyer sur Entrée
char getch() {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt); // Sauvegarde des paramètres du terminal
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Désactive le mode canonique et l'écho
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restauration des paramètres
    return ch;
}

void combat_menu() {
    printf("A\n");
}

void team_menu() {
    printf("B\n");
}

void shop_menu() {
    printf("C\n");
}

void stats_menu() {
    printf("D\n");
}

void options_menu() {
    printf("E\n");
}

void main_menu() {
    int choice = 0;
    int max_options = 6;
    char key;

    while (1) {
        system("clear"); // Efface l'écran sous Linux
        printf("\n=== Main Menu ===\n");
        printf("%s1. Combat - Face enemies in epic battles.%s\n", choice == 0 ? RED : RESET, RESET);
        printf("%s2. Team - Manage and upgrade your team.%s\n", choice == 1 ? GREEN : RESET, RESET);
        printf("%s3. Shop - Buy items and equipment.%s\n", choice == 2 ? BLUE : RESET, RESET);
        printf("%s6. Quit - Exit the game.%s\n", choice == 5 ? RED : RESET, RESET);

        key = getch(); // Lecture d'une touche sans appuyer sur Entrée

        if (key == '\033') { // Séquence d'échappement pour les flèches
            getch(); // Ignore le caractère '['
            key = getch();
            if (key == 'A') { // Flèche haut
                choice = (choice - 1 + max_options) % max_options;
            } else if (key == 'B') { // Flèche bas
                choice = (choice + 1) % max_options;
            }
        } else if (key == '\n') { // Touche Entrée
            switch (choice) {
                case 0:
                    combat_menu();
                    break;
                case 1:
                    team_menu();
                    break;
                case 2:
                    shop_menu();
                    break;
                case 3:
                    stats_menu();
                    break;
                case 4:
                    options_menu();
                    break;
                case 5:
                    printf("Goodbye!\n");
                    exit(0);
            }
            printf("Press any key to return to the menu...");
            getch();
        }
    }
}

int main() {
    main_menu();
    return 0;
}
