CC=gcc
CFLAGS=-Wall
all:  lastminute  ecrivain affichage agence utilisateur 

affichage: affichage.c
	$(CC) affichage.c -o affichage $(CFLAGS)
	
utilisateur: utilisateur.c
	$(CC) utilisateur.c -o utilisateur $(CFLAGS)
	
agence: agence.c
	$(CC) agence.c -o agence $(CFLAGS)

ecrivain: ecrivain.c
	$(CC) ecrivain.c -o ecrivain $(CFLAGS)

test: lastminute.c
	$(CC) lastminute.c -o lastminute $(CFLAGS)
	

clean:
	rm -rf utilisateur agence ecrivain lastminute affichage
	rm -rf *.o 
