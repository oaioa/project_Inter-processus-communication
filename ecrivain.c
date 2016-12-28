#include "useful.h"

int descpipe[2];
int tirage_pid;
int dad_pid;

int mutex_id;
int mutex;

int sem_id;
int sem;

int sm_id;
VOL * sm_ptr;

void dad_signal_handler(int sig)
{
	sleep(1);	
	if(sig==SIGINT){
		printf("Écrivain || SIGINT \n");	
		close(descpipe[0]);
		shmdt(sm_ptr);
		printf("Écrivain || Détachement mémoire partagée \n");
		shmctl(sm_id,IPC_RMID,0);
		printf("Écrivain || Supression mémoire partagée \n");
		remove_semaphore(mutex_id);
		printf("Écrivain || Supression mutex \n");
		remove_semaphore(sem_id);
		printf("Écrivain || Supression sémaphore places \n");		
		printf("Écrivain || Programme Évrivain arrete\n");
		exit(0);
	}
}

void son_signal_handler(int sig)
{
	if(sig==SIGINT){
		printf("Tirage || SIGINT\n");
		close(descpipe[1]);
		printf("Tirage || Programme Tirage arrete\n");
		exit(0);
	}

}

int main(int argc, char **argv)
{
	if(pipe(descpipe)==-1)
	{
		printf("Écrivain || erreur création pipe\n");exit(1);
	}
	printf("Écrivain || création du pipe réussie\n");

	int fork_n=fork();
	if(fork_n==-1)
	{
		printf("Écrivain || erreur création du fils\n");
		exit(-1);
	}

	if(fork_n==0) //on est dans le fils (tirage)
	{
		dad_pid=getppid();
		printf("Tirage || PID of dad %d \n",dad_pid);
		printf("Tirage || je suis le fils\n");
		signal(SIGINT,son_signal_handler);
		signal(SIGUSR2,son_signal_handler);
		close(descpipe[0]); // tirage ne s'en sert pas en lecture
		VOL new;
		int r;

		while(1)
		{
			r = rand() % 3;
			strcpy((new.destination),(destinations_possible[r]));
			new.number=rand()%50; //hasard du nombre de places  
			printf("Tirage || écriture faite %s avec %d \n",(new.destination),new.number);
			write(descpipe[1],&new,sizeof(VOL));
			printf("Tirage || wait for 5s\n\n");
			sleep(5);
		}

	}

	else //je suis le père (écrivain)
	{
		signal(SIGINT,dad_signal_handler);
		close(descpipe[1]); // écrivain ne s'en sert pas en écriture
		tirage_pid = fork_n;	
		VOL received;
		
		//MUTEX
		int mutex_flg = 0660|IPC_CREAT;
		mutex_id = semget(mutexK,1,mutex_flg);
		printf("Écrivain || création mutex avec id %d \n",mutex_id);
		semctl(mutex_id,0,SETVAL,1);
		mutex=semctl(mutex_id,0,GETVAL);
		printf("Écrivain || init mutex %d \n",mutex);
		
		//SEMAPHORE PLACES
	int sem_flg = 0660|IPC_CREAT;
	sem_id = semget(sem_placeK,1,sem_flg);
		printf("Écrivain || création sémaphore (places) avec id %d \n",sem_id);
	semctl(sem_id,0,SETVAL,0);
	sem=semctl(sem_id,0,GETVAL);
		printf("Écrivain || init sem %d \n",sem);
		
		//SM
		int sm_flg = IPC_CREAT | 0666;
		sm_id = shmget(memK,20*sizeof(VOL),sm_flg);
		printf("Écrivain || Création de la mémoire partagée avec ID : %d \n",sm_id);
		sm_ptr = (VOL*) shmat(sm_id, NULL,0);
		printf("Écrivain || Attachement à la mémoire partagée\n");
		int i;
		for(i = 0; i<20; i++){
		sm_ptr[i].number = -1;
		}
		printf("Écrivain || Initialisation  mémoire partagée\n");

		int cursor;
		while(1)
		{
			cursor = 0;
			read(descpipe[0],&received,sizeof(VOL));
			printf("Écrivain || Reçu %d places pour %s\n",received.number,received.destination);
			while(cursor<20 && sm_ptr[cursor].number != -1){
				cursor ++ ;
			}	
			if(cursor<20){
				sm_ptr[cursor].number = received.number;
				strcpy(sm_ptr[cursor].destination,received.destination);
				printf("Écrivain || Écriture mémoire partagée case : %d\n",cursor);
			}
			else{
				printf("Écrivain || Mémoire partagée pleine\n");
			}	
		}

	}
}


