CC=gcc
CFLAGS=-Wall
//EXE = utilisateur.o utilisateur
all: utilisateur.o utilisateur agence.o agence

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
	$(CC) $(CC) -c ecrivain.c -o ecrivain.o $(CFLAGS).o 


tests: TESTS
	make -C TESTS

clean:
	rm -rf utilisateur agence
	rm -rf *.o 
	make clean -C TESTS
