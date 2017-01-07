#include "useful.h"

int descpipe[2];
int tirage_pid;
int dad_pid;

int mutex_id;
int mutex;

int sem_id;//espace mémoire libre?
int sem;

int sm_id;
VOL * sm_ptr;

int quit_count = 0;

void dad_signal_handler(int sig)
{
	sleep(1);

	//^c
	if(sig==SIGINT){
		printf("\033[1;31mÉcrivain\033[0m || SIGINT \n");
		close(descpipe[0]);
		shmdt(sm_ptr);
		printf("\033[1;31mÉcrivain\033[0m || Détachement mémoire partagée \n");
		remove_semaphore(mutex_id);
		printf("\033[1;31mÉcrivain\033[0m || Suppression mutex \n");
		remove_semaphore(sem_id);
		printf("\033[1;31mÉcrivain\033[0m || Suppression sémaphore places \n");
		printf("\033[1;31mÉcrivain\033[0m || Programme Écrivain arrêté\n");
		sleep(2);//laisse le temps d'afficher
		exit(0);
	}

	//^z
	if(sig==SIGTSTP){
		printf("\033[1;31mÉcrivain\033[0m || SIGTSTP \n");
		printf("Ecrivain|| Appuyer sur entrée \n");
		getchar();
		printf("Tirage || Fin pause \n");
	}

	//^'\'
	if(sig==SIGQUIT){
		printf("\033[1;31mÉcrivain\033[0m || SIGQUIT \n");
		sleep(1);
	}

}

void son_signal_handler(int sig)
{
	if(sig==SIGINT){
		printf("\033[1;36mTirage\033[0m || SIGINT\n");
		close(descpipe[1]);
		printf("\033[1;36mTirage\033[0m || Programme Tirage arrêté\n");
		sleep(2);
		exit(0);
	}

	//^z
	if(sig==SIGTSTP){
		printf("\033[1;36mTirage\033[0m || SIGTSTP \n");
		printf("\033[1;36mTirage\033[0m || Appuyer sur entrée \n");
		getchar();
		printf("\033[1;36mTirage\033[0m || Fin pause \n");

	}

	//^'\'
	if(sig==SIGQUIT){
		printf("\033[1;36mTirage\033[0m || SIGQUIT \n");
		sleep(1);
	}

}

int main(int argc, char **argv)
{

	if(pipe(descpipe)==-1)
	{
		printf("\033[1;31mÉcrivain\033[0m || Erreur création tube\n");exit(1);
	}
	printf("\033[1;31mÉcrivain\033[0m || Création du tube réussie\n");

	int fork_n=fork();
	if(fork_n==-1)
	{
		printf("\033[1;31mÉcrivain\033[0m || Erreur création du fils\n");
		exit(-1);
	}

	//SEMAPHORE PLACES
	int sem_flg = 0660|IPC_CREAT; //droit d'écriture et de lecture
	sem_id = semget(sem_placeK,1,sem_flg);
	if((-1)==sem_id){
			printf("\033[1;31mÉcrivain\033[0m || Erreur création sémaphores places !!!\n");
		exit(1);
	}
printf("\033[1;31mÉcrivain\033[0m || Création sémaphore (places) avec id %d \n",sem_id);
	semctl(sem_id,0,SETVAL,0);
	sem=semctl(sem_id,0,GETVAL);
	printf("\033[1;31mÉcrivain\033[0m || Initialisation sémaphore %d \n",sem);

	if(fork_n==0) //on est dans le fils (tirage)
	{
		signal(SIGTSTP,son_signal_handler);
		signal(SIGQUIT,son_signal_handler);
		signal(SIGINT,son_signal_handler);
		close(descpipe[0]); // tirage ne s'en sert pas en lecture

		dad_pid=getppid();
		printf("\033[1;36mTirage\033[0m || PID du père %d \n",dad_pid);
		VOL new;

		int destinations = 0 ;
		while(destinations_possible[destinations][0]!='.'){//permet de compter le nombre de destinations dans le tableau
			destinations++;
		}
		printf("\033[1;36mTirage\033[0m || %d destinations sont disponibles \n",destinations);
		int destination_random;
		while(1)
		{
			sleep(1);
			destination_random = rand() % destinations;//tire au hasard un nombre correspondant à une case du tableau destinations
			strcpy((new.destination),(destinations_possible[destination_random]));//copie la destination dans une variable
			new.number=rand()%50; //hasard du nombre de places
			printf("\033[H\033[2J");//clear
			printf("\033[1;36mTirage\033[0m || Ecriture faite %s avec %d \n",(new.destination),new.number);
			write(descpipe[1],&new,sizeof(VOL));//écriture dans le tube
			printf("\033[1;36mTirage\033[0m || Attendre 10sec\n\n");
			sleep(10);//un tirage toutes les 10 sec
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
		int mutex_flg = 0660|IPC_CREAT;//droits écriture et lecture
		mutex_id = semget(mutexK,1,mutex_flg);
		if((-1)==mutex_id){
		printf("\033[1;31mÉcrivain\033[0m || Erreur création mutex !!!\n");
		dad_signal_handler(SIGINT);
		}
		printf("\033[1;31mÉcrivain\033[0m || Création mutex avec id %d \n",mutex_id);
		semctl(mutex_id,0,SETVAL,1);
		mutex=semctl(mutex_id,0,GETVAL);
		printf("\033[1;31mÉcrivain\033[0m || Initialisation mutex %d \n",mutex);

		//SM
		int sm_flg = IPC_CREAT | 0666;//droits écriture et lecture
		sm_id = shmget(memK,20*sizeof(VOL),sm_flg);
		if((-1)==mutex_id){
		printf("\033[1;31mÉcrivain\033[0m || Erreur création mémoire partagée !!!\n");
		dad_signal_handler(SIGINT);
		}
		printf("\033[1;31mÉcrivain\033[0m || Création mémoire partagée avec ID : %d \n",sm_id);
		sm_ptr = (VOL*) shmat(sm_id, NULL,0);
		printf("\033[1;31mÉcrivain\033[0m || Attachement à la mémoire partagée\n");
		int case_sm;
		int case_string;
		for(case_sm = 0; case_sm<20; case_sm++){//parcours de la mémoire partagée
			sm_ptr[case_sm].number = -1;
			for(case_string = 0; case_string<20; case_string++){
				sm_ptr[case_sm].destination[case_string]=0;//bourrage avec 0s
			}
		}
		printf("\033[1;31mÉcrivain\033[0m || Initialisation  mémoire partagée\n");

		int cursor;
		while(1)
		{
			read(descpipe[0],&received,sizeof(VOL)); //lecture tube
			printf("\033[1;31mÉcrivain\033[0m || Reçu %d places pour %s\n",received.number,received.destination);

			if(semctl(sem_id,0,GETVAL)<20){
				for(cursor=0;cursor<20;cursor++){
					if(sm_ptr[cursor].number == -1){
						down(mutex_id);
						printf("\033[1;31mÉcrivain\033[0m || Down mutex\n");
						sm_ptr[cursor].number = received.number;
						strcpy(sm_ptr[cursor].destination,received.destination);//copie de la destination dans la mémoire partagée
						printf("\033[1;31mÉcrivain\033[0m || Écriture mémoire partagée case : %d\n",cursor);
						up(sem_id);
						printf("\033[1;31mÉcrivain\033[0m || Up sémaphore %d \n",semctl(sem_id,0,GETVAL));
						up(mutex_id);
						printf("\033[1;31mÉcrivain\033[0m || Up mutex %d \n",semctl(mutex_id,0,GETVAL));
						break;
					}
				}

			}
			else{
				printf("\033[1;31mÉcrivain\033[0m || Mémoire partagée pleine\n");
			}
		}

	}
	return 0;

}


