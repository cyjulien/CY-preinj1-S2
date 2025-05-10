#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Regenerate energy
void regenerateEnergy(Character *champs, int count) {
    for (int i = 0; i < count; i++) {
        if (champs[i].HP <= 0) continue;
        int regen = (75.0 / (MAX_TEAM_MEMBERS_COUNT * 2)) * champs[i].SPD / 100.0;
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
void takeTurn(Character *actor, Character *opponents, int team) {
    if (actor->HP <= 0) return;
    printf("It's %s's turn from team: %d\n", actor->name, team);
    int action = 0;
    if (team == 1) { // Player
        printf("\n%s's turn! (HP: %d, Energy: %d)\n", actor->name, actor->HP, actor->energy);
        printf("0. Dodge\n1. Ability 1 (50 dmg)\n2. Ability 2 (100 dmg)\n3. Ability 3 (150 dmg)\nYour choice: ");
        scanf("%d", &action);
    } else { // Enemy AI
        action = rand() % 4;
    }

    actor->isDodging = (action == 0);

    if (action >= 1 && action <= 3) {
        int target = rand() % MAX_TEAM_MEMBERS_COUNT;
        while (opponents[target].HP <= 0) {
            target = rand() % MAX_TEAM_MEMBERS_COUNT;
        }
        performAttack(actor, &opponents[target], action);
    } else if (action == 0) {
        printf("%s prepares to dodge with a %d%% chance.\n", actor->name, actor->DODGE);
    } else {
        printf("Invalid choice. %s skips the turn.\n", actor->name);
    }
}

// Handle a full round
void handleRound(Character *allChars, Team players, Team enemies, int totalChars, int round) {
    printf("\n--- Round %d ---\n", round);

    qsort(allChars, totalChars, sizeof(Character), compareByEnergy);

    for (int i = 0; i < totalChars; i++) {
        int actorTeam = 0;
        for (size_t j = 0; j < players.membersCount; j++) {
            if (strcmp(players.members[j].name, allChars[i].name) == 0) actorTeam = 1;
        }
        takeTurn(&allChars[i], actorTeam == 0 ? players.members : enemies.members, actorTeam);
    }

    regenerateEnergy(allChars, totalChars);

    printf("\n--- Status after Round %d ---\n", round);
    for (int i = 0; i < totalChars; i++) {
        printf("%s - Energy: %d, HP: %d/%d\n", allChars[i].name, allChars[i].energy, allChars[i].HP, allChars[i].maxHP);
    }
}