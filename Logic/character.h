#include "effect.h"
#include "skill.h"
typedef struct {
    int HP;
    int maxHP;
    int ATK;
    int DEF;
    int DODGE;
    int SPD;
    Skill *skills;
    Effect *effects;
} Character;
