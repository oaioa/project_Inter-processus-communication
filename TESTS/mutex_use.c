#include "../useful.h"

int mutex_id;
int mutex;

void signal_handler(int sig)
{
	printf("kill effectué\n");
	if(sig == SIGQUIT){
		printf("SIGQUIT \n\n");
		exit(0);
	}
}

int main()
{
	signal(SIGINT,signal_handler);
	signal(SIGQUIT,signal_handler);
	int flag = 0660;
	mutex_id = semget(mutexK,1,flag);
	mutex=semctl(mutex_id,0,GETVAL);
	printf("mutex : %d \n",mutex);

	while(1){
		down(mutex_id);
		printf("down mutex: %d \n",semctl(mutex_id,0,GETVAL));
		printf("action 3s \n\n");
		sleep(3);
		up(mutex_id);
		printf("up mutex: %d \n",mutex=semctl(mutex_id,0,GETVAL));
	}

}

