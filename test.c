#include "useful.h"
void signal_handler(int signal){
	if(signal==SIGINT){
		exit(0);
	}
}

int main(){
	signal(SIGINT,signal_handler);
	printf("\033[039;""43m""blue""\033[0m\n");
	printf("\033[0;""32m""blue""\033[0m\n");
	printf("\033[1;""31m""blue""\033[0m\n");
	printf("\033[1;""32m""blue""\033[0m\n");
	printf("\033[1;""33m""blue""\033[0m\n");
	printf("\033[1;""34m""blue""\033[0m\n");
	printf("\033[4;""32m""blue""\033[0m\n");
	printf("\033[9;""32m""blue""\033[0m\n");
	printf("\033[3;""32m""blue""\033[0m\n");
	printf("\033[439;""32m""blue""\033[0m\n");
	printf("\033[5;""32m""blue""\033[0m\n");
	return 0;

}



