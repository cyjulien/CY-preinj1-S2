#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ENERGY 500
#define MAX_HP 1000  // Points de vie maximaux
#define NUM_CHAMPIONS 1  // Un seul champion par équipe
#define ACTION_COST 100
#define MAX_DAMAGE 150  // Dégâts maximum d'une attaque
/*
// Champion structure
typedef struct {
    char name[50];
    int energy;
    int HP;  // Points de vie
    int SPD; // Speed (random between 0 and 100)
} Champion;
*/
// Team structure
typedef struct {
    Character champions[NUM_CHAMPIONS];
} Team;

// Function to generate random attack speed
int generateRandomSPD() {
    return rand() % 101;  // Random number between 0 and 100
}
/*
// Function to initialize the champion
void initializeChampion(Champion *champion, const char *name) {
    strcpy(champion->name, name);
    champion->energy = MAX_ENERGY;
    champion->HP = MAX_HP;
    champion->SPD = generateRandomSPD();
    printf("Champion %s initialized with %d energy, %d HP and SPD %d\n", champion->name, champion->energy, champion->HP, champion->SPD);
}
*/
// Function to regenerate energy for each champion
void regenerateEnergy(Team *team) {
    for (int i = 0; i < NUM_CHAMPIONS; i++) {
        int regen = (75.0 / (NUM_CHAMPIONS*2)) * team->champions[i].SPD/100.0;  // Simple regeneration formula
        team->champions[i].energy += regen;
        printf("%s regenerates %d energy.\n", team->champions[i].name, regen);
        if (team->champions[i].energy > MAX_ENERGY) {
            team->champions[i].energy = MAX_ENERGY; // Cap the energy at MAX_ENERGY
        }
    }
}

// Function to perform an attack
void performAttack(Character *attacker, Character *defender) {
    if (attacker->energy >= ACTION_COST) {
        int damage = attacker->ATK*0.8;  // Random damage based on SPD
        defender->HP -= damage;
        attacker->energy -= ACTION_COST;
        printf("%s attacks %s, dealing %d damage! %s's HP is now %d and energy remaining is %d.\n", attacker->name, defender->name, damage, defender->name, defender->HP, attacker->energy);
    } else {
        printf("%s does not have enough energy to perform an attack!\n", attacker->name);
    }
}

// Function to handle the player's turn
void playerTurn(Team *player, Team *enemy) {
    int actionChoice;
    printf("\nIt's your turn! Do you want to:\n");
    printf("1. Perform an attack\n");
    printf("0. Pass your turn\n");
    printf("Enter your choice (1 or 0): ");
    scanf("%d", &actionChoice);

    if (actionChoice == 1) {
        performAttack(&player->champions[0], &enemy->champions[0]);
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
        performAttack(&enemy->champions[0], &player->champions[0]);
    } else {
        printf("Enemy does not have enough energy to perform an attack!\n");
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

    // Display status
    printf("\nStatus after your turn:\n");
    printf("%s - Energy: %d, HP: %d\n", player->champions[0].name, player->champions[0].energy, player->champions[0].HP);
    printf("%s - Energy: %d, HP: %d\n", enemy->champions[0].name, enemy->champions[0].energy, enemy->champions[0].HP);

    // Enemy's turn
    enemyTurn(enemy, player);

    // Regenerate energy for both players after the enemy's turn
    regenerateEnergy(player);
    regenerateEnergy(enemy);

    // Display status
    printf("\nStatus after enemy's turn:\n");
    printf("%s - Energy: %d, HP: %d\n", player->champions[0].name, player->champions[0].energy, player->champions[0].HP);
    printf("%s - Energy: %d, HP: %d\n", enemy->champions[0].name, enemy->champions[0].energy, enemy->champions[0].HP);

    printf("\n--- End of Round %d ---\n", round);
}
