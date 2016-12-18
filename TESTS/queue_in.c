#include "useful.h"

void signal_handler(int signal){
  printf("Kill received %d !\n"); 
  exit(0);
}


int main(){
  signal(SIGINT,signal_handler);

  VOL v1;
  v1.number = 4;
  strcpy(v1.destination,"Toulouse");

  int msgflg = IPC_CREAT | 0666;  

  int idMes = msgget(balK,msgflg);//creation bal avec droits 660    
  printf("Message with ID : %d\n",idMes);

  msgsnd(idMes,(void *) & v1,20,0); //message envoyé

  return 0;

  }


