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

    while (1) {
        printf("\n=== Main Menu ===\n");
        printf(RED "1. Combat" RESET " - Face enemies in epic battles.\n");
        printf(GREEN "2. Team" RESET " - Manage and upgrade your team.\n");
        printf(BLUE "3. Shop" RESET " - Buy items and equipment.\n");
        printf("6. Quit - Exit the game.\n");
        printf("Choose an option: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input, please try again.\n");
            while (getchar() != '\n'); // Clear the buffer
            continue;
        }

        switch (choice) {
            case 1:
                combat_menu();
                break;
            case 2:
                team_menu();
                break;
            case 3:
                shop_menu();
                break;
            case 4:
                stats_menu();
                break;
            case 5:
                options_menu();
                break;
            case 6:
                printf("Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice, please try again.\n");
        }
    }
}

int main() {
    main_menu();
    return 0;
}
