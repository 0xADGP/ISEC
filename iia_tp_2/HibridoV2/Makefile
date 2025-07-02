CC = gcc
CFLAGS = -Wall -g
OBJS = main.o hillclimbing.o util.o algoritmo.o funcao.o

all: programa

programa: $(OBJS)
	$(CC) $(OBJS) -o programa

util.o: util.c util.h
	$(CC) $(CFLAGS) -c util.c

algoritmo.o: algoritmo.c algoritmo.h
	$(CC) $(CFLAGS) -c algoritmo.c

funcao.o: funcao.c funcao.h
	$(CC) $(CFLAGS) -c funcao.c

hillclimbing.o: hillclimbing.c hillclimbing.h
	$(CC) $(CFLAGS) -c hillclimbing.c

main.o: main.c hillclimbing.h util.h algoritmo.h funcao.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f programa $(OBJS)
