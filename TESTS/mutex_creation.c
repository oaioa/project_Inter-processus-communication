#include "../useful.h"

int mutex_id;
int mutex;

void signal_handler(int sig)
{
	printf("kill effectué\n");
	if(sig == SIGQUIT){
		printf("SIGQUIT \n\n");
		remove_semaphore(mutex_id);
		exit(0);
	}
}

int main()
{
	signal(SIGINT,signal_handler);
	signal(SIGQUIT,signal_handler);
	int flag = 0660|IPC_CREAT;
	mutex_id = semget(mutexK,1,flag);
	semctl(mutex_id,0,SETVAL,1);
	mutex=semctl(mutex_id,0,GETVAL);
	printf("init mutex %d \n",mutex);

	while(1){
		down(mutex_id);
		printf("down  mutex: %d \n",semctl(mutex_id,0,GETVAL));
		printf("action 7\n\n");
		sleep(7);
		up(mutex_id);	
		printf("up mutex: %d \n",semctl(mutex_id,0,GETVAL));
	}
}
