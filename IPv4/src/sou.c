#include "../include/snd_rcv.h"

void soustraction() {
  while (1) {
    char chaine[5000];
    memset(chaine, '\0', sizeof(chaine));
    strcat(chaine, reception_string(PORT_SOUSTRACTION));

    // Découpage de la chaîne
    // Longueur de la chaîne reçue sans la date
    int lg = strlen(chaine) - 9;
    char *subbuff = (char *)malloc(lg * sizeof(char));
    memcpy(subbuff, &chaine[9], lg);
    subbuff[lg - 1] = '\0';
    // Le parser analyse la chaîne extraite, calcul pur.
    int ras = parser(subbuff);
    free(subbuff);
    // Stockage du résultat dans un char array
    char bif[1024];
    snprintf(bif, lg, "%d", ras);
    strcat(chaine, " = ");
    strcat(chaine, bif);
    delai_aleat();
    envoi_string(ADRESSE_LOCALE, PORT_BASE, chaine, 1);
  }
}

int main() {

#pragma omp parallel
#pragma omp sections
  {
#pragma omp section
    { en_vie("-OK"); }
#pragma omp section
    { soustraction(); }
  }

  return 0;
}
