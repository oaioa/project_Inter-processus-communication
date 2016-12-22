#include "../useful.h"

void signal_handler(int signal){
	printf("Kill received %d !\n",signal); 
	exit(0);
}


int main(){
	signal(SIGINT,signal_handler);
	VOL v1;
	v1.number = 8;
	strcpy(v1.destination,"Bonjour");

	int msgflg =  0666;  
	int idMes = msgget(balK,msgflg); 

	printf("Message with ID : %d\n",idMes);

	msgsnd(idMes,&v1,sizeof(VOL),0); //message envoyé

	return 0;

}


