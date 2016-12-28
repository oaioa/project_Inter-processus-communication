#include "../useful.h"

void father_handler(int signo)
{	
	if (signo == SIGINT){
		printf("DAD || SIGINT received\n");
	}
	if (signo == SIGUSR1){
		printf("DAD || SIGUSR1 reçu cabron ! \n");
	}		
	//	printf("DAD || Sleep for 10s\n");
	//	sleep(20);
}
void son_handler(int signo)
{	
	if (signo == SIGINT){
		printf("SON || SIGINT received\n");
	}
	if (signo == SIGUSR1){
		printf("SON || SIGUSR1 received\n");
	}
}
int main(int argc, char **argv)
{
	int num;
	num=fork();
	if (num==-1){
		printf("erreur \n");
	}
	else if (num==0){ //Je suis le fils
		printf("SON || born ! \n");
		int papa=getppid();
		printf("SON || Dad pid %d \n",papa);
		signal(SIGINT,son_handler);
		signal(SIGUSR1,son_handler);
		while(1){
			printf("SON || Sleep for 5s\n");
			sleep(5);
			printf("SON || Send SIGINT to dad \n");
			kill(papa, SIGINT);
			printf("SON || wake up dad! \n");
			sleep(1);
		}
	}
	else{ //Je suis le père
		printf("DAD || Son pid %d \n",num);
		int retpause; 
		signal(SIGINT,father_handler);
		signal(SIGUSR1,father_handler);
		while(1){
			printf("DAD || Wait for SIGINT \n");
			retpause=pause();
			printf("DAD || la valeur retournée est %d \n", retpause);
			sleep(4);
		}
	}
	return 0;
}
