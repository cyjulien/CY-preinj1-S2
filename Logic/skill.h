/**
 * @file skill.h
 * @brief Contains skill-related functions and definitions.
 */
typedef struct {
    char *name;
    char *desc;
    int cost;
    float value;
    char *affectTo;
    char *target;
    int nTarget;
    char *applyEffect;
} Skill;

/**
 * @brief Creates a new Skill.
 *
 * Creates a Skill with default values
 *
 * @return Skill structure with default values.
 */
Skill newEmptySkill() {
    Skill skill;
    skill.name = NULL;
    skill.desc = NULL;
    skill.cost = 0;
    skill.value = 0;
    skill.affectTo = NULL;
    skill.target = NULL;
    skill.nTarget = 0;
    skill.applyEffect = NULL;
    return skill;
}

/**
 * @brief Prints a Skill's data in the console.
 *
 * Debug function to print a Skill's data.
 *
 * @param Skill The Skill to be printed.
 */
void logSkill(Skill skill) {
    printf("------------------------\nName: %s\nDesc: %s\nCost: %d\nValue: %f\nAffectTo: %s\nTarget: %s\nNTarget: %d\nApplyEffect: %s\n------------------------\n", skill.name, skill.desc, skill.cost, skill.value, skill.affectTo, skill.target, skill.nTarget, skill.applyEffect);
}

/**
 * @brief Loads a Skill from a file based on their name.
 *
 * Searches for a file in "Logic/data/skills/"
 * and attempts to parse Skill data from it.
 *
 * @param name The name of the Skill (must be the same as filename: name.txt).
 * @return Skill structure with either the data from the file or nullSkill is the file is missing (nullSkill.value will be the error code).
 */
Skill getSkill(char *name) {
    Skill skill = newEmptySkill();
    skill.name = "Null";
    FILE *data = NULL;
    char path[50];
    snprintf(path, sizeof(path), "Logic/data/skills/%s.txt", name); //Using snprintf instead of strcat to avoid memory issues
    data = fopen(path, "r+");
    if (data == NULL) {
        printf("Error: %s\n", strerror(errno));
        skill.value = errno;
        if (errno == 2) {
            printf("%s was asked for but the Skill does not exist in the database. Make sure file names are correct, the maximum length for a name is 22.\n", name);
        }
        return skill;
    }
    char dataLine[150];
    int endOfFile = 0;
    while (!endOfFile) {
        if (fgetc(data) == EOF) { //Check for EOF
          endOfFile = 1;
          break;
        }
        fseek(data, -1, 1); //Go back because of fgets
        fgets(dataLine, 149, data); //Explore file line by line
        int colonPos = strcspn(dataLine, ":");
        char *field = malloc((colonPos+1) * sizeof(char));
        if (!field) {
            printf("Memory allocation failed 6060\n");
            exit(606);
        }
        strncpy(field, dataLine, colonPos);
        *(field+colonPos) = '\0';
        int valueLen = strlen(dataLine)-colonPos-2;
        char *value = malloc((valueLen+1) * sizeof(char));
        if (!value) {
            printf("Memory allocation failed 6061\n");
            exit(606);
        }
        memcpy(value, strpbrk(dataLine, " ")+1, valueLen);
        *(value+valueLen-1) = '\0';
        /**
         * Update the skill to the values read from the file:
         */
        if (strcmp("Name", field) == 0) {
            skill.name = malloc((strlen(value)+1) * sizeof(char));
            memcpy(skill.name, value, (strlen(value)+1));
        }
        if (strcmp("Desc", field) == 0) {
            skill.desc = malloc((strlen(value)+1) * sizeof(char));
            memcpy(skill.desc, value, (strlen(value)+1));
        }
        if (strcmp("Cost", field) == 0) {
            skill.cost = atoi(value);
        }
        if (strcmp("Value", field) == 0) {
            skill.value = atoi(value);
        }
        if (strcmp("AffectTo", field) == 0) {
            skill.affectTo = malloc((strlen(value)+1) * sizeof(char));
            memcpy(skill.affectTo, value, (strlen(value)+1));
        }
        if (strcmp("Target", field) == 0) {
            skill.target = malloc((strlen(value)+1) * sizeof(char));
            memcpy(skill.target, value, (strlen(value)+1));
        }
        if (strcmp("NTarget", field) == 0) {
            skill.nTarget = atoi(value);
        }
        if (strcmp("ApplyEffect", field) == 0) {
            skill.applyEffect = malloc((strlen(value)+1) * sizeof(char));
            memcpy(skill.applyEffect, value, (strlen(value)+1));
        }
        /*
        free(skill.name);
        free(skill.desc);
        free(skill.affectTo);
        */
        free(field);
        free(value);
        field = NULL;
        value = NULL;
    }

    fclose(data);
    return skill;
}
  