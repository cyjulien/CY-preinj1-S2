#include "main.h"

int main(int argc, char const *argv[]) {
    printf("---------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("Execution started.\n");
    srand(time(NULL));

    Team players = getTeam("Example");
    Team enemies = getTeam("Team2");
    Character allChars[MAX_TEAM_MEMBERS_COUNT * 2];

    int round = 1;
    int playersAlive = 3;
    int enemiesAlive = 3;
    
    while (enemiesAlive > 0 && playersAlive > 0) {
        system("clear");
        system("cls");

        playersAlive = 0;
        for (int i = 0; i < players.membersCount; i++) {
            if (players.members[i].HP > 0) {
                playersAlive++;
            }
            allChars[i] = players.members[i];
        }
        enemiesAlive = 0;
        for (int i = 0; i < enemies.membersCount; i++) {
            if (enemies.members[i].HP > 0) {
                enemiesAlive++;
            }
            allChars[i + players.membersCount] = enemies.members[i];
        }
        char *dialogueMessage = NULL;
        displayFullInterface(players, enemies, dialogueMessage);
        handleRound(allChars, players, enemies, MAX_TEAM_MEMBERS_COUNT * 2, dialogueMessage);
    }
    if (playersAlive == 0) {
        printf("Enemies win!\n");
    } else if (enemiesAlive == 0) {
        printf("Players win!\n");
    } else {
        printf("Draw!\n");
    }
    
    for (int i = 0; i < players.membersCount; i++) {
        free(players.members[i].name);
        free(players.members[i].skills);
        free(players.members[i].effects);
    }
    for (int i = 0; i < enemies.membersCount; i++) {
        free(enemies.members[i].name);
        free(enemies.members[i].skills);
        free(enemies.members[i].effects);
    }

    printf("Execution ended.\n");
    return 0;
}
