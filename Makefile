CC=gcc
FLAGS= -pthread -o

all: deadlock neverdeadlock starvation neverstarvation livelock neverlivelock
		./2.1.1
		./2.1.2
		./2.2.1
		./2.2.2
		./2.3.1
		./2.3.2

deadlock:2.1.1.c
		$(CC) $(FLAGS) 2.1.1 2.1.1.c

neverdeadlock:2.1.2.c
		$(CC) $(FLAGS) 2.1.2 2.1.2.c

starvation:2.2.1.c
		$(CC) $(FLAGS) 2.2.1 2.2.1.c

neverstarvation:2.2.2.c
		$(CC) $(FLAGS) 2.2.2 2.2.2.c

livelock:2.3.1.c
		$(CC) $(FLAGS) 2.3.1 2.3.1.c

neverlivelock:2.3.2.c
		$(CC) $(FLAGS) 2.3.2 2.3.2.c

clean:
	rm -f 2.1.1 2.1.2 2.2.1 2.2.2 2.3.1 2.3.2

.PHONY:clean all