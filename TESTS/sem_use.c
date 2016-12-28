#include "../useful.h"

int sem_id;
int sem;

void signal_handler(int sig)
{
	printf("kill effectué\n");
	if(sig == SIGQUIT){
		printf("SIGQUIT \n\n");
		exit(0);
	}
	if(sig == SIGINT){
		printf("SIGINT \n");
		down(sem_id);
		printf("down sem : %d \n",semctl(sem_id,0,GETVAL));
	}
}

int main()
{
	signal(SIGINT,signal_handler);
	signal(SIGQUIT,signal_handler);
	int flag = 0660;
	sem_id = semget(sem_placeK,1,flag);
	sem=semctl(sem_id,0,GETVAL);
	printf("sem : %d \n",sem);

	while(1){
		if(semctl(sem_id,0,GETVAL)>19){
			printf("impossible sem top élévé \n");
			sleep(1);
		}
		else{
			up(sem_id);
			printf("up sem: %d \n",semctl(sem_id,0,GETVAL));
			sleep(2);
		}
	}

}
