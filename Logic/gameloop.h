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
            //printf("%s regenerates %d energy.\n", characters[i].name, regen);
        } else {
            characters[i].energy = 0;
        }
    }
}

void applyValueToHp(Character *performer, Character *targets, Skill skill, int value, char *dialogueMessage) { // Helper function to avoid code duplication
    if (skill.value < 0) {
        int damage = (performer->ATK * (1 - targets[value].DEF/(targets[value].DEF+1000.0))) * ((-skill.value) / 100.0);
        if (damage < 0) damage = 0;
        targets[value].HP -= damage;
        if (targets[value].HP < 0) targets[value].HP = 0;
        if (targets[value].HP > targets[value].maxHP) targets[value].HP = targets[value].maxHP;
        printf("  %s uses ability %s on %s, it did %d damage. %s's HP is now %d.", performer->name, skill.name, targets[value].name, damage, targets[value].name, targets[value].HP);
        //Log the printf to the dialogue box
        snprintf(dialogueMessage + strlen(dialogueMessage), 750 - strlen(dialogueMessage), "%s uses ability %s on %s, it did %d damage. %s's HP is now %d.", performer->name, skill.name, targets[value].name, damage, targets[value].name, targets[value].HP);
    } else {
        int heal = performer->maxHP * (skill.value / 100.0);
        if (heal < 0) heal = 0;
        targets[value].HP += heal;
        if (targets[value].HP < 0) targets[value].HP = 0;
        if (targets[value].HP > targets[value].maxHP) targets[value].HP = targets[value].maxHP;
        printf("  %s uses ability %s on %s, it healed %d HP. %s's HP is now %d.", performer->name, skill.name, targets[value].name, heal, targets[value].name, targets[value].HP);
        //Log the printf to the dialogue box
        snprintf(dialogueMessage + strlen(dialogueMessage), 750 - strlen(dialogueMessage), "%s uses ability %s on %s, it healed %d HP. %s's HP is now %d.", performer->name, skill.name, targets[value].name, heal, targets[value].name, targets[value].HP);
    }
    if (strcmp("Null", skill.applyEffect) != 0) {
        Effect effect = getEffect(skill.applyEffect);
        int effectAlreadyApplied = 0;
        for (size_t i = 0; i < targets[value].effectsCount; i++) {
            if (strcmp(targets[value].effects[i].name, effect.name) == 0) {
                effectAlreadyApplied = 1;
                targets[value].effects[i].remaining = effect.duration;
                targets[value].effects[i].value = effect.value;
                printf(" %s is under the %s effect for %d turns.", targets[value].name, effect.name, effect.duration);
                //Log the printf to the dialogue box
                snprintf(dialogueMessage + strlen(dialogueMessage), 750 - strlen(dialogueMessage), " %s is under the %s effect for %d turns.", targets[value].name, effect.name, effect.duration);
            }
        }
        if (!effectAlreadyApplied) {
            targets[value].effects[targets[value].effectsCount] = effect;
            targets[value].effectsCount++;
            printf(" %s is now under the %s effect for %d turns.", targets[value].name, effect.name, effect.duration);
            //Log the printf to the dialogue box
            snprintf(dialogueMessage + strlen(dialogueMessage), 750 - strlen(dialogueMessage), " %s is now under the %s effect for %d turns.", targets[value].name, effect.name, effect.duration);
        }
    }
    printf("\n");
    snprintf(dialogueMessage + strlen(dialogueMessage), 750 - strlen(dialogueMessage), "\n  ");
}
void applyValueToEnergy(Character *performer, Character *targets, Skill skill, int value, char *dialogueMessage) { // Helper function to avoid code duplication
    targets[value].energy += skill.value;
    if (targets[value].energy < 0) targets[value].energy = 0;
    if (targets[value].energy > MAX_ENERGY) targets[value].energy = MAX_ENERGY;
    printf("  %s %s %s, %s's energy is now %d.", performer->name, skill.value<0?"removes energy from":"regenerates energy to", targets[value].name, targets[value].name, targets[value].energy);
    //Log the printf to the dialogue box
    snprintf(dialogueMessage + strlen(dialogueMessage), 750 - strlen(dialogueMessage), "%s %s %s, %s's energy is now %d.", performer->name, skill.value<0?"removes energy from":"regenerates energy to", targets[value].name, targets[value].name, targets[value].energy);
    if (strcmp("Null", skill.applyEffect) != 0) {
        Effect effect = getEffect(skill.applyEffect);
        int effectAlreadyApplied = 0;
        for (size_t i = 0; i < targets[value].effectsCount; i++) {
            if (strcmp(targets[value].effects[i].name, effect.name) == 0) {
                effectAlreadyApplied = 1;
                targets[value].effects[i].remaining = effect.duration;
                targets[value].effects[i].value = effect.value;
                printf(" %s is under the %s effect for %d turns.", targets[value].name, effect.name, effect.duration);
                //Log the printf to the dialogue box
                snprintf(dialogueMessage + strlen(dialogueMessage), 750 - strlen(dialogueMessage), " %s is under the %s effect for %d turns.", targets[value].name, effect.name, effect.duration);
            }
        }
        if (!effectAlreadyApplied) {
            targets[value].effects[targets[value].effectsCount] = effect;
            targets[value].effectsCount++;
            printf(" %s is now under the %s effect for %d turns.", targets[value].name, effect.name, effect.duration);
            //Log the printf to the dialogue box
            snprintf(dialogueMessage + strlen(dialogueMessage), 750 - strlen(dialogueMessage), " %s is now under the %s effect for %d turns.", targets[value].name, effect.name, effect.duration);
        }
    }
    printf("\n");
    snprintf(dialogueMessage + strlen(dialogueMessage), 750 - strlen(dialogueMessage), "\n  ");
}
// Perform action
int performAction(Character *performer, Character *targets, int target, int action, char *dialogueMessage) {
    Skill skill = performer->skills[action];

    if (performer->energy >= skill.cost) {
        performer->energy -= skill.cost;
        //If the target is dodging and the performer is an enemy he gets a chance of avoiding the attack
        //printf("%s dodging: %d\n", targets[target].name, targets[target].isDodging);
        if (strcmp("Enemy", skill.target) == 0 && targets[target].isDodging && (rand() % 100) < targets[target].DODGE) {
            printf("  %s dodged the attack from %s!\n", targets[target].name, performer->name);
            //Log the printf to the dialogue box
            snprintf(dialogueMessage + strlen(dialogueMessage), 750 - strlen(dialogueMessage), "%s dodged the attack from %s!\n  ", targets[target].name, performer->name);
        } else if (strcmp("HP", skill.affectTo) == 0 && skill.value != 0) {
            if (strcmp("Enemy", skill.target) == 0 && targets[target].isDodging) printf("  %s attempted to dodge but failed.\n", targets[target].name);
            //Log the printf to the dialogue box
            if (strcmp("Enemy", skill.target) == 0 && targets[target].isDodging) snprintf(dialogueMessage + strlen(dialogueMessage), 750 - strlen(dialogueMessage), "%s attempted to dodge but failed.\n  ", targets[target].name);
            //Deal damage to the targeted enemy
            if (skill.nTarget >= 1) {
                applyValueToHp(performer, targets, skill, target, dialogueMessage);
            }
            //For multiple targets, if odd then deal damage to the left and right, if even then deal damage to the left or right randomly
            if (skill.nTarget > 1){
                if (skill.nTarget%2 == 0){
                    for (size_t i = 1; i <= ((skill.nTarget - 2)/2); i++) {
                        int right = target+i;
                        if (right < MAX_TEAM_MEMBERS_COUNT) applyValueToHp(performer, targets, skill, right, dialogueMessage);
                        int left = target-i;
                        if (left >= 0) applyValueToHp(performer, targets, skill, left, dialogueMessage);
                    }
                    if (target <= 0) {
                        //Target with no one to the left
                        applyValueToHp(performer, targets, skill, target+(skill.nTarget/2), dialogueMessage);
                    } else if (target >= MAX_TEAM_MEMBERS_COUNT-1) {
                        //Target with no one to the right
                        applyValueToHp(performer, targets, skill, target-(skill.nTarget/2), dialogueMessage);
                    } else {
                        //Target with other targets on both sides
                        applyValueToHp(performer, targets, skill, target + (rand()%2==0?-1*(skill.nTarget/2):1*(skill.nTarget/2)), dialogueMessage);
                    }
                } else {
                    for (size_t i = 1; i <= ((skill.nTarget - 1)/2); i++) {
                        int right = target+i;
                        if (right < MAX_TEAM_MEMBERS_COUNT) applyValueToHp(performer, targets, skill, right, dialogueMessage);
                        int left = target-i;
                        if (left >= 0) applyValueToHp(performer, targets, skill, left, dialogueMessage);
                    }
                }
            }
        } else if (strcmp("Energy", skill.affectTo) == 0 && skill.value != 0) {
            if (strcmp("Enemy", skill.target) == 0 && targets[target].isDodging) printf("  %s attempted to dodge but failed.\n", targets[target].name);
            //Log the printf to the dialogue box
            if (strcmp("Enemy", skill.target) == 0 && targets[target].isDodging) snprintf(dialogueMessage + strlen(dialogueMessage), 750 - strlen(dialogueMessage), "%s attempted to dodge but failed.\n  ", targets[target].name);
            if (skill.nTarget >= 1) {
                applyValueToEnergy(performer, targets, skill, target, dialogueMessage);
            }
            //For multiple targets, if odd then deal damage to the left and right, if even then deal damage to the left or right randomly
            if (skill.nTarget > 1){
                if (skill.nTarget%2 == 0){
                    for (size_t i = 1; i < skill.nTarget - 2; i++) {
                        applyValueToEnergy(performer, targets, skill, target+i, dialogueMessage);
                    }
                    for (size_t i = 1; i < skill.nTarget - 2; i++) {
                        applyValueToEnergy(performer, targets, skill, target-i, dialogueMessage);
                    }
                    if (target <= 0) {
                        //Target with no one to the left
                        applyValueToEnergy(performer, targets, skill, target+(skill.nTarget/2), dialogueMessage);
                    } else if (target >= MAX_TEAM_MEMBERS_COUNT-1) {
                        //Target with no one to the right
                        applyValueToEnergy(performer, targets, skill, target-(skill.nTarget/2), dialogueMessage);
                    } else {
                        //Target with other targets on both sides
                        applyValueToEnergy(performer, targets, skill, target + (rand()%2==0?-1*(skill.nTarget/2):1*(skill.nTarget/2)), dialogueMessage);
                    }
                } else {
                    for (size_t i = 1; i <= ((skill.nTarget - 1)/2); i++) {
                        int right = target+i;
                        if (right < MAX_TEAM_MEMBERS_COUNT) applyValueToEnergy(performer, targets, skill, right, dialogueMessage);
                        int left = target-i;
                        if (left >= 0) applyValueToEnergy(performer, targets, skill, left, dialogueMessage);
                    }
                }
            }
        }
        return 1;
    } else {
        printf("  %s does not have enough energy to perform %s!\n", performer->name, skill.name);
        //Log the printf to the dialogue box
        snprintf(dialogueMessage + strlen(dialogueMessage), 750 - strlen(dialogueMessage), "%s does not have enough energy to perform %s!\n  ", performer->name, skill.name);
        return 0;
    }
}

// Sort characters by energy in descending order
int compareByEnergy(const void *a, const void *b) {
    Character *c1 = (Character *)a;
    Character *c2 = (Character *)b;
    return c2->energy - c1->energy;
}

// Handle actions for a character
int takeTurn(Character *actor, Character *allies, Character *opponents, int team, char *dialogueMessage) {
    if (actor->HP <= 0) return 0; // Skip if dead
    //Count down effects
    for (size_t i = 0; i < actor->effectsCount; i++) {
        if (actor->effects[i].remaining > 0) {
            printf("  %s is under the %s effect: ", actor->name, actor->effects[i].name);
            //Log the printf to the dialogue box
            snprintf(dialogueMessage + strlen(dialogueMessage), 750 - strlen(dialogueMessage), "%s is under the %s effect: ", actor->name, actor->effects[i].name);
            if (strcmp("HP", actor->effects[i].affectTo) == 0) {
                actor->HP += actor->effects[i].value; //Effects do a flat amount of HP
                if (actor->HP > actor->maxHP) actor->HP = actor->maxHP;
                if (actor->HP < 0) actor->HP = 0;
                if (actor->HP == 0) {
                    printf("%s is dead.\n", actor->name);
                    //Log the printf to the dialogue box
                    snprintf(dialogueMessage + strlen(dialogueMessage), 750 - strlen(dialogueMessage), "%s is dead.\n  ", actor->name);
                } else {
                    printf("%s's HP is now %d.\n", actor->name, actor->HP);
                    //Log the printf to the dialogue box
                    snprintf(dialogueMessage + strlen(dialogueMessage), 750 - strlen(dialogueMessage), "%s's HP is now %d.\n  ", actor->name, actor->HP);
                }
            } else if (strcmp("Energy", actor->effects[i].affectTo) == 0) {
                actor->energy += actor->effects[i].value; //Effects do a flat amount of energy
                if (actor->energy > MAX_ENERGY) actor->energy = MAX_ENERGY;
                printf("%s's energy is now %d.\n", actor->name, actor->energy);
                //Log the printf to the dialogue box
                snprintf(dialogueMessage + strlen(dialogueMessage), 750 - strlen(dialogueMessage), "%s's energy is now %d.\n  ", actor->name, actor->energy);
            }
            actor->effects[i].remaining--;
            printf("  %s's %s effect will last for %d more turns.\n", actor->name, actor->effects[i].name, actor->effects[i].remaining);
            //Log the printf to the dialogue box
            snprintf(dialogueMessage + strlen(dialogueMessage), 750 - strlen(dialogueMessage), "%s's %s effect will last for %d more turns.\n  ", actor->name, actor->effects[i].name, actor->effects[i].remaining);
        } else if (actor->effects[i].remaining == 0) {
            //check if effect name is Null
            if (strcmp("Null", actor->effects[i].name) != 0) {
                //Remove the effect
                printf("  %s's %s effect has ended.\n", actor->name, actor->effects[i].name);
                //Log the printf to the dialogue box
                snprintf(dialogueMessage + strlen(dialogueMessage), 750 - strlen(dialogueMessage), "%s's %s effect has ended.\n  ", actor->name, actor->effects[i].name);
                // Remove the effect
                for (size_t j = i; j < actor->effectsCount - 1; j++) {
                    actor->effects[j] = actor->effects[j + 1];
                }
                actor->effectsCount--;
                i--;
            }
        }
    }
    
    // Start the action, if it's the player's turn show the controls
    // If it's the enemy's turn, choose a random action
    int action = 0;
    if (team == 1) { // Player
        printf(COLOR_YELLOW "  [PLAYER CONTROLS]------------------------------------------------------\n" COLOR_RESET);
        printf("  %s's turn! (HP: %d, Energy: %d)\n", actor->name, actor->HP, actor->energy);
        printf("  0. Dodge\n");
        for (size_t i = 0; i < actor->skillsCount; i++)
        {
            printf("  %ld. %s, cost: %d.\n   %s\n", (i+1), actor->skills[i].name, actor->skills[i].cost, actor->skills[i].desc);
        }
        
        printf("  Your choice: ");
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
    actor->isDodging = !(action >= 1 && action <= actor->skillsCount); // If the action is dodge, set the isDodging flag to true
    if (action >= 1 && action <= actor->skillsCount) { // If the action is a skill
        // Check which target the skill is for
        if (strcmp("Ally", actor->skills[action-1].target) == 0)
        {
            int target = -1;
            //Check if all targets are dead
            int allDead = 0;
            for (size_t i = 0; i < MAX_TEAM_MEMBERS_COUNT; i++)
            {
                if (allies[i].HP <= 0) {
                    allDead++;
                }
            }
            if (allDead >= MAX_TEAM_MEMBERS_COUNT) {
                printf("  All allies are dead. You can't use this ability.\n");
                return 0;
            }
            if (team == 1) {
                do {
                    printf("  Choose a target \033[36m(1-%d)\033[0m: ", MAX_TEAM_MEMBERS_COUNT);
                    char input[50];
                    fgets(input, sizeof input, stdin);
                    input[strcspn(input, "\n")] = 0;
                    target = atoi(input)-1;
                    if (target < 0 || target >= MAX_TEAM_MEMBERS_COUNT) {
                        printf("  Invalid target %d. Please choose a valid target.\n", target+1);
                        target = -1;
                    } else if (allies[target].HP <= 0) {
                        printf("  Target %s is already dead. Please choose a valid target.\n", allies[target].name);
                        target = -1;
                    }
                    
                } while (target == -1 || allies[target].HP <= 0);
            } else {
                target = rand() % MAX_TEAM_MEMBERS_COUNT;
                while (allies[target].HP <= 0) {
                    target = rand() % MAX_TEAM_MEMBERS_COUNT;
                }
            }
            return performAction(actor, allies, target, action-1, dialogueMessage);
        } else if (strcmp("Self", actor->skills[action-1].target) == 0){
            //look for the actor in the allies array
            int target = -1;
            for (size_t i = 0; i < MAX_TEAM_MEMBERS_COUNT; i++)
            {
                if (strcmp(actor->name, allies[i].name) == 0) {
                    target = i;
                    break;
                }
            }
            return performAction(actor, allies, target, action-1, dialogueMessage);
        } else {
            int target = -1;
            //Check if all targets are dead
            int allDead = 0;
            for (size_t i = 0; i < MAX_TEAM_MEMBERS_COUNT; i++)
            {
                if (opponents[i].HP <= 0) {
                    allDead++;
                }
            }
            if (allDead >= MAX_TEAM_MEMBERS_COUNT) {
                printf("  All opponents are dead. You can't use this ability.\n");
                return 0;
            }
            if (team == 1) {
                do {
                    printf("  Choose a target \033[35m(1-%d)\033[0m: ", MAX_TEAM_MEMBERS_COUNT);
                    char input[50];
                    fgets(input, sizeof input, stdin);
                    input[strcspn(input, "\n")] = 0;
                    target = atoi(input)-1;
                    if (target < 0 || target >= MAX_TEAM_MEMBERS_COUNT) {
                        printf("  Invalid target %d. Please choose a valid target.\n", target+1);
                        target = -1;
                    } else if (opponents[target].HP <= 0) {
                        printf("  Target %s is already dead. Please choose a valid target.\n", opponents[target].name);
                        target = -1;
                    }
                } while (target == -1 || opponents[target].HP <= 0);
            } else {
                target = rand() % MAX_TEAM_MEMBERS_COUNT;
                while (opponents[target].HP <= 0) {
                    target = rand() % MAX_TEAM_MEMBERS_COUNT;
                }
            }
            return performAction(actor, opponents, target, action-1, dialogueMessage);
        }
    } else if (actor->energy >= DODGE_COST) {
        actor->energy -= DODGE_COST;
        printf("  %s prepares to dodge with a %d%% chance.\n", actor->name, actor->DODGE);
        //Log the printf to the dialogue box
        snprintf(dialogueMessage + strlen(dialogueMessage), 750 - strlen(dialogueMessage), "%s prepares to dodge with a %d%% chance.\n  ", actor->name, actor->DODGE);
        return 1;
    } else {
        printf("  %s doesn't have enough energy to dodge. Resting this turn\n", actor->name);
        //Log the printf to the dialogue box
        snprintf(dialogueMessage + strlen(dialogueMessage), 750 - strlen(dialogueMessage), "%s doesn't have enough energy to dodge. Resting this turn\n  ", actor->name);
        return 0;
    }
}

// Handle a full round
void handleTurn(Character *allChars, Team players, Team enemies, int totalChars, char *dialogueMessage) {
    //Reset the dialogue message
    snprintf(dialogueMessage, 750, "Information. \n  ");
    // Check in which team the character is
    // Check if action happened to avoid infinite loop to regenerate energy
    int actionHappened = 0;
    for (size_t i = 0; i < players.membersCount; i++) {
        if (strcmp(players.members[i].name, allChars[0].name) == 0) {
            actionHappened = takeTurn(&players.members[i], players.members, enemies.members, 1, dialogueMessage);
        }
    }
    for (size_t i = 0; i < enemies.membersCount; i++) {
        if (strcmp(enemies.members[i].name, allChars[0].name) == 0) {
            actionHappened = takeTurn(&enemies.members[i], enemies.members, players.members, 0, dialogueMessage);
        }
    }

    // Regenerate energy for all characters after the turn if an action happened
    if (actionHappened) {
        regenerateEnergy(players.members, players.membersCount);
        regenerateEnergy(enemies.members, enemies.membersCount);
    }

    // Debugging log character status
    /*printf("\n--- Status after Turn ---\n");
    for (size_t i = 0; i < players.membersCount; i++) {
        printf("%s - Energy: %d, HP: %d/%d Dodge: %d\n", players.members[i].name, players.members[i].energy, players.members[i].HP, players.members[i].maxHP, players.members[i].isDodging);
    }
    for (size_t i = 0; i < enemies.membersCount; i++) {
        printf("%s - Energy: %d, HP: %d/%d Dodge: %d\n", enemies.members[i].name, enemies.members[i].energy, enemies.members[i].HP, enemies.members[i].maxHP, players.members[i].isDodging);
    }*/
}

void sortCharactersByEnergy(Character *allChars, int totalChars) {
    qsort(allChars, totalChars, sizeof(Character), compareByEnergy);
    // Debugging log action order
    /*for (size_t i = 0; i < 6; i++)
    {
        printf("%d. %s\n", i, allChars[i].name);
    }
    printf("-------------\n");*/
}
