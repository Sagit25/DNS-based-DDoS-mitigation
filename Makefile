SOURCES = src/host.c src/auth.c src/local.c src/client.c
# OBJECTS = $(SOURCES:.c=.o)
CC = gcc
CFLAGS = -Wall -Iinclude

# all: $(OBJECTS)

# %.o: %.c
# $(CC) $(CFLAGS) -c $< -o $@

# clean:
# rm -f $(OBJECTS)

all: client host auth local

host: src/host.c
	$(CC) -o $@ $< $(CFLAGS)

auth: src/auth.c
	$(CC) -o $@ $< $(CFLAGS)

local: src/local.c
	$(CC) -o $@ $< $(CFLAGS)

client: src/client.c
	$(CC) -o $@ $< $(CFLAGS)

clean:
	rm -f client host auth local
