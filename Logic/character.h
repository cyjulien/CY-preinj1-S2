/**
 * @file character.h
 * @brief Contains character-related functions and definitions.
 */
#include <stdlib.h>
#include <string.h>
#include "effect.h"
#include "skill.h"
#include "config.h"

/**
 * @struct Character
 * @brief Represents a character with all of its data.
 */
typedef struct {
    char *name;               /**< Character's name. */
    int lvl;                  /**< The Character's level. */
    int maxHP;                /**< Maximum health points. */
    int HP;                   /**< Current health points. */
    int ATK;                  /**< Attack. */
    int DEF;                  /**< Defense. */
    int DODGE;                /**< Dodge chance (as a percentage). */
    int SPD;                  /**< Speed value. */
    int isDodging;            /**< Whether or not the character is currently dodging. */
    int energy;               /**< Energy for actions. */
    Skill *skills;            /**< Array of the character's skills. */
    Effect *effects;          /**< Array of the character's status effects. */
} Character;

/**
 * @brief Creates a new Character.
 *
 * Creates a Character with default values
 *
 * @return Character structure with default values.
 */
Character newEmptyCharacter() {
  Character character;
  character.name = "Null";
  character.lvl = 0;
  character.maxHP = 0;
  character.HP = 0;
  character.ATK = 0;
  character.DEF = 0;
  character.DODGE = 0;
  character.isDodging = 0;
  character.SPD = 0;
  character.energy = 0;
  character.skills = malloc(MAX_SKILLS * sizeof(Skill));
  for (size_t i = 0; i < MAX_SKILLS; i++)
  {
    character.skills[i] = newEmptySkill();
  }
  character.effects = malloc(MAX_EFFECTS * sizeof(Effect));
  for (size_t i = 0; i < MAX_EFFECTS; i++)
  {
    character.effects[i] = newEmptyEffect();
  }
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
  printf("------------------------\nName: %s\nLevel: %d\nHP: %d\nmax HP: %d\nATK: %d\nDEF: %d\nDODGE: %d\nSPD: %d\nEnergy: %d\n------------------------\n", character.name, character.lvl, character.HP, character.maxHP, character.ATK, character.DEF, character.DODGE, character.SPD, character.energy);
}

/**
 * @brief Loads a character from a file based on their name.
 *
 * Searches for a file in "Logic/data/characters/"
 * and attempts to parse character data from it.
 *
 * @param name The name of the character (must be the same as filename: name.txt).
 * @return Character structure with either the data from the file or nullCharacter is the file is missing (nullCharacter.maxHP will be the error code).
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
          break;
        }
        fseek(data, -1, 1);
        fgets(dataLine, 79, data);
        int colonPos = strcspn(dataLine, ":");
        char *field = malloc((colonPos+1) * sizeof(char));
        if (!field) {
          printf("Memory allocation failed\n");
          exit(303);
        }
        strncpy(field, dataLine, colonPos);
        *(field+colonPos) = '\0';
        if (strcmp("Skills", field) == 0)
        {
          Skill skill = getSkill("Example");
          for (size_t i = 0; i < MAX_SKILLS; i++)
          {
            if (strcmp("Null", character.skills[i].name) == 0)
            {
              character.skills[i] = skill;
            } else if (i >= MAX_SKILLS)
            {
              printf("Error trying to register more than %d skills to one character, this can be changed in the config.", MAX_SKILLS);
              break;
            }
          }
          
          break;
        }
        int valueLen = strlen(dataLine)-colonPos-2;
        char *value = malloc((valueLen+1) * sizeof(char));
        if (!value) {
          printf("Memory allocation failed\n");
          exit(303);
        }
        memcpy(value, strpbrk(dataLine, " ")+1, valueLen);
        *(value+valueLen-1) = '\0';
        //printf("------------------------\ncolonPos:%d dataLen: %d, char %d\n------------------------\n", colonPos, strlen(dataLine), sizeof(char));
        //printf("%s: %s\n", field, value);
        if (strcmp("Name", field) == 0) {
          character.name = malloc((strlen(value)+1) * sizeof(char));
          memcpy(character.name, value, (strlen(value)+1));
        }
        if (strcmp("Lv", field) == 0) {
          character.lvl = atoi(value);
        }
        if (strcmp("maxHP", field) == 0) {
          character.maxHP = atoi(value);
          character.HP = character.maxHP;
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
