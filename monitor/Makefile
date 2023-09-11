# Name of the output binary
TARGET = tcp_syn_capture

# Source Files
SOURCES = packet_capture.c

# Compiler
CC = gcc

# Compiler Flags
CFLAGS = -Wall -O2

# Linker Flags
LDFLAGS = -lpcap

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
