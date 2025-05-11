#include <stdio.h>

void afficherInterface() {
    printf("  [EQUIPE 1]---------------------------------------------------------------\n");
    printf("  CHAMPION 1 |1|          CHAMPION 2 |4|          CHAMPION 3 |3|          \n");
    printf("  (###)                     [      ]                [##########]           \n");
    printf("  [>>>>]                    [>>>>>>]                [>>>>>>>>>]            \n");
    printf("                                                                          \n");
    printf("  [EQUIPE 2]---------------------------------------------------------------\n");
    printf("  ENNEMY 1 |0|            ENNEMY 2 |2|              >ENNEMY 3<            \n");
    printf("  (!!!)(00)                 [####]                   [##      ]            \n");
    printf("  [>>>>]                    [>>>>>>]                 [>>>>>>  ]            \n");
    printf("                                                                          \n");
    printf("  ENNEMY 3\n");
    printf("  TECHNIQUES SPECIALES\n");
    printf("  [1] TECH 1 (durée: 3 tours)\n");
    printf("      o à décrire\n");
    printf("  [-] TECH 2 (recharge: 2 / durée: 4 tours)\n");
    printf("      o à décrire\n");
    printf("  [2] TECH 3 (durée: 3 tours)\n");
    printf("      o à décrire\n");
}

void displayInterface(Team topTeam, Team bottomTeam, const char *dialogueMessage) {
    // Display the top team
    printf("  === [TOP TEAM] ===\n");
    for (int i = 0; i < topTeam.membersCount; i++) {
        printf("  Name: %s | HP: %d | Energy: %d\n",
               topTeam.members[i].name,
               topTeam.members[i].HP,
               topTeam.members[i].energy);
    }

    // Display dialogue box
    printf("\n  === [DIALOGUE BOX] ===\n");
    if (dialogueMessage) {
        printf("  %s\n", dialogueMessage);
    } else {
        printf("  No actions yet...\n");
    }

    // Display the bottom team
    printf("\n  === [BOTTOM TEAM] ===\n");
    for (int i = 0; i < bottomTeam.membersCount; i++) {
        printf("  Name: %s | HP: %d | Energy: %d\n",
               bottomTeam.members[i].name,
               bottomTeam.members[i].HP,
               bottomTeam.members[i].energy);
    }
}

void displayFullInterface(Team topTeam, Team bottomTeam, const char *dialogueMessage) {
    // ─── TOP TEAM HEADER ────────────────────────────────────────────────
    printf(COLOR_YELLOW "  [%s]---------------------------------------------------------------\n" COLOR_RESET, topTeam.name);

    // Character names with index
    for (int i = 0; i < topTeam.membersCount; i++) {
        printf("  %-12s|%d|   ", topTeam.members[i].name, i+1);
    }
    printf("\n");

    // HP bars + current HP value
    for (int i = 0; i < topTeam.membersCount; i++) {
        int hpPercent = (topTeam.members[i].HP * 10) / topTeam.members[i].maxHP;
        const char *color = (topTeam.members[i].HP > 0) ? COLOR_GREEN : COLOR_RED;
        printf("  %s[%-10s]%s %3d   ", color, "##########" + (10 - hpPercent), COLOR_RESET, topTeam.members[i].HP);
    }
    printf("\n");

    // Energy bars + current energy
    for (int i = 0; i < topTeam.membersCount; i++) {
        int energyPercent = (topTeam.members[i].energy * 10) / MAX_ENERGY;
        printf("  " COLOR_CYAN "[%-10s]" COLOR_RESET " %3d   ", ">>>>>>>>>>" + (10 - energyPercent), topTeam.members[i].energy);
    }
    printf("\n\n");

    // ─── BOTTOM TEAM HEADER ─────────────────────────────────────────────
    printf(COLOR_YELLOW "  [%s]---------------------------------------------------------------\n" COLOR_RESET, bottomTeam.name);

    // Character names with index
    for (int i = 0; i < bottomTeam.membersCount; i++) {
        printf("  %-12s|%d|   ", bottomTeam.members[i].name, i+1);
    }
    printf("\n");

    // HP bars + current HP
    for (int i = 0; i < bottomTeam.membersCount; i++) {
        int hpPercent = (bottomTeam.members[i].HP * 10) / bottomTeam.members[i].maxHP;
        const char *color = (bottomTeam.members[i].HP > 0) ? COLOR_GREEN : COLOR_RED;
        printf("  %s[%-10s]%s %3d   ", color, "##########" + (10 - hpPercent), COLOR_RESET, bottomTeam.members[i].HP);
    }
    printf("\n");

    // Energy bars + current energy
    for (int i = 0; i < bottomTeam.membersCount; i++) {
        int energyPercent = (bottomTeam.members[i].energy * 10) / MAX_ENERGY;
        printf("  " COLOR_CYAN "[%-10s]" COLOR_RESET " %3d   ", ">>>>>>>>>>" + (10 - energyPercent), bottomTeam.members[i].energy);
    }
    printf("\n\n");

    // ─── DIALOGUE BOX ───────────────────────────────────────────────────
    printf(COLOR_YELLOW "  [DIALOGUE BOX]----------------------------------------------------------\n" COLOR_RESET);
    if (dialogueMessage && strlen(dialogueMessage) > 0) {
        printf("  %s\n", dialogueMessage);
    } else {
        printf("  No actions yet...\n");
    }
}