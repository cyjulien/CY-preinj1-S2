/**
 * @file team.h
 * @brief Contains team-related functions and definitions.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @struct Team
 * @brief Represents a team with all of its data.
 */
typedef struct {
    char *name;               /**< The Team's name. */
    int membersCount;         /**< How many Characters are in the team. */
    Character *members;       /**< An array of all the team's members. */
} Team;

/**
 * @brief Creates a new team.
 *
 * Creates a team with default values
 *
 * @return Team structure with default values.
 */
Team newEmptyTeam() {
  Team team;
  team.name = "Null";
  team.membersCount = 0;
  team.members = malloc(MAX_TEAM_MEMBERS_COUNT * sizeof(Character));
  return team;
}

/**
 * @brief Prints a team's data in the console.
 *
 * Debug function to print a team's data.
 *
 * @param team The team to be printed.
 */
void logTeam(Team team) {
  printf("------------------------\nName: %s\n%d Members:\n", team.name, team.membersCount);
  int isEmpty = 1;
  for (size_t i = 0; i < MAX_TEAM_MEMBERS_COUNT; i++) {
    if (strcmp("Null", team.members[i].name) != 0) {
      isEmpty = 0;
      printf("%s\n", team.members[i].name);
    }
  }
  if (isEmpty) printf("The team is empty\n");
  printf("------------------------\n");
}

/**
 * @brief Loads a team from a file based on their name.
 *
 * Searches for a file in "Logic/data/teams/"
 * and attempts to parse team data from it.
 *
 * @param name The name of the team (must be the same as filename: name.txt).
 * @return Team structure with either the data from the file or nullTeam is the file is missing (nullTeam.membersCount will be the error code).
 */
Team getTeam(char *name) {
  Team team = newEmptyTeam();
  team.name = "Null";
  FILE *data = NULL;
  char path[50];
  snprintf(path, sizeof(path), "Logic/data/teams/%s.txt", name);
  data = fopen(path, "r+");
  if (data == NULL) {
    printf("Error: %s\n", strerror(errno));
    team.membersCount = errno;
    if (errno == 2) {
      printf("A team file was asked for but the team does not exist in the database. Make sure file names are correct, the maximun length for a name is 22.\n");
    }
    return team;
  }
  char dataLine[80];
  int endOfFile = 0;
  while (!endOfFile) {
    if (fgetc(data) == EOF) {
      endOfFile = 1;
      break;
    }
    fseek(data, -1, 1);
    fgets(dataLine, 79, data);
    int colonPos = strcspn(dataLine, ":");
    char *field = malloc(colonPos * sizeof(char));
    strncpy(field, dataLine, colonPos);
    *(field+colonPos) = '\0';
    if (strcmp("Members", field) == 0) {
      int i = 0;
      while (!endOfFile) {
        if (fgetc(data) == EOF) {
          endOfFile = 1;
          break;
        }
        fseek(data, -1, 1);
        fgets(dataLine, 79, data);
        *(dataLine+strcspn(dataLine, "\r\n")) = '\0';
        int valueLen = strlen(dataLine)-1;
        char *value = malloc((valueLen+1) * sizeof(char));
        memcpy(value, strpbrk(dataLine, " ")+2, (valueLen+1));
        *(value+valueLen) = '\0';
        team.members[i] = getCharacter(value);
        free(value);
        value = NULL;
        i++;
      }
      team.membersCount = i;
      if (i < MAX_TEAM_MEMBERS_COUNT)
      {
        for (size_t j = i; j < MAX_TEAM_MEMBERS_COUNT; j++)
        {
          team.members[i] = newEmptyCharacter();
        }
        
      }
      
      free(field);
      field = NULL;
      break;
    }
    int valueLen = strlen(dataLine)-colonPos-2;
    char *value = malloc((valueLen+1) * sizeof(char));
    memcpy(value, strpbrk(dataLine, " ")+1, (valueLen+1));
    *(value+valueLen-1) = '\0';
    if (strcmp("Name", field) == 0) {
      team.name = malloc((strlen(value)+1) * sizeof(char));
      memcpy(team.name, value, strlen(value)+1);
    }
    free(field);
    free(value);
    field = NULL;
    value = NULL;
  }

  fclose(data);
  return team;
}