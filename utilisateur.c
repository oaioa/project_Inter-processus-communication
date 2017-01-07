#include "useful.h"

int bal_id;

void signal_handler(int signal){
	if(signal==SIGUSR1){
		printf("\033[1;33mUtilisateur\033[0m || SIGURS1 !\n");
		printf("\033[1;33mUtilisateur\033[0m ||\033[3;32m Requête possible \033[0m\n");
	}
	if(signal==SIGUSR2){
		printf("\033[1;33mUtilisateur\033[0m || SIGURS2 !\n");
		printf("\033[1;33mUtilisateur\033[0m ||\033[3;31m Requête impossible \033[0m\n");
	}

	if(signal==SIGINT){
		printf("\033[1;33mUtilisateur\033[0m || SIGINT !\n");
		printf("\033[1;33mUtilisateur\033[0m || Fermeture \n");
		sleep(2);
		exit(0);
	}
}

int main(){

	signal(SIGINT,signal_handler);
	signal(SIGUSR1,signal_handler);
	signal(SIGUSR2,signal_handler);

	int wait;

	//BAL
	int bal_flg = 0666;
	bal_id = msgget(balK,bal_flg);//attachement bal avec droits écriture et lecture
	wait = 10;
	//Si attachement impossible
	while((-1)==bal_id && wait>0){
		printf("\033[1;33mUtilisateur\033[0m || Erreur ouverture BAL : tâche de l'agence\n");
		wait --;
		sleep(2);//essai 10 fois toutes les 2 sec d'ouvrir la boîte aux lettres
		printf("\033[1;33mUtilisateur\033[0m || Essais restants %d \n",wait);
		bal_id = semget(balK,1,bal_flg);
	}
	//Arrêt des essais d'attachement
	if(wait==0){
		printf("\033[1;33mUtilisateur\033[0m || Impossible d'ouvrir BAL : je me tue  \n");
		signal_handler(SIGINT);
	}
	printf("\033[1;33mUtilisateur\033[0m || ID BAL : %d\n",bal_id);

	int PID = getpid();
	printf("\033[1;33mUtilisateur\033[0m || Mon PID : %d\n",PID);
	VOL v1;
	v1.PID = PID ;
	int number_input;
	char destination_input[20];

	while(1){
		sleep(1); //attente de fin de signal, sinon saute le printf
		printf("\033[1;33mUtilisateur\033[0m || Entrer la destination \n");
		scanf("%s",destination_input);
		printf("\033[1;33mUtilisateur\033[0m || Entrer le nombre de places \n");

		while(scanf("%d",&number_input)!=1 || number_input < 0){
			printf("\033[1;33mUtilisateur\033[0m || Ce n'est pas un nombre positif\n");
        		purge_buffer();
			printf("\033[1;33mUtilisateur\033[0m || Entrer un nombre de places positif \n");
		}

		v1.number = number_input ;
		strcpy(v1.destination,destination_input);
		msgsnd(bal_id,(void *) & v1,sizeof(VOL),0); //message envoyé
		printf("\033[1;33mUtilisateur\033[0m || Message envoyé \n");
	}
	return 0;
}
