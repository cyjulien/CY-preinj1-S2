#include "main.h"

int main(int argc, char const *argv[]) {
  srand(time(NULL)); // Initialize random seed

  Team player, enemy;

  // Initialize the player and enemy champions
  //initializeChampion(&player.champions[0], "Player Champion");
  //initializeChampion(&enemy.champions[0], "Enemy Champion");
  /*Character chara = getCharacter("Johnson");
  logCharacter(chara);*/
  player.champions[0] = getCharacter("Example");
  enemy.champions[0] = getCharacter("Johnson");
  /*logCharacter(getCharacter("Example"));
  logCharacter(player.champions[0]);*/

  int round = 1;

  while (player.champions[0].HP > 0 && enemy.champions[0].HP > 0) {
      handleRound(&player, &enemy, round);
      round++;
  }

  // Check who won
  if (player.champions[0].HP > 0) {
      printf("You win!\n");
  } else if (enemy.champions[0].HP > 0) {
      printf("You lose! Enemy wins.\n");
  } else {
      printf("It's a draw!\n");
  }
  free(player.champions[0].name);
  free(enemy.champions[0].name);
  printf("Execution ended.\n");
  return 0;
}
