#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ENERGY 1000
#define NUM_CHAMPIONS 1  // Un seul champion par équipe
#define ACTION_COST 200

// Champion structure
typedef struct {
    char name[50];
    int energy;
    int SPD;  // Speed (random between 0 and 100)
} Champion;

// Team structure
typedef struct {
    Champion champions[NUM_CHAMPIONS];
} Team;

// Function to generate random attack speed
int generateRandomSPD() {
    return rand() % 101;  // Random number between 0 and 100
}

// Function to initialize the champion
void initializeChampion(Champion *champion, const char *name) {
    strcpy(champion->name, name);
    champion->energy = MAX_ENERGY;
    champion->SPD = generateRandomSPD();
    printf("Champion %s initialized with %d energy and SPD %d\n", champion->name, champion->energy, champion->SPD);
}

// Function to regenerate energy for each champion
void regenerateEnergy(Team *team) {
    for (int i = 0; i < NUM_CHAMPIONS; i++) {
        int regen = 10 + (team->champions[i].SPD / 10);  // Simple regeneration formula
        team->champions[i].energy += regen;
        printf("Regenerating %d energy for %s\n", regen, team->champions[i].name);
        if (team->champions[i].energy > MAX_ENERGY) {
            team->champions[i].energy = MAX_ENERGY; // Cap the energy at MAX_ENERGY
        }
    }
}

// Function to perform an action (consume energy)
void performAction(Champion *champion) {
    if (champion->energy >= ACTION_COST) {
        champion->energy -= ACTION_COST;
        printf("%s performs an action, energy left: %d\n", champion->name, champion->energy);
    } else {
        printf("%s does not have enough energy to perform an action!\n", champion->name);
    }
}

// Function to handle the player's turn
void playerTurn(Team *player, Team *enemy) {
    int actionChoice;
    printf("\nIt's your turn! Do you want to:\n");
    printf("1. Perform an action\n");
    printf("0. Pass your turn\n");
    printf("Enter your choice (1 or 0): ");
    scanf("%d", &actionChoice);

    if (actionChoice == 1) {
        if (player->champions[0].energy >= ACTION_COST) {
            performAction(&player->champions[0]);
        } else {
            printf("You do not have enough energy to perform an action!\n");
        }
    } else if (actionChoice == 0) {
        printf("You passed your turn.\n");
    } else {
        printf("Invalid choice, turn skipped.\n");
    }
}

// Function to handle the enemy's turn (AI-controlled)
void enemyTurn(Team *enemy, Team *player) {
    printf("\nEnemy's turn!\n");
    if (enemy->champions[0].energy >= ACTION_COST) {
        performAction(&enemy->champions[0]);
    } else {
        printf("Enemy does not have enough energy to perform an action!\n");
    }
}

// Function to handle one round of the game
void handleRound(Team *player, Team *enemy, int round) {
    printf("\n--- Round %d ---\n", round);

    // Player's turn
    playerTurn(player, enemy);
    
    // Regenerate energy for both players after the player's turn
    regenerateEnergy(player);
    regenerateEnergy(enemy);

    // Enemy's turn
    enemyTurn(enemy, player);

    // Regenerate energy for both players after the enemy's turn
    regenerateEnergy(player);
    regenerateEnergy(enemy);

    printf("\n--- End of Round %d ---\n", round);
}

int main() {
    srand(time(NULL)); // Initialize random seed

    Team player, enemy;

    // Initialize the player and enemy champions
    initializeChampion(&player.champions[0], "Player Champion");
    initializeChampion(&enemy.champions[0], "Enemy Champion");

    int round = 1;
    
    while (player.champions[0].energy > 0 && enemy.champions[0].energy > 0) {
        handleRound(&player, &enemy, round);
        round++;
    }

    // Check who won
    if (player.champions[0].energy > 0) {
        printf("You win!\n");
    } else if (enemy.champions[0].energy > 0) {
        printf("You lose! Enemy wins.\n");
    } else {
        printf("It's a draw!\n");
    }

    return 0;
}


