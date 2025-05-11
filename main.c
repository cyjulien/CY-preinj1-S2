#include "main.h"

int main(int argc, char const *argv[]) {
    printf("---------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("Execution started.\n");

    while (1)
    {
        printf("\n" COLOR_YELLOW "=== MAIN MENU ===\n" COLOR_RESET);
        printf("1. Start New Combat\n");
        printf("2. Exit\n");
        printf("Your choice: ");
        char input[10];
        fgets(input, sizeof input, stdin);
        int choice = atoi(input);
        while (choice != 1 && choice != 2) {
            printf("Invalid choice. Please enter 1 or 2: ");
            fgets(input, sizeof input, stdin);
            choice = atoi(input);
        }
        if (choice == 2) {
            printf("Exiting the game. Goodbye!\n");
            break;
        }
        Team players = newEmptyTeam();
        Team enemies = newEmptyTeam();
        /*while (players.membersCount == 0) {
            char teamName[50];
            printf("Enter the name of the team you want to play with: ");
            fgets(teamName, sizeof teamName, stdin);
            players = getTeam(teamName);
        }
        while (enemies.membersCount == 0) {
            char teamName[50];
            printf("Available teams:\n");
            printf("  - Team1\n");
            printf("Enter the name of the team you want to play against: ");
            fgets(teamName, sizeof teamName, stdin);
            enemies = getTeam(teamName);
        }*/
        players = getTeam("Team2");
        enemies = getTeam("Example");
        printf("Starting new combat...\n");
        srand(time(NULL));

        Character allChars[MAX_TEAM_MEMBERS_COUNT * 2];

        int round = 1;
        int playersAlive = 3;
        int enemiesAlive = 3;
        
        char *dialogueMessage = NULL;
        dialogueMessage = malloc(750 * sizeof(char));
        snprintf(dialogueMessage, 750, "");
        
        time_t start = time(NULL);

        while (enemiesAlive > 0 && playersAlive > 0) {
            if (time(NULL) - start >= TURN_COOLDOWN) {
                playersAlive = 0;
                for (int i = 0; i < players.membersCount; i++) {
                    if (players.members[i].HP > 0) {
                        playersAlive++;
                    }
                    allChars[i] = players.members[i];
                }
                enemiesAlive = 0;
                for (int i = 0; i < enemies.membersCount; i++) {
                    if (enemies.members[i].HP > 0) {
                        enemiesAlive++;
                    }
                    allChars[i + players.membersCount] = enemies.members[i];
                }
                sortCharactersByEnergy(allChars, MAX_TEAM_MEMBERS_COUNT * 2);
                handleTurn(allChars, players, enemies, MAX_TEAM_MEMBERS_COUNT * 2, dialogueMessage);
                system("clear");
                system("cls");
                sortCharactersByEnergy(allChars, MAX_TEAM_MEMBERS_COUNT * 2);
                displayFullInterface(players, enemies, dialogueMessage, allChars);
                start = time(NULL);
            }
        }
        printf(COLOR_YELLOW "\n[BATTLE RESULT]--------------------------------------------------------------\n" COLOR_RESET);

        if (playersAlive == 0) {
            printf("  " COLOR_RED "Enemies win! All player characters have fallen.\n" COLOR_RESET);
        } else if (enemiesAlive == 0) {
            printf("  " COLOR_GREEN "Players win! All enemies have been defeated.\n" COLOR_RESET);
        } else {
            printf("  " COLOR_CYAN "It's a draw! No combatants remain.\n" COLOR_RESET);
        }
        
        free(dialogueMessage);
        //*dialogueMessage = NULL;
        for (int i = 0; i < players.membersCount; i++) {
            free(players.members[i].name);
            free(players.members[i].skills);
            free(players.members[i].effects);
        }
        for (int i = 0; i < enemies.membersCount; i++) {
            free(enemies.members[i].name);
            free(enemies.members[i].skills);
            free(enemies.members[i].effects);
        }
    }

    printf("Execution ended.\n");
    return 0;
}