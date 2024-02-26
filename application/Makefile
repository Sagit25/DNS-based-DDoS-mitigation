SOURCES = src/host.c src/auth.c src/local.c src/client.c
CC = gcc
CFLAGS = -Wall -O3 -Iinclude
LDFLAGS = -lssl -lcrypto -lpcap

all: host.o auth.o local.o client.o bot.o test.o monitor.o

host.o: src/host.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

auth.o: src/auth.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

local.o: src/local.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

client.o: src/client.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

bot.o: src/bot.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

test.o: src/test.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

monitor.o: mon.o circ.o packet.o syn.o log.o ddos.o
	$(CC) -o $@ mon.o circ.o packet.o syn.o log.o ddos.o $(CFLAGS) $(LDFLAGS)
	rm -f mon.o circ.o packet.o syn.o log.o ddos.o

mon.o: src/monitor.c 
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)
circ.o: src/monitor/circular_buffer.c
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)
packet.o: src/monitor/packet_handler.c
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)
syn.o: src/monitor/tcp_syn_monitor.c
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)
log.o: src/monitor/monitor_log.c
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)
ddos.o: src/monitor/detect_ddos.c
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o