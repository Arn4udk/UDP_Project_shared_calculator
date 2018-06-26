#include "../include/snd_rcv.h"

void boucle_reception(int *check) {
  while (1) {
    char buf2[1024];
    fflush(stdout);
    strcpy(buf2, reception_string(PORT_BASE));
    printf("\n%s\norchestrateur> ", buf2);
    fflush(stdout);

    switch (buf2[9]) {

    case '+':
      check[0] = 0;
      break;
    case '-':
      check[1] = 0;
      break;
    case '*':
      check[2] = 0;
      break;
    case '/':
      check[3] = 0;
      break;
    }
  }
}

void boucle_emission(time_t *tab, int *check) {
  while (1) {
    char string[1024];                    // le string utilisateur
    memset(string, '\0', sizeof(string)); // reset du tabl
    fprintf(stdout, "\norchestrateur> "); // affichage de la requete par l'orche
    fflush(stdout);
    scanf("%s", string);
    fflush(stdout);

    char buf1[1024]; // buffer d'envoi à ne pas vider, contient la requête
                     // initiale
    strcpy(buf1, renvoi_heure());
    strcat(buf1, string);
    switch (string[0]) {
    case '+':
      if (tab[0] == 0) {
        printf("Le noeud + est down, requete impossible\n");
      } else if (check[0] == 1) {
        printf("Le noeud + est en cours de calcul, requete impossible\n");
      } else {
        check[0] = 1;
        envoi_string(ADRESSE_LOCALE, PORT_ADDITION, buf1, 0);
        affiche_heure();
      }
      break;

    case '-':
      if (tab[1] == 0) {
        printf("Le noeud - est down, requete impossible\n");
      } else if (check[1] == 1) {
        printf("Le noeud - est en cours de calcul, requete impossible\n");
      } else {
        check[1] = 1;
        envoi_string(ADRESSE_LOCALE, PORT_SOUSTRACTION, buf1, 0);
        affiche_heure();
      }
      break;

    case '*':
      if (tab[2] == 0) {
        printf("Le noeud * est down, requete impossible\n");
      } else if (check[2] == 1) {
        printf("Le noeud * est en cours de calcul, requete impossible\n");
      } else {
        check[2] = 1;
        envoi_string(ADRESSE_LOCALE, PORT_MULTIPLICATION, buf1, 0);
        affiche_heure();
      }
      break;

    case '/':
      if (tab[3] == 0) {
        printf("Le noeud / est down, requete impossible\n");
      } else if (check[3] == 1) {
        printf("Le noeud / est en cours de calcul, requete impossible\n");
      } else {
        check[3] = 1;
        envoi_string(ADRESSE_LOCALE, PORT_DIVISION, buf1, 0);
        affiche_heure(); // demande effectuée à ...
      }
      break;

    default:
      printf("\n Seuls les opérateurs / * + - sont supportés ! :(\n");
      break;
    }
  }
}

int main(void) {

  // signal(SIGINT, inthand); // le signal handler
  int i = 0;
  time_t timing[4];
  time(&timing[0]);
  for (i = 1; i < 4; i++) {
    timing[i] = timing[0];
  }

  int check[4];
  for (i = 0; i < 4; i++) {
    check[i] = 0;
  }

#pragma omp parallel
#pragma omp sections
  {
#pragma omp section
    { boucle_emission(timing, check); }
#pragma omp section
    { boucle_reception(check); }
#pragma omp section
    { spam(); }
#pragma omp section
    { en_vie_check(timing); }
  }

  return 0;
}
