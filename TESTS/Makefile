CC=gcc
CFLAGS=-Wall
all: sem_creation.o sem_creation sem_use.o sem_use mutex_creation.o mutex_creation mutex_use.o mutex_use family sh_c.o sh_c sh_u.o sh_u queue_in.o queue_in queue_out.o queue_out pipe pipe.o

sem_creation.o: sem_creation.c
	$(CC) -c sem_creation.c -o sem_creation.o $(CFLAGS)
sem_creation: sem_creation.o
	$(CC) sem_creation.o -o sem_creation $(CFLAGS)

sem_use.o: sem_use.c
	$(CC) -c sem_use.c -o sem_use.o $(CFLAGS)
sem_use: sem_use.o
	$(CC) sem_use.o -o sem_use $(CFLAGS)



mutex_creation.o: mutex_creation.c
	$(CC) -c mutex_creation.c -o mutex_creation.o $(CFLAGS)
mutex_creation: mutex_creation.o
	$(CC) mutex_creation.o -o mutex_creation $(CFLAGS)

mutex_use.o: mutex_use.c
	$(CC) -c mutex_use.c -o mutex_use.o $(CFLAGS)
mutex_use: mutex_use.o
	$(CC) mutex_use.o -o mutex_use $(CFLAGS)


sh_c.o: shared_creation.c
	$(CC) -c shared_creation.c -o shared_creation.o $(CFLAGS)
sh_c: shared_creation.o
	$(CC) shared_creation.o -o shared_creation $(CFLAGS)

sh_u.o: shared_use.c
	$(CC) -c shared_use.c -o shared_use.o $(CFLAGS)	
sh_u: shared_use.o
	$(CC) shared_use.o -o shared_use $(CFLAGS)


queue_in.o: queue_in.c
	$(CC) -c queue_in.c -o queue_in.o $(CFLAGS)
queue_in: queue_in.o
	$(CC) queue_in.o -o queue_in $(CFLAGS)
	
queue_out.o: queue_out.c
	$(CC) -c queue_out.c -o queue_out.o $(CFLAGS)	
queue_out: queue_out.o
	$(CC) queue_out.o -o queue_out $(CFLAGS)

pipe.o: pipe.c
	$(CC) -c pipe.c -o pipe.o $(CFLAGS)	
pipe: pipe.o
	$(CC) pipe.o -o pipe $(CFLAGS)

family.o: family.c
	$(CC) -c family.c -o family.o $(CFLAGS)	
family: family.o
	$(CC) family.o -o family $(CFLAGS)


clean:
	rm -rf family sem_use sem_creation pipe queue_out queue_in shared_use shared_creation mutex_creation mutex_use
	rm -rf *.o
