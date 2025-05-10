#include <stdio.h>
#include "Logic/team.h" // Inclure les définitions de Team et Character

void interface0(Team equipeHaut, Team equipeBas) {
    // Affichage de l'équipe en haut de l'écran
    printf("=== Équipe en haut ===\n");
    for (int i = 0; i < equipeHaut.membersCount; i++) {
        printf("Nom: %s | HP: %d | Énergie: %d\n",
               equipeHaut.members[i].name,
               equipeHaut.members[i].HP,
               equipeHaut.members[i].energy);
    }

    // Laisser de l'espace pour la boîte de dialogue
    printf("\n=== Boîte de dialogue ===\n");
    printf("Exemple : Blabla attaque blabla et fait 400 dégâts\n\n");

    // Affichage de l'équipe en bas de l'écran
    printf("=== Équipe en bas ===\n");
    for (int i = 0; i < equipeBas.membersCount; i++) {
        printf("Nom: %s | HP: %d | Énergie: %d\n",
               equipeBas.members[i].name,
               equipeBas.members[i].HP,
               equipeBas.members[i].energy);
    }
}

int main() {
    // Exemple : Chargement des équipes depuis des fichiers
    Team equipeHaut = getTeam("equipeHaut"); // Remplacer par le nom de fichier réel
    Team equipeBas = getTeam("equipeBas");   // Remplacer par le nom de fichier réel

    // Appel de la fonction interface0
    interface0(equipeHaut, equipeBas);

    return 0;
}