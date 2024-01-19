SOURCES = src/host.c src/auth.c src/local.c src/client.c
OBJECTS = $(SOURCES:.c=.o)
CC = gcc
CFLAGS = -Wall -O3 -Iinclude

all: $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)
