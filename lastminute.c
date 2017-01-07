#include "useful.h"
void signal_handler(int signal){
	printf("\033[1;36mLastminute\033[0m || Voulez-vous quitter ?\n");
	printf("\033[1;36mLastminute\033[0m || o pour oui autre chose pour non\n");
	char quit;
	scanf("%c",&quit);
	if(quit=='o'){
		if(signal==SIGINT){
			printf("\033[1;36mLastminute\033[0m || L'agence va fermer dans 4s dépêchez-vous \n");
			sleep(4);//temps d'affichage
			system("pkill -SIGINT agence")	;
			printf("\033[1;36mLastminute\033[0m || Agence fermée\n");
			sleep(2);
			printf("\033[1;36mLastminute\033[0m || Fermeture affichage\n");
			system("pkill -SIGINT affichage");
			printf("\033[1;36mLastminute\033[0m || Affichage fermée\n");
			sleep(2);
			printf("\033[1;36mLastminute\033[0m || Fermeture écrivain et tirage\n");
			system("pkill -SIGINT ecrivain");
			printf("\033[1;36mLastminute\033[0m || Au revoir !\n");
			sleep(3);
			exit(0);
		}
	}
}

int main(){
	signal(SIGINT,signal_handler);
	printf("\033[1;36mLastminute\033[0m || Début du logiciel lastMinute\n");
	sleep(1);
	printf("\033[1;36mLastminute\033[0m || Lancement de l'écrivain\n");
	system("gnome-terminal --geometry='55x25+0-25' -e './ecrivain'");//taille de la fenêtre du terminal ouvert

	sleep(1);
	printf("\033[1;36mLastminute\033[0m || Lancement de l'agence et d'utilisateur\n");
	system("gnome-terminal --geometry=60x20-0+20 -x bash -c './agence & sleep 1;./utilisateur'");

	sleep(1);
	printf("\033[1;36mLastminute\033[0m || Lancement de l'affichage\n");
	system("gnome-terminal --geometry=70x25-0-25 -e './affichage'");

	printf("\033[1;36mLastminute\033[0m || Crtl + C pour quitter le programme \n");


	while(1);
	return 0;

}



