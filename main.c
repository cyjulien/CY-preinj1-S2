#include "main.h"

int main(int argc, char const *argv[]) {
    printf("Execution started.\n");
    srand(time(NULL));

    /*Team player = getTeam("Example");
    Team enemy = getTeam("Team2");
    logCharacter(player.members[0]);
    for (size_t i = 0; i < MAX_SKILLS; i++)
    {
        logSkill(player.members[0].skills[i]);
    }
    int round = 1;
    while (player.members[0].HP > 0 && enemy.members[0].HP > 0) {
        handleRound(&player, &enemy, round);
        round++;
    }
    // Check who won
    if (player.members[0].HP > 0) {
        printf("You win!\n");
    } else if (enemy.members[0].HP > 0) {
        printf("You lose! Enemy wins.\n");
    } else {
        printf("It's a draw!\n");
    }*/
    Team players = getTeam("Example");
    Team enemies = getTeam("Team2");
    Character allChars[MAX_TEAM_MEMBERS_COUNT * 2];

    for (int i = 0; i < players.membersCount; i++) {
        allChars[i] = players.members[i];
    }
    for (int i = 0; i < enemies.membersCount; i++) {
        allChars[i + players.membersCount] = enemies.members[i];
    }

    int round = 1;
    while (round < 50) {
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
