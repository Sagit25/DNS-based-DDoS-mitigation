SOURCES = src/host.c src/auth.c src/local.c src/client.c
CC = gcc
CFLAGS = -Wall -Iinclude -lssl -lcrypto

all: host auth local client

host: src/host.c
	$(CC) -o $@ $< $(CFLAGS)

auth: src/auth.c
	$(CC) -o $@ $< $(CFLAGS)

local: src/local.c
	$(CC) -o $@ $< $(CFLAGS)

client: src/client.c
	$(CC) -o $@ $< $(CFLAGS)

monitor: src/monitor.c
	$(CC) -o $@ $< $(CFLAGS)

test: src/test.c
	$(CC) -o $@ $< $(CFLAGS)

clean:
	rm -f host auth local client