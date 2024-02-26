SOURCES = src/host.c src/auth.c src/local.c src/client.c
CC = gcc
CFLAGS = -Wall -Iinclude 
LDFLAGS = -lssl -lcrypto

all: host auth local client bot test

host: src/host.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

auth: src/auth.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

local: src/local.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

client: src/client.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

bot: src/bot.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

test: src/test.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

clean:
	rm -f host auth local client bot test