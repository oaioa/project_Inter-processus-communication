# include "useful.h"

int mutex_id;
int mutex;

int sem_id;//libre case mémoire
int sem;

int sm_id;
VOL * sm_ptr;

void signal_handler(int sig){

	if(sig == SIGINT){
		printf("SIGINT \n\n");
		shmdt(sm_ptr);
		printf("\033[1;35mAffichage\033[0m || Détachement mémoire partagée \n");
		sleep(2);
		exit(0);
	}
	if(sig == SIGQUIT){
		printf("SIGQUIT \n");
	}
}

int main()
{
	signal(SIGINT,signal_handler);
	signal(SIGQUIT,signal_handler);
	int wait;
	printf("\033[H\033[2J"); //clear
	int i=0;

	//MUTEX
	int mutex_flg = 0660; //droit écriture et lecture
	mutex_id = semget(mutexK,1,mutex_flg); 
	wait = 10;
	while((-1)==mutex_id && wait>0){
		printf("\033[1;35mAffichage\033[0m || Erreur ouverture mutex : tâche de l'écrivain\n");
		wait --;
		sleep(2);// un essai toutes les deux secondes, 10 essais
		printf("\033[1;35mAffichage\033[0m || Essais restants %d \n",wait);
		mutex_id = semget(mutexK,1,mutex_flg);
	}
	if(wait==0){
		printf("\033[1;35mAffichage\033[0m || Impossible d'ouvrir mutex : je me tue  \n");
		signal_handler(SIGINT);
	}
	printf("\033[1;35mAffichage\033[0m || Ouverture mutex avec id %d \n",mutex_id);
	mutex=semctl(mutex_id,0,GETVAL);
	printf("\033[1;35mAffichage\033[0m || Mutex %d \n",mutex);

	//SEMAPHORE PLACES
	int sem_flg = 0660; //droit écriture et lecture
	sem_id = semget(sem_placeK,1,sem_flg);
	wait = 10;
	while((-1)==mutex_id && wait>0){
		printf("\033[1;35mAffichage\033[0m || Erreur ouverture sémaphore places : tâche de l'écrivain\n");
		wait --;
		sleep(2);// un essai toutes les deux secondes, 10 essais
		printf("\033[1;35mAffichage\033[0m || Essais restants %d \n",wait);
		mutex_id = semget(mutexK,1,mutex_flg);
	}
	if(wait==0){
		printf("\033[1;35mAffichage\033[0m || Impossible d'ouvrir sémaphore : je me tue  \n");
		signal_handler(SIGINT);
	}
	semctl(sem_id,0,SETVAL,0);
	sem=semctl(sem_id,0,GETVAL);
	printf("\033[1;35mAffichage\033[0m || Sémaphore %d \n",sem);

	//MEMOIRE PARTAGEE
	int sm_flg = 0666;//droit écriture et lecture
	sm_id = shmget(memK,20*sizeof(VOL),sm_flg);
	wait = 10 ;
	while((-1)==sm_id && wait>0){
		printf("\033[1;35mAffichage\033[0m || Erreur ouverture mémoire partagée : tâche de l'écrivain\n");
		wait --;
		sleep(2);// un essai toutes les deux secondes, 10 essais
		printf("\033[1;35mAffichage\033[0m || Essais restants %d \n",wait);
		sm_id = shmget(memK,20*sizeof(VOL),sm_flg);
	}
	if(wait==0){
		printf("\033[1;35mAffichage\033[0m || Impossible d'ouvrir mémoire partagée : je me tue  \n");
		signal_handler(SIGINT);
	}
	printf("\033[1;35mAffichage\033[0m || Ouverture mémoire partagée avec ID : %d\n",sm_id);
	sm_ptr = (VOL*) shmat(sm_id, NULL, 0);
	printf("\033[1;35mAffichage\033[0m || Attachement mémoire partagée \n");

	while(1){
		printf("\033[H\033[2J");//clear
		down(mutex);
		printf("\033[1;35mAffichage\033[0m || Down mutex\n");
		printf("--------------------------------------------------------------------\n");
		for(i=0;i<20;i++){
			if(sm_ptr[i].number<0){
				printf("\033[1;35mAffichage\033[0m || ---\n");
			}
			else{
				printf("\033[1;35mAffichage\033[0m || %-2d : %-3d places pour %-20s \n",i,sm_ptr[i].number,sm_ptr[i].destination);
			}//i:case mémoire partagé, sm_ptr[i].number:nombre de places et sa destination avec sm_ptr[i].destination
		}
		printf("--------------------------------------------------------------------\n");
		printf("\033[1;35mAffichage\033[0m || Sleep 15 s\n");
		sleep(1);
		for(i=0;i<20;i++){
			if(sm_ptr[i].number==0){
				sm_ptr[i].number = -1;//si nombre de places descend à 0, remplace par -1,
				down(sem_id);		  //l'écrivain fait un test si place libre pour écrire
				printf("\033[1;35mAffichage\033[0m || Down sémaphore places pour ligne %d \n",i);
			}
		}
		up(mutex);
		printf("\033[1;35mAffichage\033[0m || Up mutex\n");
	}
}

