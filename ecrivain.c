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

int quit_count = 0;

void dad_signal_handler(int sig)
{
	sleep(1);	

	//^c
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

	//^z
	if(sig==SIGTSTP){
		printf("Écrivain || SIGTSTP \n");
		printf("Ecrivain|| appuyer sur entrée \n");
		getchar();
		printf("Tirage || fin pause \n");
	}

	//^'\' 
	if(sig==SIGQUIT){
		printf("Écrivain || SIGQUIT \n");
		sleep(1);
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

	//^z
	if(sig==SIGTSTP){
		printf("Tirage || SIGTSTP \n");
		printf("Tirage || appuyer sur entrée \n");
		getchar();
		printf("Tirage || fin pause \n");

	}

	//^'\' 
	if(sig==SIGQUIT){
		printf("Tirage || SIGQUIT \n");
		sleep(1);
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

	//SEMAPHORE PLACES
	int sem_flg = 0660|IPC_CREAT;
	sem_id = semget(sem_placeK,1,sem_flg);
	printf("Écrivain || création sémaphore (places) avec id %d \n",sem_id);
	semctl(sem_id,0,SETVAL,0);
	sem=semctl(sem_id,0,GETVAL);
	printf("Écrivain || init sem %d \n",sem);

	if(fork_n==0) //on est dans le fils (tirage)
	{
		signal(SIGTSTP,son_signal_handler);
		signal(SIGQUIT,son_signal_handler);
		signal(SIGINT,son_signal_handler);
		close(descpipe[0]); // tirage ne s'en sert pas en lecture

		dad_pid=getppid();
		printf("Tirage || PID of dad %d \n",dad_pid);
		VOL new;

		int destinations = 0 ;
		while(destinations_possible[destinations][0]!= '.'){
			destinations++;
		}
		printf("Tirage || %d destinations sont desponibles \n",destinations);
		int destination_random;
		while(1)
		{

			destination_random = rand() % destinations;
			strcpy((new.destination),(destinations_possible[destination_random]));
			new.number=rand()%50; //hasard du nombre de places  
			printf("Tirage || écriture faite %s avec %d \n",(new.destination),new.number);
			write(descpipe[1],&new,sizeof(VOL));
			printf("Tirage || wait for 5s\n\n");
			sleep(1);
		}

	}

	else //je suis le père (écrivain)
	{
		signal(SIGINT,dad_signal_handler);
		signal(SIGTSTP,dad_signal_handler);
		signal(SIGQUIT,dad_signal_handler);
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
			read(descpipe[0],&received,sizeof(VOL));
			printf("Écrivain || Reçu %d places pour %s\n",received.number,received.destination);	

			if(semctl(sem_id,0,GETVAL)<20){
				for(cursor=0;cursor<20;cursor++){
					if(sm_ptr[cursor].number == -1){
						down(mutex_id);
						printf("Écrivain || down mutex\n");
						sm_ptr[cursor].number = received.number;
						strcpy(sm_ptr[cursor].destination,received.destination);
						printf("Écrivain || Écriture mémoire partagée case : %d\n",cursor);
						up(sem_id);
						printf("Écrivain || up sem %d \n",semctl(sem_id,0,GETVAL));
						up(mutex_id);
						printf("Écrivain || up mutex %d \n",semctl(mutex_id,0,GETVAL));
						break;
					}
				}

			}
			else{
				printf("Écrivain || Mémoire partagée pleine\n");
			}	
		}

	}
	return 0;

}


