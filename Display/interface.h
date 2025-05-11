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
    printf("  [EQUIPE 1]---------------------------------------------------------------\n");

    // Top team names
    for (int i = 0; i < topTeam.membersCount; i++) {
        printf("  %s |%d|          ", topTeam.members[i].name, i);
    }
    printf("\n");

    // Top team HP bars (text-based visualization)
    for (int i = 0; i < topTeam.membersCount; i++) {
        int hpPercent = (topTeam.members[i].HP * 10) / topTeam.members[i].maxHP;
        printf("  [%*s]          ", 10, "##########" + (10 - hpPercent));
    }
    printf("\n");

    // Top team Energy bars
    for (int i = 0; i < topTeam.membersCount; i++) {
        int energyPercent = (topTeam.members[i].energy * 10) / MAX_ENERGY;
        printf("  [%-*s]          ", 10, ">>>>>>>>>>" + (10 - energyPercent));
    }
    printf("\n\n");

    printf("  [EQUIPE 2]---------------------------------------------------------------\n");

    // Bottom team names
    for (int i = 0; i < bottomTeam.membersCount; i++) {
        printf("  %s |%d|          ", bottomTeam.members[i].name, i);
    }
    printf("\n");

    // Bottom team HP bars
    for (int i = 0; i < bottomTeam.membersCount; i++) {
        int hpPercent = (bottomTeam.members[i].HP * 10) / bottomTeam.members[i].maxHP;
        printf("  [%*s]          ", 10, "##########" + (10 - hpPercent));
    }
    printf("\n");

    // Bottom team Energy bars
    for (int i = 0; i < bottomTeam.membersCount; i++) {
        int energyPercent = (bottomTeam.members[i].energy * 10) / MAX_ENERGY;
        printf("  [%-*s]          ", 10, ">>>>>>>>>>" + (10 - energyPercent));
    }
    printf("\n\n");

    // Dialogue box
    printf("  [DIALOGUE BOX]----------------------------------------------------------\n");
    if (dialogueMessage && strlen(dialogueMessage) > 0) {
        printf("  %s\n", dialogueMessage);
    } else {
        printf("  No actions yet...\n");
    }
}