#include "useful.h"
int bal_id;
int sm_id;
VOL * sm_ptr;

void signal_handler(int signal){
	if( signal == SIGINT){
		printf("Agence || SIGINT !\n"); 
		msgctl(bal_id,IPC_RMID,NULL);
		printf("Agence || Supression bal \n");
		shmdt(sm_ptr) ;
		printf("Agence || détachement mémoire partagé \n");
		exit(0);
	}
}

int main(){
	signal(SIGINT,signal_handler);
	VOL vol;

	//BAL
	int bal_flg = IPC_CREAT | 0666;  
	bal_id = msgget(balK,bal_flg);//creation bal avec droits 660    
	printf("Agence || Création bal avec ID : %d\n",bal_id);

	//SM
	int sm_flg = 0666;
	sm_id = shmget(memK,20*sizeof(VOL),sm_flg);//creation shared memory
	printf("Agence || Ouverture mémoire partagée avec ID : %d\n",sm_id);

	sm_ptr = (VOL*) shmat(sm_id, NULL, 0);
	printf("Agence || Attaché à mémoire partagé \n");

	while(1){
		msgrcv(bal_id,&vol,sizeof(VOL),0,0);
		printf("Agence || %d places pour %s \n",vol.number,vol.destination);
	}
	/*
	   int i;
	   for(i = 0 ; i<20 ; i++){
	   shm[i].number=i;
	   strcpy(shm[i].destination,"TLS");
	   sleep(5);
	   }
	   */

	return 0;
}
