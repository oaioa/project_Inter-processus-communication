#include "useful.h"
// !! pour taile bal sizeof(struct MESSAGE)
// il faut faire la somme de chaque composantes indépendamments
//
void signal_handler(int signal){
	if(signal==SIGUSR1){
		printf("Utilisateur || SIGURS1 !\n"); 
		printf("Utilisateur ||\033[3;32m Requête possible \033[0m\n");
	}
	if(signal==SIGUSR2){
		printf("Utilisateur || SIGURS2 !\n"); 
		printf("Utilisateur ||\033[3;31m Requête impossible \033[0m\n");
	}
	if(signal==SIGINT){
		printf("Utilisateur || SIGINT !\n"); 
		printf("Utilisateur || Fermeture \n");
		exit(0);
	}
}

int main(){

	signal(SIGINT,signal_handler);
	signal(SIGUSR1,signal_handler);
	signal(SIGUSR2,signal_handler);
	

	//BAL
	int msgflg = 0666;  
	int idMes = msgget(balK,msgflg);//attachement bal avec droits 660    
	printf("Utilisateur || ID bal : %d\n",idMes);
	
	int PID = getpid();
	printf("Utilisateur || mon PID : %d\n",PID);
	VOL v1;
	v1.PID = PID ;
	int number_input;
	char destination_input[20];

	while(1){
		printf("Utilisateur || Entrez la destination \n");
		scanf("%s",destination_input);
		printf("Utilisateur || Entrez le nombre de place \n");
		scanf("%d",&number_input);
		v1.number = number_input ;
		strcpy(v1.destination,destination_input);
		msgsnd(idMes,(void *) & v1,sizeof(VOL),0); //message envoyé
		printf("Utilisateur || Message envoyé \n");
		printf("Utilisateur || pause 4s\n");
		sleep(4);
	}
	return 0;

}



