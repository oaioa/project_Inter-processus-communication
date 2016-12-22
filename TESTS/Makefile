CC=gcc
CFLAGS=-Wall
//EXE = shared_creation.o shared_creation
all: shared_creation.o shared_creation shared_use.o shared_use queue_in.o queue_in queue_out.o queue_out

shared_creation.o: shared_creation.c
	$(CC) -c shared_creation.c -o shared_creation.o $(CFLAGS)

shared_creation: shared_creation.o
	$(CC) shared_creation.o -o shared_creation $(CFLAGS)
	
shared_use.o: shared_use.c
	$(CC) -c shared_use.c -o shared_use.o $(CFLAGS)
	
shared_use: shared_use.o
	$(CC) shared_use.o -o shared_use $(CFLAGS)

queue_in.o: queue_in.c
	$(CC) -c queue_in.c -o queue_in.o $(CFLAGS)

queue_in: queue_in.o
	$(CC) queue_in.o -o queue_in $(CFLAGS)
	
queue_out.o: queue_out.c
	$(CC) -c queue_out.c -o queue_out.o $(CFLAGS)
	
queue_out: queue_out.o
	$(CC) queue_out.o -o queue_out $(CFLAGS)

clean:
	rm -rf queue_out queue_in shared_use shared_creation
	rm -rf *.o
