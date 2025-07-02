# Makefile for the hill climbing algorithm project

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lm

# Source files
SRCS = main.c funcao.c algoritmo.c util.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
EXEC = hill_climbing

# Default target
all: $(EXEC)

# Link object files to create the executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJS) $(EXEC)

# Phony targets
.PHONY: all clean