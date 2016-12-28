CC=gcc
CFLAGS=-Wall
all: ecrivain.o ecrivain affichage.o affichage agence.o agence utilisateur.o utilisateur 

affichage.o: affichage.c
	$(CC) -c affichage.c -o affichage.o $(CFLAGS)

affichage: affichage.o
	$(CC) affichage.o -o affichage $(CFLAGS)
	
utilisateur.o: utilisateur.c
	$(CC) -c utilisateur.c -o utilisateur.o $(CFLAGS)

utilisateur: utilisateur.o
	$(CC) utilisateur.o -o utilisateur $(CFLAGS)
	
agence.o: agence.c
	$(CC) -c agence.c -o agence.o $(CFLAGS)
	
agence: agence.o
	$(CC) agence.o -o agence $(CFLAGS)


ecrivain.o: ecrivain.c
	$(CC) -c ecrivain.c -o ecrivain.o $(CFLAGS)

ecrivain: ecrivain.o
	$(CC) ecrivain.o -o ecrivain $(CFLAGS)


tests: TESTS
	make -C TESTS

clean:
	rm -rf utilisateur agence ecrivain affichage
	rm -rf *.o 
	make clean -C TESTS
