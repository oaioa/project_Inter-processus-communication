#include "../useful.h"

int idBal;

void signal_handler(int signal){
	printf("Kill received  !\n");//message d'entrée de fonction 
	msgctl(idBal,IPC_RMID,NULL);//suppression bal (avec ID = idBal)
	exit(0);
	
}


int main(){
	signal(SIGINT,signal_handler);

	VOL v1;

	idBal=msgget(balK,0666|IPC_CREAT);
	while(1){
		if((-1)==idBal){
			printf("Erreur creation boite au lettre\n");
			exit(1);
		}	
		printf("code received : %d \n",msgrcv(idBal,&v1,sizeof(VOL),0,0));
		printf("%d free for %s \n",v1.number,v1.destination);
	}
	return 0;

}


