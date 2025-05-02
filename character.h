#include <stdlib.h>
#include <string.h>
#include "effect.h"
#include "skill.h"
typedef struct {
    char *name;
    int HP;
    int maxHP;
    int ATK;
    int DEF;
    int DODGE;
    int SPD;
    int Energy;
    Skill *skills;
    Effect *effects;
} Character;

Character nullChar;
nullChar.name = "null";
nullChar.maxHp = 0;

Character getCharacter(char *name) {
  FILE *data = NULL;
  data = fopen(strcat(name, ".txt"), "r+");
  if (data == NULL) {
    printf("Error: %s\n", strerror(errno));
    nullChar.maxHp = errno;
    return nullChar;
  }
  char dataLine[80];
  int endOfFile = 0;
  while (dataLine != "Test" && !endOfFile) {
    if (fgetc(data) == EOF) {
      endOfFile = 1;
    }
    fseek(data, -1, 1);
    fgets(dataLine, 79, data);
  }

  fclose(data);
}
