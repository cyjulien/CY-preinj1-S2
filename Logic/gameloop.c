#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ENERGY 1000
#define NUM_CHAMPIONS 3
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

// Function to initialize the champions
void initializeChampion(Champion *champion, const char *name) {
    strcpy(champion->name, name);
    champion->energy = MAX_ENERGY;
    champion->SPD = generateRandomSPD();
    printf("Champion %d %d\n", champion->energy, champion->SPD);
}

// Function to calculate energy regeneration for each champion
void regenerateEnergy(Team *team, int numAllies, int numEnemies) {
    int totalParticipants = numAllies + numEnemies;
    for (int i = 0; i < NUM_CHAMPIONS; i++) {
        int regen = 10 * (10.0 / totalParticipants) * 1+team->champions[i].SPD/100.0;
        team->champions[i].energy += regen;
        printf("Regenerating %d energy for %s\n", regen, team->champions[i].name);
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
void sortChampionsBySPD(Team *team) {
    for (int i = 0; i < NUM_CHAMPIONS - 1; i++) {
        for (int j = i + 1; j < NUM_CHAMPIONS; j++) {
            if (team->champions[i].SPD < team->champions[j].SPD) {
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
        sortChampionsBySPD(team1);
        sortChampionsBySPD(team2);
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