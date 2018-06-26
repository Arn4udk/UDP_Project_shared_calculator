#include "../include/snd_rcv.h"

// tr = 1 -> ajout de l'IP et de l'heure, concaténés à la chaîne envoyée
int envoi_string(char *adresse, char *port, char *data, int tr) {
  int sockfd;        // file descriptor du socket pour l'identifier. //IPV4
  socklen_t addrlen; // taille de l'adresse. //4
  struct sockaddr_in receiver_addr; // adresse du receveur. //4

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) ==
      -1) { // AF_INET = IPV4, AF_INET6 = IPV6
    perror("socket");
    exit(EXIT_FAILURE);
  }

  receiver_addr.sin_family = AF_INET; // format d'adresse du receveur (ici
                                      // IPV4).
  receiver_addr.sin_port = htons(atoi(port)); // le port du récépteur est pris
                                              // en argument de la main. il doit
                                              // être du bon type.

  if (inet_pton(AF_INET, adresse, &receiver_addr.sin_addr) != 1) {
    perror("inet_pton");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  // Taille de la structure de format d'adresse, dépend probablement de IPV4 ou
  // IPV6
  addrlen = sizeof(struct sockaddr_in);

  /*Segment d'ajout de l'adresse IP dans le résultat */

  int n;

  struct ifreq ifr;

  char array[] = "eth0";

  n = socket(AF_INET, SOCK_DGRAM, 0);

  // Type of address to retrieve - IPv4 IP address

  ifr.ifr_addr.sa_family = AF_INET;

  // Copy the interface name in the ifreq structure

  strncpy(ifr.ifr_name, array, IFNAMSIZ - 1);

  ioctl(n, SIOCGIFADDR, &ifr);

  close(n);

  // display result
  // Ajout de <adresse> at heure
  if (tr) {
    strcat(data, " by <");
    strcat(data, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
    strcat(data, "> at ");
    strcat(data, renvoi_heure());
  }

  if (sendto(sockfd, data, strlen(data), 0, (struct sockaddr *)&receiver_addr,
             addrlen) == -1) {
    perror("sendto");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  if (close(sockfd) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  return 0;
}

char *reception_string(char *port) {
  int sockfd;            // fd du socket pour l'identifier
  static char buf[1024]; // buffer de récéption de la requête
  socklen_t addrlen;     // taille de l'adresse (IPv4).

  struct sockaddr_in receiver_addr; // structure du socket de réception.
  struct sockaddr_in sender_addr;   // structure du socket d'envoi.

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("creation du socket");
    exit(EXIT_FAILURE);
  }

  // Socket IPv4
  receiver_addr.sin_family = AF_INET;
  // On écoute sur le port en argument
  receiver_addr.sin_port = htons(atoi(port));
  // Le socket est lié à toutes les interfaces locales
  receiver_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  // Taille de la structure de format d'adresse
  addrlen = sizeof(struct sockaddr_in);

  // On initialise le buffer de reception
  memset(buf, '\0', 1024);

  // lose the pesky "Address already in use" error message
  /*if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) ==
  -1) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }*/
  // On assigne le format d'adresse receiver_addr au socket
  if (bind(sockfd, (struct sockaddr *)&receiver_addr, addrlen) == -1) {
    // perror("Bind");
    // close(sockfd);
    // exit(EXIT_FAILURE); Comme ça plus d'erreur ;)
  }

  // Reception du socket
  if (recvfrom(sockfd, buf, 1024, 0, (struct sockaddr *)&sender_addr,
               &addrlen) == -1) {
    perror("recvfrom");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  close(sockfd);

  return buf;
}

void affiche_heure() {
  time_t rawtime;
  time(&rawtime);
  struct tm *tm_struct = localtime(&rawtime);
  int hour = tm_struct->tm_hour;
  int min = tm_struct->tm_min;
  int sec = tm_struct->tm_sec;
  printf("\r# demande effectuée à %02d:%02d:%02d\n", hour, min, sec);
}

char *renvoi_heure() {
  time_t rawtime;
  time(&rawtime);
  static char buf[8];
  struct tm *tm_struct = localtime(&rawtime);
  int hour = tm_struct->tm_hour;
  int min = tm_struct->tm_min;
  int sec = tm_struct->tm_sec;
  sprintf(buf, "%02d:%02d:%02d ", hour, min, sec);
  return buf;
}

int renvoi_seconde() {
  time_t rawtime;
  time(&rawtime);
  struct tm *tm_struct = localtime(&rawtime);
  return tm_struct->tm_sec;
}

struct tm *renvoi_date() {
  time_t rawtime;
  time(&rawtime);
  struct tm *tm_struct = localtime(&rawtime);
  return tm_struct;
}

void delai_aleat() {
  time_t t;
  srand((unsigned)time(&t));
  int r = rand() % DELAI_CALCUL; // entre 0 et 19s de lag
  sleep(r);
}

int get_nombre(char *string, int offset) {
  int i;     // boucle générale de parcours pour le nombre
  int j = 0; // pour le temporaire
  char temp[100];

  for (i = offset; string[i] != '\0'; i++) {
    temp[j] = string[i];
    j++;
  }

  return atoi(temp);
}

// Le parser de qualité
int parser(char *calcul) {
  int a;       // premier chiffre/nombre
  int b;       // deuxieme chiffre/nombre
  char c;      // opérande
  char *token; // string transitoire de découpe
  const char s[2] = ",";

  /* On récup le premier token (opérande + 1er nb) */

  token = strtok(calcul, s);

  c = token[0]; // On fixe l'opérande.

  a = get_nombre(token, 2);

  /* On récupère le tokens suivant */

  token = strtok(NULL, s);

  b = get_nombre(token, 0);

  switch (c) {

  case '+':
    return a + b;
    break;
  case '-':
    return a - b;
    break;
  case '*':
    return a * b;
    break;
  case '/':
    return a / b;
    break;
  }

  return -1;
}

// Envoi d'un string toutes les DELAI_VIE secondes
void en_vie(char *c) {
  while (1) {
    sleep(DELAI_VIE);
    envoi_string(ADRESSE_LOCALE, PORT_VIE, c, 0);
  }
}

void spam() {
  while (1) {
    sleep(DELAI_VIE / 2);
    envoi_string(ADRESSE_LOCALE, PORT_VIE, "spam", 0);
  }
}

void en_vie_check(time_t *tab) {
  while (1) {
    char recep[4];
    double difft = DELAI_MORT;
    int i;
    time_t time_actuel;
    time(&time_actuel);
    for (i = 0; i < 4; i++) // on analyse après chaque appel de en_vie_check
    {
      if (difftime(time_actuel, tab[i]) > difft) {
        tab[i] = 0;
      }
    }

    strcpy(recep,
           reception_string(PORT_VIE)); // le spam permet de casser ce barrage

    switch (recep[0]) {
    case '+':
      time(&tab[0]);
      break;
    case '-':
      time(&tab[1]);
      break;
    case '*':
      time(&tab[2]);
      break;
    case '/':
      time(&tab[3]);
      break;
    default:
      break;
    }
  }
}
