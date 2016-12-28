# include "useful.h"

int mutex_id;
int mutex;

int sem_id;
int sem;

int sm_id;
VOL * sm_ptr;

void signal_handler(int sig){

	if(sig == SIGQUIT){
		printf("SIGQUIT \n\n");
		shmdt(sm_ptr);
		printf("Affichage || Détachement mémoire partagée \n");
	}
	if(sig == SIGINT){
		printf("SIGINT \n");
		exit(0);
	}
}

int main()
{
	signal(SIGINT,signal_handler);
	signal(SIGQUIT,signal_handler);
	int i=0;

	//MUTEX
	int mutex_flg = 0660;
	mutex_id = semget(mutexK,1,mutex_flg);
	printf("Affichage || ouverture mutex avec id %d \n",mutex_id);
	mutex=semctl(mutex_id,0,GETVAL);
	printf("Affichage || mutex %d \n",mutex);

	//SEMAPHORE PLACES
	int sem_flg = 0660;
	sem_id = semget(sem_placeK,1,sem_flg);
	semctl(sem_id,0,SETVAL,0);
	sem=semctl(sem_id,0,GETVAL);
	printf("Écrivain || sem %d \n",sem);

	//SM
	int sm_flg = 0666;  
	sm_id = shmget(memK,20*sizeof(VOL),sm_flg);
	printf("Affichage || Ouverture mémoire partagée avec ID : %d\n",sm_id);
	sm_ptr = (VOL*) shmat(sm_id, NULL, 0);
	printf("Affichage || Attachement mémoire partagée \n");

	while(1){
		down(mutex);
		printf("Affichage || down mutex\n\n\n");
		printf("--------------------------------------------------------------------\n");
		for(i=0;i<20;i++){
			printf("Affichage || %d : %d places pour %s \n",i,sm_ptr[i].number,sm_ptr[i].destination); 
		}
		printf("--------------------------------------------------------------------\n");
		up(mutex);
		printf("Affichage || up mutex\n");
		printf("Affichage || sleep 15 s\n");
		sleep(15);
	}
}

