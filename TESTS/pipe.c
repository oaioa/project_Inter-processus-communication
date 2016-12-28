#include "../useful.h"

int descpipe[2];
int son_pid;
int dad_pid;

void dad_signal_handler(int sig)
{
	sleep(1);	
	if(sig==SIGINT){
		printf("DAD || SIGINT \n");
		printf("DAD || SIGINT to son with PID %d \n",son_pid);
		kill(son_pid,SIGUSR2);
		printf("DAD || sleep 5s\n");
		sleep(5);
		exit(0);
	}
}

void son_signal_handler(int sig)
{
	if(sig==SIGINT){
		close(descpipe[1]);
		printf("SON || SIGINT reçu\n");
		printf("SON || Programme Tirage arrete\n");
		exit(0);
	}
	if(sig==SIGUSR2){
		close(descpipe[1]);
		printf("SON || Programme Tirage arrete\n");
		exit(0);
	}
	
}

int main(int argc, char **argv)
{
	if(pipe(descpipe)==-1)
	{
		printf("erreur création pipe\n");exit(1);
	}
	printf("création du pipe réussie\n");

	int fork_n=fork();
	if(fork_n==-1)
	{
		printf("erreur création du fils\n");
		exit(-1);
	}

	if(fork_n==0) //on est dans le fils (tirage)
	{
		dad_pid=getppid();
		printf("SON || PID of dad %d \n",dad_pid);
		printf("SON || je suis le fils\n");
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
			printf("SON || écriture faite %s avec %d \n",(new.destination),new.number);
			write(descpipe[1],&new,sizeof(VOL));
			printf("SON || wait for 5s\n\n");
			sleep(5);
		}

	}

	else //je suis le père (écrivain)
	{
		son_pid = fork_n;	
		printf("DAD || here \n");
		VOL received;
		signal(SIGINT,dad_signal_handler);
		close(descpipe[1]); // écrivain ne s'en sert pas en écriture

		while(1)
		{
			while(read(descpipe[0],&received,sizeof(VOL))){
				printf("DAD || %d places pour %s\n",received.number,received.destination);
			}
	}

		close(descpipe[0]);
	}
}
