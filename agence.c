#include "useful.h"

//GLOBAL
int bal_id;

int mutex_id;
int mutex;

int sm_id;
VOL * sm_ptr;

void signal_handler(int signal){
	if( signal == SIGINT){
		printf("\033[1;32mAgence\033[0m || SIGINT !\n");
		msgctl(bal_id,IPC_RMID,NULL);
		printf("\033[1;32mAgence\033[0m || Suppression BAL \n");
		shmdt(sm_ptr) ;
		printf("\033[1;32mAgence\033[0m || Détachement mémoire partagée \n");
		shmctl(sm_id,IPC_RMID,0);
		printf("\033[1;32mAgence\033[0m || Suppression mémoire partagée \n");

		printf("\033[1;32mAgence\033[0m || Kill processus utilisateur(s)dans \n");
		system("pkill -SIGINT utilisateur")	;//ferme tous les utilisateurs
		sleep(2);//laisse le temps de voir l'affichage
		exit(0);
	}
}

int main(){
	signal(SIGINT,signal_handler);
	int wait;
	VOL vol;
	//BAL
	int bal_flg = IPC_CREAT | 0666;
	bal_id = msgget(balK,bal_flg);//creation bal avec droits lecture et écriture
	if((-1)==bal_id){
		printf("\033[1;32mAgence\033[0m || Erreur création BAL !!!\n");
		exit(1);
	}
	printf("\033[1;32mAgence\033[0m || Création BAL avec ID : %d\n",bal_id);

	//MUTEX
	int mutex_flg = 0660;//droits lecture et écriture
	mutex_id = semget(mutexK,1,mutex_flg);
	wait = 10;
	while((-1)==mutex_id && wait>0){
		printf("\033[1;32mAgence\033[0m || Erreur ouverture mutex : tâche de l'écrivain\n");
		wait --;
		sleep(2);//attente de 2sec entre chaque essai (10 essais)
		printf("\033[1;32mAgence\033[0m || Essais restants %d \n",wait);
		mutex_id = semget(mutexK,1,mutex_flg);
	}
	if(wait==0){
		printf("\033[1;32mAgence\033[0m || Impossible d'ouvrir mutex : je me tue  \n");
		signal_handler(SIGINT);
	}
	printf("\033[1;32mAgence\033[0m || Ouverture mutex avec id %d \n",mutex_id);
	mutex=semctl(mutex_id,0,GETVAL);
	printf("\033[1;32mAgence\033[0m || Mutex %d \n",mutex);

	//SM
	int sm_flg = 0666;//droits lecture et écriture
	sm_id = shmget(memK,20*sizeof(VOL),sm_flg);//creation shared memory
	wait = 10 ;
	while((-1)==sm_id && wait>0){
		printf("\033[1;32mAgence\033[0m || Erreur ouverture mémoire partagée : tâche de l'écrivain\n");
		wait --;
		sleep(2);//2sec d'attente entre chaque essai
		printf("\033[1;32mAgence\033[0m || Essais restants %d \n",wait);
		sm_id = shmget(memK,20*sizeof(VOL),sm_flg);
	}
	if(wait==0){
		printf("\033[1;32mAgence\033[0m || Impossible d'ouvrir mémoire partagée : je me tue  \n");
		signal_handler(SIGINT);
	}
	printf("\033[1;32mAgence\033[0m || Ouverture mémoire partagée avec ID : %d\n",sm_id);
	sm_ptr = (VOL*) shmat(sm_id, NULL, 0);
	printf("\033[1;32mAgence\033[0m || Attachement mémoire partagée \n");

	int sm_case;
	int sm_vols_destination;
	int sm_vols_number_max;
	while(1){
		sm_vols_number_max=0;
		sm_vols_destination=0;
		msgrcv(bal_id,&vol,sizeof(VOL),0,0);
		printf("\033[1;32mAgence\033[0m || Utilisateur %d --> %d places pour %s \n",vol.PID,vol.number,vol.destination);

		sm_case=0;
		down(mutex);
		printf("\033[1;32mAgence\033[0m || Down mutex \n");

		for(sm_case=0;sm_case<20;sm_case++){//parcours de la mémoire partagée
			if(strcmp(vol.destination,sm_ptr[sm_case].destination)==0){
				sm_vols_destination++;
				if(sm_ptr[sm_case].number>=vol.number){
					printf("\033[1;32mAgence\033[0m || Requête possible ! \n");
					sm_ptr[sm_case].number-=vol.number; //retire le nombre de places demandées
					printf("\033[1;32mAgence\033[0m || Mise à jour places \n");
					sm_vols_number_max = 0 ;
					kill(vol.PID,SIGUSR1);
					break;
				}
				else{
					if(sm_vols_number_max<sm_ptr[sm_case].number){
						sm_vols_number_max=sm_ptr[sm_case].number;
					}
				}
			}
		}
		if(sm_vols_destination == 0){
			printf("\033[1;32mAgence\033[0m || Aucun vol avec destination \n");
			kill(vol.PID,SIGUSR2);
		}
		else{
			if(sm_vols_number_max>0){
				printf("\033[1;32mAgence\033[0m || Maximum places pour destination voulue : %d\n",sm_vols_number_max);
				kill(vol.PID,SIGUSR2);
			}
		}


		up(mutex);
		printf("\033[1;32mAgence\033[0m || Up mutex \n");
	}
	return 0;
}
