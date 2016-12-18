#include "useful.h"

void signal_handler(int signal){
  printf("Kill received %d !\n"); 
  // !! pour taile bal sizeof(struct MESSAGE)
  // il faut faire la somme de chaque composantes indépendamments
  //
  shmctl(memK, IPC_RMID, 0) ;
  exit(0);
}

int main(){
  VOL vol;
  signal(SIGINT,signal_handler);

  int msgflg = IPC_CREAT | 0666;  
  //  int idMes = msgget(balK,msgflg);//creation bal avec droits 660    
  //  printf("Message with ID : %d\n",idMes);


  int smid = shmget(memK,20*sizeof(VOL),msgflg);//creation shared memory
  VOL * shm;
  printf("Message with ID : %d\n",smid);

  shm = (VOL*) shmat(smid, NULL, 0);
  printf("Attaché à mémoire partagé \n");
  int i;
  for(i = 0 ; i<20 ; i++){
    shm[i].number=i;
    strcpy(shm[i].destination,"TLS");
  sleep(5);
  }
  shmctl(smid,IPC_RMID,0);

  return 0;
}
