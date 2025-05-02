#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ENERGY 100
#define NUM_CHAMPIONS 3
#define ACTION_COST 20

// Champion structure
typedef struct {
    char name[50];
    int energy;
    int attackSpeed;  // Attack speed (random between 0 and 100)
} Champion;

// Team structure
typedef struct {
    Champion champions[NUM_CHAMPIONS];
} Team;

// Function to generate random attack speed
int generateRandomAttackSpeed() {
    return rand() % 101;  // Random number between 0 and 100
}

// Function to initialize the champions
void initializeChampion(Champion *champion, const char *name) {
    strcpy(champion->name, name);
    champion->energy = MAX_ENERGY;
    champion->attackSpeed = generateRandomAttackSpeed();
    printf("Champion %d %d\n", champion->energy, champion->attackSpeed);
}

// Function to calculate energy regeneration for each champion
void regenerateEnergy(Team *team, int numAllies, int numEnemies) {
    int totalParticipants = numAllies + numEnemies;
    for (int i = 0; i < NUM_CHAMPIONS; i++) {
        float regenF = (10 / totalParticipants) * team->champions[i].attackSpeed/100;
        float ratio = (10 / totalParticipants);
        int regen = regenF;
        team->champions[i].energy += regen;
        printf("Regenerating %f %f %d energy for %s\n", ratio, team->champions[i].attackSpeed/100, regen, team->champions[i].name);
        if (team->champions[i].energy > MAX_ENERGY) {
            team->champions[i].energy = MAX_ENERGY; // Cap the energy at 100
        }
    }
}

// Function to perform an action (consuming energy)
void performAction(Champion *champion) {
    if (champion->energy >= ACTION_COST) {
        champion->energy -= ACTION_COST;
        printf("%s performs an action, energy left: %d\n", champion->name, champion->energy);
    } else {
        printf("%s does not have enough energy to perform an action!\n", champion->name);
    }
}

// Function to find the champion with the highest energy
Champion* findChampionWithMaxEnergy(Team *team) {
    Champion *maxEnergyChampion = &team->champions[0];
    for (int i = 1; i < NUM_CHAMPIONS; i++) {
        if (team->champions[i].energy > maxEnergyChampion->energy) {
            maxEnergyChampion = &team->champions[i];
        }
    }
    return maxEnergyChampion;
}

// Function to sort champions by attack speed for the first turn
void sortChampionsByAttackSpeed(Team *team) {
    for (int i = 0; i < NUM_CHAMPIONS - 1; i++) {
        for (int j = i + 1; j < NUM_CHAMPIONS; j++) {
            if (team->champions[i].attackSpeed < team->champions[j].attackSpeed) {
                // Swap champions[i] and champions[j]
                Champion temp = team->champions[i];
                team->champions[i] = team->champions[j];
                team->champions[j] = temp;
            }
        }
    }
}

// Function to handle the turn
void handleTurn(Team *team1, Team *team2, int turn) {
    printf("\nTurn %d\n", turn);

    // First turn: The champion with the highest attack speed goes first
    if (turn == 1) {
        sortChampionsByAttackSpeed(team1);
        sortChampionsByAttackSpeed(team2);
    }

    // For subsequent turns, the champion with the highest energy goes first
    if (turn > 1) {
        Champion *firstPlayer = findChampionWithMaxEnergy(team1);
        Champion *secondPlayer = findChampionWithMaxEnergy(team2);

        // Perform actions for the champions with the highest energy
        if (firstPlayer->energy >= ACTION_COST) {
            performAction(firstPlayer);
        }
        if (secondPlayer->energy >= ACTION_COST) {
            performAction(secondPlayer);
        }
    }

    // Regenerate energy for both teams
    regenerateEnergy(team1, NUM_CHAMPIONS, NUM_CHAMPIONS);
    regenerateEnergy(team2, NUM_CHAMPIONS, NUM_CHAMPIONS);

    printf("\nEnd of Turn %d\n", turn);
}

int main() {
    srand(time(NULL));

    // Initialize teams
    Team team1, team2;

    initializeChampion(&team1.champions[0], "Champion1_Team1");
    initializeChampion(&team1.champions[1], "Champion2_Team1");
    initializeChampion(&team1.champions[2], "Champion3_Team1");

    initializeChampion(&team2.champions[0], "Champion1_Team2");
    initializeChampion(&team2.champions[1], "Champion2_Team2");
    initializeChampion(&team2.champions[2], "Champion3_Team2");

    // Simulate turns
    for (int turn = 1; turn <= 5; turn++) {
        handleTurn(&team1, &team2, turn);
    }

    return 0;
}
