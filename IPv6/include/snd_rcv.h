#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include <omp.h>


#ifndef SND_RCV_H
#define SND_RCV_H
#define PORT_BASE "8000"
#define PORT_ADDITION "8001"
#define PORT_DIVISION "8002"
#define PORT_MULTIPLICATION "8003"
#define PORT_SOUSTRACTION "8004"
#define DELAI_VIE 10
#define ADRESSE_LOCALE "::FFFF:127.0.0.1"
#define PORT_VIE "8010"
#define DELAI_CALCUL 10
#define DELAI_MORT 10


int envoi_string(char* adresse,char* port,char* data,int tr);
char* reception_string(char* port);
int traitement(char* motif);
void affiche_heure();
void delai_aleat();
char* renvoi_heure();
int get_nombre(char *string, int offset);
int parser(char *calcul);
void en_vie(char* c);
void en_vie_check();
int renvoi_seconde();
void en_vie_check(time_t* tab);
void spam();
struct tm * renvoi_date();

#endif
