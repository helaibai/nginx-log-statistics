OBJ=log.o store.o debug.o hash.o strnchar.o main.o
CC=gcc
CFLAGS= -Wall
all: log
log: $(OBJ)
	$(CC) $^ -o $@ -lpthread
clean:
	$(RM) *.o log
