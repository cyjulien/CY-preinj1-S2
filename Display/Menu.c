#include <stdio.h>
#include <stdlib.h>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"

void combat_menu() {
    printf("A\n");
}

void equipe_menu() {
    printf("B\n");
}

void boutique_menu() {
    printf("C\n");
}

void statistiques_menu() {
    printf("D\n");
}

void options_menu() {
    printf("E\n");
}

void main_menu() {
    int choice = 0;

    while (1) {
        printf("\n=== Menu Principal ===\n");
        printf(RED "1. Combat" RESET " - Affrontez des ennemis dans des batailles épiques.\n");
        printf(GREEN "2. Équipe" RESET " - Gérez et améliorez votre équipe.\n");
        printf(BLUE "3. Boutique" RESET " - Achetez des objets et équipements.\n");
        printf("6. Quitter - Quittez le jeu.\n");
        printf("Choisissez une option: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Entrée invalide, veuillez réessayer.\n");
            while (getchar() != '\n'); // Vider le buffer
            continue;
        }

        switch (choice) {
            case 1:
                combat_menu();
                break;
            case 2:
                equipe_menu();
                break;
            case 3:
                boutique_menu();
                break;
            case 4:
                statistiques_menu();
                break;
            case 5:
                options_menu();
                break;
            case 6:
                printf("Au revoir!\n");
                exit(0);
            default:
                printf("Choix invalide, veuillez réessayer.\n");
        }
    }
}

int main() {
    main_menu();
    return 0;
}