#include "main.h"

int main(int argc, char const *argv[]) {
  srand(time(NULL));

  // Initialize teams
  Team team1, team2;

  initializeChampion(&team1.champions[0], "Champion1_Team1");
  initializeChampion(&team1.champions[1], "Champion2_Team1");
  initializeChampion(&team1.champions[2], "Champion3_Team1");

  initializeChampion(&team2.champions[0], "Champion1_Team2");
  initializeChampion(&team2.champions[1], "Champion2_Team2");
  initializeChampion(&team2.champions[2], "Champion3_Team2");

  // Simulate turns
  for (int turn = 1; turn <= 5; turn++) {
      handleTurn(&team1, &team2, turn);
  }
  
  printf("Execution ended.\n");
  return 0;
}
