#include "../useful.h"

int smid; //ID de la mémoire partagée
VOL * vol_sm; //pointeur sur la mémoie partégée

void signal_handler(int signal){
	printf("Kill received %d !\n",signal); 
	shmdt(vol_sm); //détachement de la mémoire partagée 
	exit(0);
}

int main(){
	signal(SIGINT,signal_handler);

	int msgflg = 0666;  
	smid = shmget(memK,20*sizeof(VOL),msgflg);//creation shared memory
	vol_sm = (VOL*) shmat(smid, NULL, 0);
	printf("Attaché à mémoire partagé \n");
	int i;
	while(1){
		for(i = 0 ; i<20 ; i++){
			printf("%d for %s \n",vol_sm[i].number,(vol_sm[i].destination));
		}
		printf("\n\n");
		sleep(1);
	}

	return 0;
}
