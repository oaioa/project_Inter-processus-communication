#include "useful.h"
int descpipe[2];

void signal_handlerpere(int nofils)
{
  printf("kill effectué\n");
  kill(nofils,SIGINT);
  sleep(15000);
  exit(0);
}

void signal_handlerfils()
{
  close(descpipe[1]);
  printf("Programme Tirage arrete\n");
  exit(0);
}

int main()
{
  VOL new;
  int r=0;
  int nofils=fork();

  if(pipe(descpipe)==-1)
  {
    printf("erreur création pipe\n");exit(1);
  }
  printf("création du pipe réussie\n");

  if(nofils==-1)
  {
    printf("erreur création du fils\n");
    exit(-1);
  }

  if(nofils==0) //on est dans le fils (tirage)
  {
    printf("je suis le fils\n");
    signal(SIGINT,signal_handlerfils);
    close(descpipe[0]); // tirage ne s'en sert pas en lecture


    while(1)
    {
      r=rand()%2;
      printf("destinations: %s\n",destinations_possible[r]);
      new.destination[r]=destinations_possible[r];
      new.number=rand()%50; //hasard du nombre de places  
      write(descpipe[1],&new,sizeof(new));
      printf("écriture faite %s\n",new);
      sleep(10);

    }
 
  }
  
     else //je suis le père (écrivain)
     {
     close(descpipe[1]);
     VOL dest[20];
     VOL place;

     signal(SIGINT,signal_handlerpere);
     close(descpipe[1]); // écrivain ne s'en sert pas en écriture
     int id=create_shmem(memK,80);
     if (id==-1)
     {
     printf("erreur création shared memory ecrivain id:%d \n",id);
     exit(1);
     }
     printf("shared memory créée id:%d\n",id);
     attach_shmem(id);
     printf("attachement shared memory\n");

     int id2=create_semaphore(sem_placeK); //création du sémapjore de places libres
     int id3=create_semaphore(mutexK); //création du sémapjore mutex
     int mutex=init_semaphore(id3,1);
     int place_dispo=init_semaphore(id2,20);
     printf("mutex= %d\n",mutex);
     printf("sem_place= %d\n",place_dispo);
     while(1)
     {
   //  while(read(descpipe[0],&dest[0],sizeof(dest[0])))
     //  printf("dest:%s\n",dest[0]);
     while(read(descpipe[0],&place,sizeof(place)))
       printf("%d",place);

     down(place_dispo);
     down(mutex);
  // mise dans la mémoire partagé

  up(mutex);

  }
close(descpipe[0]);
  }




}
