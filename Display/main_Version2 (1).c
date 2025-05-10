#include <stdio.h>
#include "Logic/team.h" // Inclure les définitions pour Team et Character

void displayInterface(Team topTeam, Team bottomTeam, const char *dialogueMessage) {
    // Display the top team
    printf("  === [TOP TEAM] ===\n");
    for (int i = 0; i < topTeam.membersCount; i++) {
        printf("  Name: %s | HP: %d | Energy: %d\n",
               topTeam.members[i].name,
               topTeam.members[i].HP,
               topTeam.members[i].energy);
    }

    // Display dialogue box
    printf("\n  === [DIALOGUE BOX] ===\n");
    if (dialogueMessage) {
        printf("  %s\n", dialogueMessage);
    } else {
        printf("  No actions yet...\n");
    }

    // Display the bottom team
    printf("\n  === [BOTTOM TEAM] ===\n");
    for (int i = 0; i < bottomTeam.membersCount; i++) {
        printf("  Name: %s | HP: %d | Energy: %d\n",
               bottomTeam.members[i].name,
               bottomTeam.members[i].HP,
               bottomTeam.members[i].energy);
    }
}

int main() {
    // Example teams for testing
    Team topTeam = getTeam("topTeam");    // Replace with actual team data
    Team bottomTeam = getTeam("bottomTeam"); // Replace with actual team data

    // Example dialogue message
    const char *dialogueMessage = "Example: Player1 attacks Enemy1 and deals 400 damage.";

    // Call the displayInterface function
    displayInterface(topTeam, bottomTeam, dialogueMessage);

    return 0;
}
