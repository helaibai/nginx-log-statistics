OBJ=log.o store.c
CC=gcc
#CFLAGS= -DDEBUG
all: log
log: $(OBJ)
	$(CC) $^ -o $@ -lpthread
clean:
	$(RM) *.o log
