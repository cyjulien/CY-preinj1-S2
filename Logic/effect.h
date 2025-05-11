/**
 * @file effect.h
 * @brief Contains effect-related functions and definitions.
 */
typedef struct {
    char *name;
    char *desc;
    int remaining;
    int value;
    char *affectTo;
    char *target;
    int nTarget;
    int duration;
} Effect;

/**
 * @brief Creates a new Effect.
 *
 * Creates a Effect with default values
 *
 * @return Effect structure with default values.
 */
Effect newEmptyEffect() {
    Effect effect;
    effect.name = NULL;
    effect.desc = NULL;
    effect.remaining = 0;
    effect.value = 0;
    effect.affectTo = NULL;
    effect.target = NULL;
    effect.nTarget = 0;
    effect.duration = 0;
    return effect;
}

/**
 * @brief Prints an Effect's data in the console.
 *
 * Debug function to print an Effect's data.
 *
 * @param Effect The Effect to be printed.
 */
void logEffect(Effect effect) {
    printf("------------------------\nName: %s\nDesc: %s\nRemaining: %d\nValue: %d\nAffectTo: %s\nTarget: %s\nNTarget: %d\nDuration: %d\n------------------------\n", effect.name, effect.desc, effect.remaining, effect.value, effect.affectTo, effect.target, effect.nTarget, effect.duration);
}

/**
 * @brief Loads a Effect from a file based on their name.
 *
 * Searches for a file in "Logic/data/effects/"
 * and attempts to parse Effect data from it.
 *
 * @param name The name of the Effect (must be the same as filename: name.txt).
 * @return Effect structure with either the data from the file or nullEffect is the file is missing (nullEffect.value will be the error code).
 */
Effect getEffect(char *name) {
    Effect effect = newEmptyEffect();
    effect.name = "Null";
    FILE *data = NULL;
    char path[50];
    snprintf(path, sizeof(path), "Logic/data/effects/%s.txt", name); //Using snprintf instead of strcat to avoid memory issues
    data = fopen(path, "r+");
    if (data == NULL) {
        printf("Error: %s\n", strerror(errno));
        effect.value = errno;
        if (errno == 2) {
            printf("%s was asked for but the Effect does not exist in the database. Make sure file names are correct, the maximum length for a name is 22.\n", name);
        }
        return effect;
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
            printf("Memory allocation failed 707\n");
            exit(707);
        }
        strncpy(field, dataLine, colonPos);
        *(field+colonPos) = '\0';
        int valueLen = strlen(dataLine)-colonPos-2;
        char *value = malloc((valueLen+1) * sizeof(char));
        if (!value) {
            printf("Memory allocation failed 707\n");
            exit(707);
        }
        memcpy(value, strpbrk(dataLine, " ")+1, valueLen);
        *(value+valueLen-1) = '\0';
        /**
         * Update the effect to the values read from the file:
         */
        if (strcmp("Name", field) == 0) {
            effect.name = malloc((strlen(value)+1) * sizeof(char));
            memcpy(effect.name, value, (strlen(value)+1));
        }
        if (strcmp("Desc", field) == 0) {
            effect.desc = malloc((strlen(value)+1) * sizeof(char));
            memcpy(effect.desc, value, (strlen(value)+1));
        }
        if (strcmp("Value", field) == 0) {
            effect.value = atoi(value);
        }
        if (strcmp("AffectTo", field) == 0) {
            effect.affectTo = malloc((strlen(value)+1) * sizeof(char));
            memcpy(effect.affectTo, value, (strlen(value)+1));
        }
        if (strcmp("Target", field) == 0) {
            effect.target = malloc((strlen(value)+1) * sizeof(char));
            memcpy(effect.target, value, (strlen(value)+1));
        }
        if (strcmp("NTarget", field) == 0) {
            effect.nTarget = atoi(value);
        }
        if (strcmp("Duration", field) == 0) {
            effect.duration = atoi(value);
            effect.remaining = effect.duration;
        }
        /*
        free(effect.name);
        free(effect.desc);
        free(effect.affectTo);
        */
        free(field);
        free(value);
        field = NULL;
        value = NULL;
    }

    fclose(data);
    return effect;
}