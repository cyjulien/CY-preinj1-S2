/**
 * @file character.h
 * @brief Contains character-related functions and definitions.
 */
#include <stdlib.h>
#include <string.h>
#include "effect.h"
#include "skill.h"

/**
 * @struct Character
 * @brief Represents a character with all of its data.
 */
typedef struct {
    char *name;               /**< Character's name. */
    int HP;                   /**< Current health points. */
    int maxHP;                /**< Maximum health points. */
    int ATK;                  /**< Attack. */
    int DEF;                  /**< Defense. */
    int DODGE;                /**< Dodge chance (as a percentage). */
    int SPD;                  /**< Speed value. */
    int energy;               /**< Energy for actions. */
    Skill *skills;            /**< Array of the character's skills. */
    Effect *effects;          /**< Array of the character's status effects. */
} Character;

/**
 * @brief Creates a new character.
 * 
 * Creates a character with default values
 * 
 * @return Character structure with default values.
 */
Character newEmptyCharacter() {
  Character character;
  character.name = "Default";
  character.HP = 0;
  character.maxHP = 0;
  character.ATK = 0;
  character.DEF = 0;
  character.DODGE = 0;
  character.SPD = 0;
  character.energy = 0;
  character.skills = NULL;
  character.effects = NULL;
  return character;
}

/**
 * @brief Prints a character's data in the console.
 * 
 * Debug function to print a character's data.
 * 
 * @param character The character to be printed.
 */
void logCharacter(Character character) {
  printf("------------------------\nName: %s\nHP: %d\nmax HP: %d\nATK: %d\nDEF: %d\nDODGE: %d\nSPD: %d\nEnergy: %d\n------------------------\n", character.name, character.HP, character.maxHP, character.ATK, character.DEF, character.DODGE, character.SPD, character.energy);
}

/**
 * @brief Loads a character from a file based on their name.
 * 
 * Searches for a file in "Logic/data/characters/"
 * and attempts to parse character data from it.
 * 
 * @param name The name of the character (must be the same as filename: name.txt).
 * @return Character structure with either the data from the file or NullCharacter is the file is missing (NullCharacter's maxHP will be the error code).
 */
Character getCharacter(char *name) {
  Character character = newEmptyCharacter();
  character.name = "null";
  FILE *data = NULL;
  char path[50];
  snprintf(path, sizeof(path), "Logic/data/characters/%s.txt", name);
  data = fopen(path, "r+");
  if (data == NULL) {
    printf("Error: %s\n", strerror(errno));
    character.maxHP = errno;
    if (errno == 2) {
      printf("A character file was asked for but the character does not exist in the database. Make sure file names are correct, the maximun length for a name is 22.\n");
    }
    return character;
  }
  char dataLine[80];
  int endOfFile = 0;
  while (!endOfFile) {
    if (fgetc(data) == EOF) {
      endOfFile = 1;
    }
    fseek(data, -1, 1);
    fgets(dataLine, 79, data);
    int colonPos = strcspn(dataLine, ":");
    char *field = malloc(colonPos * sizeof(char));
    strncpy(field, dataLine, colonPos);
    *(field+colonPos) = '\0';
    if (strcmp("Skills", field) == 0)
    {
      break;
    }
    int valueLen = strlen(dataLine)-colonPos-2;
    char *value = malloc(valueLen * sizeof(char));
    memcpy(value, strpbrk(dataLine, " ")+1, valueLen);
    *(value+valueLen-1) = '\0';
    //printf("------------------------\ncolonPos:%d dataLen: %d, char %d\n------------------------\n", colonPos, strlen(dataLine), sizeof(char));
    //printf("%s: %s\n", field, value);
    if (strcmp("Name", field) == 0) {
      memcpy(character.name, value, strlen(value));
    }
    if (strcmp("HP", field) == 0) {
      character.HP = atoi(value);
    }
    if (strcmp("maxHP", field) == 0) {
      character.maxHP = atoi(value);
    }
    if (strcmp("ATK", field) == 0) {
      character.ATK = atoi(value);
    }
    if (strcmp("DEF", field) == 0) {
      character.DEF = atoi(value);
    }
    if (strcmp("DODGE", field) == 0) {
      character.DODGE = atoi(value);
    }
    if (strcmp("SPD", field) == 0) {
      character.SPD = atoi(value);
    }
    if (strcmp("Energy", field) == 0) {
      character.energy = atoi(value);
    }
    free(field);
    free(value);
    field = NULL;
    value = NULL;
  }

  fclose(data);
  return character;
}
