SOURCES = src/host.c src/auth.c src/local.c src/client.c
CC = gcc
CFLAGS = -Wall -O3 -Iinclude
LIBS = -lssl -lcrypto -lpcap

APPS = host auth local client bot test monitor

all: $(APPS)

host: src/host.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

auth: src/auth.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

local: src/local.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

client: src/client.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

bot: src/bot.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

test: src/test.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

monitor: src/monitor/main.c src/monitor/circular_buffer.c src/monitor/packet_handler.c src/monitor/tcp_syn_monitor.c src/monitor/monitor_log.c src/monitor/detect_ddos.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)


clean:
	rm -f $(APPS)