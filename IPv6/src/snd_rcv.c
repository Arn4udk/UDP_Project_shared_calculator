#include "../include/snd_rcv.h"

// tr = 1 -> ajout de l'IP et de l'heure, concaténés à la chaîne envoyée
int envoi_string(char *adresse, char *port, char *data, int tr) {

  int sockfd;
  socklen_t addrlen;
  struct sockaddr_in6 dest;

  // socket factory
  if ((sockfd = socket(AF_INET6, SOCK_DGRAM, 0)) == -1) {
    perror("Sender error (socket)");
    exit(EXIT_FAILURE);
  }

  // init remote addr structure and other params
  dest.sin6_family = AF_INET6;
  dest.sin6_port = htons(atoi(port));
  addrlen = sizeof(struct sockaddr_in6);

  // get addr from command line and convert it
  if (inet_pton(AF_INET6, adresse, &dest.sin6_addr) != 1) {
    perror("Sender error (inet_pton)");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  if (tr) {
    strcat(data, " at ");
    strcat(data, renvoi_heure());
  }

  // send string
  if (sendto(sockfd, data, strlen(data) + 1, 0, (void *)&dest, addrlen) == -1) {
    perror("Sender error (sendto)");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  // close the socket
  close(sockfd);
  return 0;
}

char *reception_string(char *port) {
  int sockfd;
  static char buf[1024];
  socklen_t addrlen;

  struct sockaddr_in6 my_addr;
  struct sockaddr_in6 client;

  // socket factory
  if ((sockfd = socket(AF_INET6, SOCK_DGRAM, 0)) == -1) {
    perror("Receiver error (socket)");
    exit(EXIT_FAILURE);
  }

  // init local addr structure and other params
  my_addr.sin6_family = AF_INET6;
  my_addr.sin6_port = htons(atoi(port));
  my_addr.sin6_addr = in6addr_any;
  addrlen = sizeof(struct sockaddr_in6);
  memset(buf, '\0', 1024);

  // bind addr structure with socket
  bind(sockfd, (void *)&my_addr, addrlen);

  // reception de la chaine de caracteres
  if (recvfrom(sockfd, buf, 1024, 0, (void *)&client, &addrlen) == -1) {
    perror("Receiver error (recvfrom)");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  // close the socket
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

int traitement(char *motif) {
  int vala = motif[2] - '0';
  int valb = motif[4] - '0';
  char operande = motif[0];

  switch (operande) {

  case '+':
    return vala + valb;
    break;
  case '-':
    return vala - valb;
    break;
  case '*':
    return vala * valb;
    break;
  case '/':
    return vala / valb;
    break;
  }
  return 0;
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

// Envoie d'un string toutes les DELAI_VIE secondes
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

    strcpy(recep, reception_string(PORT_VIE));

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
