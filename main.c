#include "main.h"

int main(int argc, char const *argv[]) {
    printf("Execution started.\n");
    srand(time(NULL));

    Team players = getTeam("Example");
    Team enemies = getTeam("Team2");
    Character allChars[MAX_TEAM_MEMBERS_COUNT * 2];

    int round = 1;
    while (round < 50) {
        for (int i = 0; i < players.membersCount; i++) {
            allChars[i] = players.members[i];
        }
        for (int i = 0; i < enemies.membersCount; i++) {
            allChars[i + players.membersCount] = enemies.members[i];
        }
        handleRound(allChars, players, enemies, MAX_TEAM_MEMBERS_COUNT * 2, round++);
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
