CC = gcc
CFLAGS = -Wall -g
TARGETS = feed manager

.PHONY: all clean

all: clean $(TARGETS)

feed: feed.c feed.h
	$(CC) $(CFLAGS) -o feed feed.c

manager: manager.c manager.h
	$(CC) $(CFLAGS) -o manager manager.c

clean:
	rm -f $(TARGETS)
