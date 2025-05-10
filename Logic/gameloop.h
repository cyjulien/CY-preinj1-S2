#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Regenerate energy
void regenerateEnergy(Character *characters, int count) {
    for (int i = 0; i < count; i++) {
        if (characters[i].HP > 0) {
            int regen = (75.0 / (MAX_TEAM_MEMBERS_COUNT * 2)) * characters[i].SPD / 100.0;
            characters[i].energy += regen;
            if (characters[i].energy > MAX_ENERGY) characters[i].energy = MAX_ENERGY;
            printf("%s regenerates %d energy.\n", characters[i].name, regen);
        }
    }
}

// Perform action
void performAction(Character *performer, Character *target, int action) {
    Skill skill = performer->skills[action];

    if (performer->energy >= skill.cost) {
        performer->energy -= skill.cost;
        
        //If the target is dodging and the performer is an enemy he gets a chance of avoiding the attack
        if (strcmp("Enemy", skill.target) == 0 && target->isDodging && (rand() % 100) < target->DODGE) {
            printf("%s dodged the attack from %s!\n", target->name, performer->name);
            return;
        } 
        if (strcmp("HP", skill.affectTo) == 0 && skill.value != 0) {
            target->HP += skill.value;
            printf("%s uses ability %s on %s, it did %d %s. %s's HP is now %d.", performer->name, skill.name, target->name, abs(skill.value), skill.value<0?"damage":"heal", target->name, target->HP);
            if (strcmp("Null", skill.applyEffect) != 0) printf("%s is now under the %s effect.", target->name, "404");
            printf("\n");
        }
    } else {
        printf("%s does not have enough energy to perform %s!\n", performer->name, skill.name);
    }
}

// Sort characters by energy in descending order
int compareByEnergy(const void *a, const void *b) {
    Character *c1 = (Character *)a;
    Character *c2 = (Character *)b;
    return c2->energy - c1->energy;
}

// Handle actions for a character
void takeTurn(Character *actor, Character *allies, Character *opponents, int team) {
    if (actor->HP <= 0) return;
    printf("It's %s's turn from team: %d\n", actor->name, team);
    int action = 0;
    if (team == 1) { // Player
        printf("\n%s's turn! (HP: %d, Energy: %d)\n", actor->name, actor->HP, actor->energy);
        printf("0. Dodge\n");
        for (size_t i = 0; i < actor->skillsCount; i++)
        {
            printf("%d. %s, cost: %d.\n   %s\n", i+1, actor->skills[i].name, actor->skills[i].cost, actor->skills[i].desc);
        }
        
        printf("Your choice: ");
        /**
         * Safely takes in the user's input
         * If the users enters an invalid input the action will default to 0 (dodge)
         */
        char input[50];
        fgets(input, sizeof input, stdin);
        input[strcspn(input, "\n")] = 0;
        action = atoi(input);
    } else { // Enemy AI
        action = rand() % 4;
    }

    actor->isDodging = (action == 0);

    if (action >= 1 && action <= actor->skillsCount) {
        if (strcmp("Ally", actor->skills[action-1].target) == 0)
        {
            int target = rand() % MAX_TEAM_MEMBERS_COUNT;
            while (allies[target].HP <= 0) {
                target = rand() % MAX_TEAM_MEMBERS_COUNT;
            }
            performAction(actor, &allies[target], action-1);
        } else
        {
            int target = rand() % MAX_TEAM_MEMBERS_COUNT;
            while (opponents[target].HP <= 0) {
                target = rand() % MAX_TEAM_MEMBERS_COUNT;
            }
            performAction(actor, &opponents[target], action-1);
        }
    } else if (actor->energy >= DODGE_COST) {
        actor->energy -= DODGE_COST;
        printf("%s prepares to dodge with a %d%% chance.\n", actor->name, actor->DODGE);
    } else {
        printf("%s doesn't have enough energy to dodge. Resting this turn\n", actor->name);
    }
}

// Handle a full round
void handleRound(Character *allChars, Team players, Team enemies, int totalChars, int round) {
    printf("\n--- Round %d ---\n", round);

    qsort(allChars, totalChars, sizeof(Character), compareByEnergy);
    for (size_t i = 0; i < 6; i++)
    {
        printf("%d. %s\n", i, allChars[i].name);
    }
    printf("-------------\n");
    
    for (size_t i = 0; i < players.membersCount; i++) {
        if (strcmp(players.members[i].name, allChars[0].name) == 0) takeTurn(&players.members[i], players.members, enemies.members, 1);
    }
    for (size_t i = 0; i < enemies.membersCount; i++) {
        if (strcmp(enemies.members[i].name, allChars[0].name) == 0) takeTurn(&enemies.members[i], enemies.members, players.members, 0);
    }

    regenerateEnergy(players.members, players.membersCount);
    regenerateEnergy(enemies.members, enemies.membersCount);

    printf("\n--- Status after Round %d ---\n", round);
    for (size_t i = 0; i < players.membersCount; i++) {
        printf("%s - Energy: %d, HP: %d/%d\n", players.members[i].name, players.members[i].energy, players.members[i].HP, players.members[i].maxHP);
    }
    for (size_t i = 0; i < enemies.membersCount; i++) {
        printf("%s - Energy: %d, HP: %d/%d\n", enemies.members[i].name, enemies.members[i].energy, enemies.members[i].HP, enemies.members[i].maxHP);
    }
}