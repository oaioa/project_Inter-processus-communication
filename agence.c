#include "useful.h"

//GLOBAL
int bal_id;

int mutex_id;
int mutex;

int sm_id;
VOL * sm_ptr;

void signal_handler(int signal){
	if( signal == SIGINT){
		printf("Agence || SIGINT !\n"); 
		msgctl(bal_id,IPC_RMID,NULL);
		printf("Agence || Supression bal \n");
		shmdt(sm_ptr) ;
		printf("Agence || détachement mémoire partagé \n");
		exit(0);
	}
}

int main(){
	signal(SIGINT,signal_handler);
	VOL vol;

	//BAL
	int bal_flg = IPC_CREAT | 0666;  
	bal_id = msgget(balK,bal_flg);//creation bal avec droits 660    
	printf("Agence || Création bal avec ID : %d\n",bal_id);

	//MUTEX
	int mutex_flg = 0660;
	mutex_id = semget(mutexK,1,mutex_flg);
	printf("Agence || ouverture mutex avec id %d \n",mutex_id);
	mutex=semctl(mutex_id,0,GETVAL);
	printf("Agence || mutex %d \n",mutex);

	//SM
	int sm_flg = 0666;
	sm_id = shmget(memK,20*sizeof(VOL),sm_flg);//creation shared memory
	printf("Agence || Ouverture mémoire partagée avec ID : %d\n",sm_id);
	sm_ptr = (VOL*) shmat(sm_id, NULL, 0);
	printf("Agence || Attaché à mémoire partagé \n");

	int sm_case;
	int sm_vols_destination;
	int sm_vols_number_max;
	while(1){
		sm_vols_number_max=0;
		sm_vols_destination=0;
		msgrcv(bal_id,&vol,sizeof(VOL),0,0);
		printf("Agence || utilisateur %d --> %d places pour %s \n",vol.PID,vol.number,vol.destination);

		sm_case=0;
		down(mutex);	
		printf("Agence || down mutex \n");

		printf("-----------------------------------------------------\n");
		for(sm_case=0;sm_case<20;sm_case++){
			if(sm_ptr[sm_case].number == -1){
				printf("%-2d : VIDE! \n", sm_case);
			}
			else{
				printf("%-2d : %-20s ==  %-20s ? %-4d \n",sm_case,vol.destination,sm_ptr[sm_case].destination,strcmp(vol.destination,sm_ptr[sm_case].destination));
			}

		}
		printf("-----------------------------------------------------\n");

		for(sm_case=0;sm_case<20;sm_case++){
			if(strcmp(vol.destination,sm_ptr[sm_case].destination)==0){
				sm_vols_destination++;
				if(sm_ptr[sm_case].number>=vol.number){
					printf("Agence || Requête possible ! \n");
					sm_ptr[sm_case].number-=vol.number;
					printf("Agence || Mise à jour places \n");
					sm_vols_number_max = 0 ;
					kill(vol.PID,SIGUSR1);
					break;
				}
				else{
					if(sm_vols_number_max<sm_ptr[sm_case].number){
						sm_vols_number_max=sm_ptr[sm_case].number;
					}
				}
			}
		}
		if(sm_vols_destination == 0){
			printf("Agence || aucun vol avec destination \n");
			kill(vol.PID,SIGUSR2);
		}
		else{
			if(sm_vols_number_max>0){
				printf("Agence || max place pour destinationation voulue : %d\n",sm_vols_number_max);
				kill(vol.PID,SIGUSR2);
			}
		}


		up(mutex);
		printf("Agence || up mutex \n");
	}
	return 0;
}
