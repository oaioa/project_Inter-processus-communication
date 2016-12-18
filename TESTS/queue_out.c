#include "useful.h"

void signal_handler(int signal){
  printf("Kill received %d !\n"); 
  // !! pour taile bal sizeof(struct MESSAGE)
  // il faut faire la somme de chaque composantes indépendamments
  //

  exit(0);
}


int main(){
  VOL v1;
  v1.number = 4;
  strcpy(v1.destination,"Toulouse");
  signal(SIGINT,signal_handler);

  int msgflg = IPC_CREAT | 0666;  
  int idMes = msgget(balK,msgflg);//creation bal avec droits 660    
  printf("Message with ID : %d\n",idMes);

  msgsnd(idMes,(void *) & v1,20,0); //message envoyé

  VOL * shm = (VOL*) shmat (smid, NULL, 0);
  while(1){
    int i;
    for(i = 0 ; i<20 ; i++){
      shm[i].number=i; 
      strcpy(shm[i].destination,"TLS");
    }
  }
  return 0;

  }


