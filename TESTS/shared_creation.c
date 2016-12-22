#include "../useful.h"

int smid; //ID de la mémoire partagée
VOL * vol_sm; //pointeur sur la mémoie partégée

void signal_handler(int signal){
  printf("Kill received %d !\n"); 
	shmdt(vol_sm); //détachement de la mémoire partagée 
  shmctl(smid, IPC_RMID, 0) ;//supression de la mémoire partagée
  exit(0);
}

int main(){
  signal(SIGINT,signal_handler);

  VOL vol;
  int msgflg = IPC_CREAT | 0666;  
  smid = shmget(memK,20*sizeof(VOL),msgflg);//creation shared memory
  vol_sm = (VOL*) shmat(smid, NULL, 0);
  printf("Attaché à mémoire partagé \n");
  int i;
  for(i = 0 ; i<20 ; i++){
    vol_sm[i].number=i;
    strcpy(vol_sm[i].destination,"TLS");
  sleep(5);
  }
  shmctl(smid,IPC_RMID,0);

  return 0;
}
