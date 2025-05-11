#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Regenerate energy
void regenerateEnergy(Character *characters, int count) {
    for (int i = 0; i < count; i++) {
        if (characters[i].HP > 0) {
            int regen = (50.0 / (count)) * (1 + characters[i].SPD / 100.0);
            characters[i].energy += regen;
            if (characters[i].energy > MAX_ENERGY) characters[i].energy = MAX_ENERGY;
            printf("%s regenerates %d energy.\n", characters[i].name, regen);
        }
    }
}

void applyValueToHp(Character *performer, Character *targets, Skill skill, int value) { // Helper function to avoid code duplication
    targets[value].HP += skill.value;
    if (targets[value].HP < 0) targets[value].HP = 0;
    if (targets[value].HP > targets[value].maxHP) targets[value].HP = targets[value].maxHP;
    printf("%s uses ability %s on %s, it did %d %s. %s's HP is now %d.", performer->name, skill.name, targets[value].name, abs(skill.value), skill.value<0?"damage":"heal", targets[value].name, targets[value].HP);
    if (strcmp("Null", skill.applyEffect) != 0) {
        Effect effect = getEffect(skill.applyEffect);
        int effectAlreadyApplied = 0;
        for (size_t i = 0; i < targets[value].effectsCount; i++) {
            if (strcmp(targets[value].effects[i].name, effect.name) == 0) {
                effectAlreadyApplied = 1;
                targets[value].effects[i].remaining = effect.duration;
                targets[value].effects[i].value = effect.value;
                printf(" %s is under the %s effect for %d turns.", targets[value].name, effect.name, effect.duration);
            }
        }
        if (!effectAlreadyApplied) {
            targets[value].effects[targets[value].effectsCount] = effect;
            targets[value].effectsCount++;
            printf(" %s is now under the %s effect for %d turns.", targets[value].name, effect.name, effect.duration);
        }
    }
    printf("\n");
}
void applyValueToEnergy(Character *performer, Character *targets, Skill skill, int value) { // Helper function to avoid code duplication
    targets[value].energy += skill.value;
    if (targets[value].energy > MAX_ENERGY) targets[value].energy = MAX_ENERGY;
    printf("%s %s %s, %s's energy is now %d.", performer->name, skill.value<0?"removes energy from":"regenerates energy to", targets[value].name, targets[value].name, targets[value].energy);
    if (strcmp("Null", skill.applyEffect) != 0) {
        Effect effect = getEffect(skill.applyEffect);
        int effectAlreadyApplied = 0;
        for (size_t i = 0; i < targets[value].effectsCount; i++) {
            if (strcmp(targets[value].effects[i].name, effect.name) == 0) {
                effectAlreadyApplied = 1;
                targets[value].effects[i].remaining = effect.duration;
                targets[value].effects[i].value = effect.value;
                printf(" %s is under the %s effect for %d turns.", targets[value].name, effect.name, effect.duration);
            }
        }
        if (!effectAlreadyApplied) {
            targets[value].effects[targets[value].effectsCount] = effect;
            targets[value].effectsCount++;
            printf(" %s is now under the %s effect for %d turns.", targets[value].name, effect.name, effect.duration);
        }
    }
    printf("\n");
}
// Perform action
void performAction(Character *performer, Character *targets, int target, int action) {
    Skill skill = performer->skills[action];

    if (performer->energy >= skill.cost) {
        performer->energy -= skill.cost;
        //If the target is dodging and the performer is an enemy he gets a chance of avoiding the attack
        printf("%s dodging: %d\n", targets[target].name, targets[target].isDodging);
        if (strcmp("Enemy", skill.target) == 0 && targets[target].isDodging && (rand() % 100) < targets[target].DODGE) {
            printf("%s dodged the attack from %s!\n", targets[target].name, performer->name);
        } else if (strcmp("HP", skill.affectTo) == 0 && skill.value != 0) {
            if (strcmp("Enemy", skill.target) == 0 && targets[target].isDodging) printf("%s attempted to dodge but failed.\n", targets[target].name);
            //Deal damage to the targeted enemy
            if (skill.nTarget >= 1) {
                applyValueToHp(performer, targets, skill, target);
            }
            //For multiple targets, if odd then deal damage to the left and right, if even then deal damage to the left or right randomly
            if (skill.nTarget > 1){
                if (skill.nTarget%2 == 0){
                    for (size_t i = 1; i < skill.nTarget - 2; i++) {
                        applyValueToHp(performer, targets, skill, target+i);
                    }
                    for (size_t i = 1; i < skill.nTarget - 2; i++) {
                        applyValueToHp(performer, targets, skill, target-i);
                    }
                    if (target <= 0) {
                        //Target with no one to the left
                        applyValueToHp(performer, targets, skill, target+(skill.nTarget/2));
                    } else if (target >= MAX_TEAM_MEMBERS_COUNT-1) {
                        //Target with no one to the right
                        applyValueToHp(performer, targets, skill, target-(skill.nTarget/2));
                    } else {
                        //Target with other targets on both sides
                        applyValueToHp(performer, targets, skill, target + (rand()%2==0?-1*(skill.nTarget/2):1*(skill.nTarget/2)));
                    }
                } else {
                    for (size_t i = 1; i < ((skill.nTarget - 1)/2)+1; i++) {
                        if (target+i < MAX_TEAM_MEMBERS_COUNT) applyValueToHp(performer, targets, skill, target+i);
                    }
                    for (size_t i = 1; i < ((skill.nTarget - 1))/2+1; i++) {
                        if (target-i >= 0) applyValueToHp(performer, targets, skill, target-i);
                    }
                }
            }
        } else if (strcmp("Energy", skill.affectTo) == 0 && skill.value != 0) {
            if (strcmp("Enemy", skill.target) == 0 && targets[target].isDodging) printf("%s attempted to dodge but failed.\n", targets[target].name);
            if (skill.nTarget >= 1) {
                applyValueToEnergy(performer, targets, skill, target);
            }
            //For multiple targets, if odd then deal damage to the left and right, if even then deal damage to the left or right randomly
            if (skill.nTarget > 1){
                if (skill.nTarget%2 == 0){
                    for (size_t i = 1; i < skill.nTarget - 2; i++) {
                        applyValueToEnergy(performer, targets, skill, target+i);
                    }
                    for (size_t i = 1; i < skill.nTarget - 2; i++) {
                        applyValueToEnergy(performer, targets, skill, target-i);
                    }
                    if (target <= 0) {
                        //Target with no one to the left
                        applyValueToEnergy(performer, targets, skill, target+(skill.nTarget/2));
                    } else if (target >= MAX_TEAM_MEMBERS_COUNT-1) {
                        //Target with no one to the right
                        applyValueToEnergy(performer, targets, skill, target-(skill.nTarget/2));
                    } else {
                        //Target with other targets on both sides
                        applyValueToEnergy(performer, targets, skill, target + (rand()%2==0?-1*(skill.nTarget/2):1*(skill.nTarget/2)));
                    }
                } else {
                    for (size_t i = 1; i < ((skill.nTarget - 1)/2)+1; i++) {
                        if (target+i < MAX_TEAM_MEMBERS_COUNT) applyValueToEnergy(performer, targets, skill, target+i);
                    }
                    for (size_t i = 1; i < ((skill.nTarget - 1))/2+1; i++) {
                        if (target-i >= 0) applyValueToEnergy(performer, targets, skill, target-i);
                    }
                }
            }
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
    if (actor->HP <= 0) return; // Skip if dead
    printf("It's %s's turn from team: %d\n", actor->name, team);
    //Count down effects
    for (size_t i = 0; i < actor->effectsCount; i++) {
        if (actor->effects[i].remaining > 0) {
            if (actor->effects[i].remaining == 0) {
                printf("%s's %s effect has ended.\n", actor->name, actor->effects[i].name);
                // Remove the effect
                for (size_t j = i; j < actor->effectsCount - 1; j++) {
                    actor->effects[j] = actor->effects[j + 1];
                }
                actor->effectsCount--;
                i--;
            }
            printf("%s is under the %s effect: ", actor->name, actor->effects[i].name);
            if (strcmp("HP", actor->effects[i].affectTo) == 0) {
                actor->HP += actor->effects[i].value;
                if (actor->HP > actor->maxHP) actor->HP = actor->maxHP;
                if (actor->HP < 0) actor->HP = 0;
                if (actor->HP == 0) {
                    printf("%s is dead.\n", actor->name);
                } else {
                    printf("%s's HP is now %d.\n", actor->name, actor->HP);
                }
            } else if (strcmp("ATK", actor->effects[i].affectTo) == 0) {
                actor->ATK += actor->effects[i].value;
                printf("%s's ATK is now %d.\n", actor->name, actor->ATK);
            } else if (strcmp("DEF", actor->effects[i].affectTo) == 0) {
                actor->DEF += actor->effects[i].value;
                printf("%s's DEF is now %d.\n", actor->name, actor->DEF);
            } else if (strcmp("Energy", actor->effects[i].affectTo) == 0) {
                actor->energy += actor->effects[i].value;
                if (actor->energy > MAX_ENERGY) actor->energy = MAX_ENERGY;
                printf("%s's energy is now %d.\n", actor->name, actor->energy);
            }
            actor->effects[i].remaining--;
            printf("%s's %s effect will last for %d more turns.\n", actor->name, actor->effects[i].name, actor->effects[i].remaining);
        }
    }
    
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
    actor->isDodging = !(action >= 1 && action <= actor->skillsCount);
    if (action >= 1 && action <= actor->skillsCount) {
        if (strcmp("Ally", actor->skills[action-1].target) == 0)
        {
            int target = -1;
            if (team == 1) {
                do {
                    printf("Choose a target (1-%d): ", MAX_TEAM_MEMBERS_COUNT);
                    char input[50];
                    fgets(input, sizeof input, stdin);
                    input[strcspn(input, "\n")] = 0;
                    target = atoi(input)-1;
                    if (target < 0 || target >= MAX_TEAM_MEMBERS_COUNT) {
                        printf("Invalid target %d. Please choose a valid target.\n", target+1);
                        target = -1;
                    } else if (allies[target].HP <= 0) {
                        printf("Target %s is already dead. Please choose a valid target.\n", allies[target].name);
                        target = -1;
                    }
                    
                } while (target == -1 || allies[target].HP <= 0);
            } else {
                target = rand() % MAX_TEAM_MEMBERS_COUNT;
                while (allies[target].HP <= 0) {
                    target = rand() % MAX_TEAM_MEMBERS_COUNT;
                }
            }
            performAction(actor, allies, target, action-1);
        } else {
            int target = -1;
            if (team == 1) {
                do {
                    printf("Choose a target (1-%d): ", MAX_TEAM_MEMBERS_COUNT);
                    char input[50];
                    fgets(input, sizeof input, stdin);
                    input[strcspn(input, "\n")] = 0;
                    target = atoi(input)-1;
                    if (target < 0 || target >= MAX_TEAM_MEMBERS_COUNT) {
                        printf("Invalid target %d. Please choose a valid target.\n", target+1);
                        target = -1;
                    } else if (opponents[target].HP <= 0) {
                        printf("Target %s is already dead. Please choose a valid target.\n", opponents[target].name);
                        target = -1;
                    }
                } while (target == -1 || opponents[target].HP <= 0);
            } else {
                target = rand() % MAX_TEAM_MEMBERS_COUNT;
                while (opponents[target].HP <= 0) {
                    target = rand() % MAX_TEAM_MEMBERS_COUNT;
                }
            }
            performAction(actor, opponents, target, action-1);
        }
    } else if (actor->energy >= DODGE_COST) {
        actor->energy -= DODGE_COST;
        printf("%s prepares to dodge with a %d%% chance.\n", actor->name, actor->DODGE);
    } else {
        printf("%s doesn't have enough energy to dodge. Resting this turn\n", actor->name);
    }
}

// Handle a full round
void handleRound(Character *allChars, Team players, Team enemies, int totalChars, char *dialogueMessage) {
    printf("\n--- Round ---\n");

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

    printf("\n--- Status after Round ---\n");
    for (size_t i = 0; i < players.membersCount; i++) {
        printf("%s - Energy: %d, HP: %d/%d Dodge: %d\n", players.members[i].name, players.members[i].energy, players.members[i].HP, players.members[i].maxHP, players.members[i].isDodging);
    }
    for (size_t i = 0; i < enemies.membersCount; i++) {
        printf("%s - Energy: %d, HP: %d/%d Dodge: %d\n", enemies.members[i].name, enemies.members[i].energy, enemies.members[i].HP, enemies.members[i].maxHP, players.members[i].isDodging);
    }
}