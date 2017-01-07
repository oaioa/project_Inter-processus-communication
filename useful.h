#include <stdio.h> //print ...
#include <stdlib.h> //NULL pointer ...
#include <signal.h> //signal ...
#include <unistd.h> //pause ...
#include <time.h> //random ...
#include "semaphore.h"
#include "shmem.h"//shared memory
#include <sys/msg.h>
#include <sys/types.h>// types : key_t
#include <sys/ipc.h> //IPC permissions
#include <string.h> //strcopy ...

//noms de fonctions stylisées


typedef struct vol{
  int number;
  int PID;
  char destination[20];
}VOL;

//tableau destinations
char* destinations_possible[]={
"Paris","Londres","Barcelone" , "Pekin" , "Lyon", "New York", "Los Angeles" ,
"Varsovie" , "Budapest" , "Bucarest" , "Madrid" , "Brest" , "Stockholm" , "Berlin"
, "Buenos Aires" , "Lille" , "Rennes" , "Bruxelles" , "Toulouse" , "Bordeaux" ,
"Nantes" , "Pau" , "Bangkok" , "Sofia" , "Belgrade" , "Moscou" , "Rome" ,
"Vienne" , "Venise" , "Florence" , "Strasbourg" , "Marseille" , "Nice" ,
"Grenoble" , "Montpellier" , "Toulon" , "Anger" , "Reims" , "Le Havre" ,
"Le Caire","."
};

//CLÉS
key_t balK = 16;
key_t memK = 32;
key_t mutexK = 48;
key_t sem_placeK = 64;

//régler le problème de buffer pour scanf
void purge_buffer ()
{
  char c;
  while((c = getchar()) != '\n' && c != EOF);
}

//couleurs
// Agence verte : "\033[1;32mAgence\033[0m"
// Utilisateur yellow "\033[1;33mUtilisateur\033[0m"
// Écrivain rouge "\033[1;31mÉcrivain\033[0m"
// Affichage violet "\033[1;35mAffichage\033[0m"
// Tirage bleu "\033[1;34mTirage\033[0m"
// Lastminute magenta  "\033[1;36mLastminute\033[0m"
