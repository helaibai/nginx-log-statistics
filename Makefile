OBJ=log.o store.o debug.o hash.o strnchar.o
CC=gcc
#CFLAGS= -DDEBUG
all: log
log: $(OBJ)
	$(CC) $^ -o $@ -lpthread
clean:
	$(RM) *.o log
