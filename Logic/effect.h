/**
 * @file skill.h
 * @brief Contains skill-related functions and definitions.
 */
typedef struct {
    char *name;
    char *desc;
    int remaining;
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
    effect.name = "Null";
    effect.desc = "This effect does nothing";
    effect.remaining = 0;
    effect.duration = 0;
    return effect;
}