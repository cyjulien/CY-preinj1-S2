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

int main() {
    afficherInterface();
    return 0;
}