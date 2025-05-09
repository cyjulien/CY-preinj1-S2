#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ENERGY 500
#define MAX_HP 1000
#define NUM_CHAMPIONS 3
#define ACTION_COST 100

// Character structure
typedef struct {
    char name[50];
    int energy;
    int HP;
    int SPD;
    int isDodging;
    int dodgeChance; // between 0 and 40
    int team; // 0 for player, 1 for enemy
} Character;

// Initialize a champion
void initializeChampion(Character *champion, const char *name, int team) {
    strcpy(champion->name, name);
    champion->energy = MAX_ENERGY;
    champion->HP = MAX_HP;
    champion->SPD = rand() % 101;
    champion->isDodging = 0;
    champion->dodgeChance = rand() % 41;
    champion->team = team;
    printf("Champion %s initialized (Team %d) with %d energy, %d HP, SPD %d, Dodge Chance %d%%\n",
           champion->name, team, champion->energy, champion->HP, champion->SPD, champion->dodgeChance);
}

// Regenerate energy
void regenerateEnergy(Character *champs, int count) {
    for (int i = 0; i < count; i++) {
        if (champs[i].HP <= 0) continue;
        int regen = (75.0 / (NUM_CHAMPIONS * 2)) * champs[i].SPD / 100.0;
        champs[i].energy += regen;
        if (champs[i].energy > MAX_ENERGY) champs[i].energy = MAX_ENERGY;
        printf("%s regenerates %d energy.\n", champs[i].name, regen);
    }
}

// Perform attack
void performAttack(Character *attacker, Character *defender, int ability) {
    int damage = 0;
    switch (ability) {
        case 1: damage = 50; break;
        case 2: damage = 100; break;
        case 3: damage = 150; break;
        default: return;
    }

    if (attacker->energy >= ACTION_COST) {
        attacker->energy -= ACTION_COST;
        
        // We check the speed difference between the attacker and the defender
        // Champion has a chance to dodge only if his speed is above the attcker
        // The chances to dodge can't go above 40%
        int chanceDodge = defender->SPD - attacker->SPD > 40 ? 40 : defender->SPD - attacker->SPD;
        if(chanceDodge < 0) chanceDodge = 0;
        
        if ((rand() % 100) < chanceDodge) {
            printf("%s dodged the attack from %s!\n", defender->name, attacker->name);
        } else {
            defender->HP -= damage;
            printf("%s uses ability %d on %s, dealing %d damage! %s's HP is now %d.\n",
                   attacker->name, ability, defender->name, damage, defender->name, defender->HP);
        }
    } else {
        printf("%s does not have enough energy to perform the attack!\n", attacker->name);
    }
}

// Sort characters by energy in descending order
int compareByEnergy(const void *a, const void *b) {
    Character *c1 = (Character *)a;
    Character *c2 = (Character *)b;
    return c2->energy - c1->energy;
}

// Handle actions for a character
void takeTurn(Character *actor, Character *opponents) {
    if (actor->HP <= 0) return;

    int action = 0;
    if (actor->team == 0) { // Player
        printf("\n%s's turn! (HP: %d, Energy: %d)\n", actor->name, actor->HP, actor->energy);
        printf("0. Dodge\n1. Ability 1 (50 dmg)\n2. Ability 2 (100 dmg)\n3. Ability 3 (150 dmg)\nYour choice: ");
        scanf("%d", &action);
    } else { // Enemy AI
        action = rand() % 4;
    }

    actor->isDodging = (action == 0);

    if (action >= 1 && action <= 3) {
        int target = rand() % NUM_CHAMPIONS;
        while (opponents[target].HP <= 0) {
            target = rand() % NUM_CHAMPIONS;
        }
        performAttack(actor, &opponents[target], action);
    } else if (action == 0) {
        printf("%s prepares to dodge with a %d%% chance.\n", actor->name, actor->dodgeChance);
    } else {
        printf("Invalid choice. %s skips the turn.\n", actor->name);
    }
}

// Handle a full round
void handleRound(Character *allChars, Character *players, Character *enemies, int totalChars, int round) {
    printf("\n--- Round %d ---\n", round);

    qsort(allChars, totalChars, sizeof(Character), compareByEnergy);

    for (int i = 0; i < totalChars; i++) {
        if (allChars[i].team == 0) {
            takeTurn(&allChars[i], enemies);
        } else {
            takeTurn(&allChars[i], players);
        }
    }

    regenerateEnergy(allChars, totalChars);

    printf("\n--- Status after Round %d ---\n", round);
    for (int i = 0; i < totalChars; i++) {
        printf("%s - Team %d - Energy: %d, HP: %d\n", allChars[i].name, allChars[i].team, allChars[i].energy, allChars[i].HP);
    }
}

// Main game logic
int main() {
    srand(time(NULL));

    Character players[NUM_CHAMPIONS];
    Character enemies[NUM_CHAMPIONS];
    Character allChars[NUM_CHAMPIONS * 2];

    for (int i = 0; i < NUM_CHAMPIONS; i++) {
        char name[20];
        sprintf(name, "Player%d", i + 1);
        initializeChampion(&players[i], name, 0);
        allChars[i] = players[i];
    }
    for (int i = 0; i < NUM_CHAMPIONS; i++) {
        char name[20];
        sprintf(name, "Enemy%d", i + 1);
        initializeChampion(&enemies[i], name, 1);
        allChars[i + NUM_CHAMPIONS] = enemies[i];
    }

    int round = 1;
    while (1) {
        handleRound(allChars, players, enemies, NUM_CHAMPIONS * 2, round++);
    }

    return 0;
}