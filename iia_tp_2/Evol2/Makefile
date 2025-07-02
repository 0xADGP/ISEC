CC = gcc
CFLAGS = -Wall -g
OBJS = main.o util.o algoritmo.o funcao.o

all: programa

programa: $(OBJS)
	$(CC) $(OBJS) -o programa

main.o: main.c util.h algoritmo.h funcao.h
	$(CC) $(CFLAGS) -c main.c

util.o: util.c util.h
	$(CC) $(CFLAGS) -c util.c

algoritmo.o: algoritmo.c algoritmo.h
	$(CC) $(CFLAGS) -c algoritmo.c

funcao.o: funcao.c funcao.h
	$(CC) $(CFLAGS) -c funcao.c

clean:
	rm -f programa $(OBJS)
