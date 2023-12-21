# Name of the output binary
TARGET = tcp_syn_monitor

# Source Files
SOURCES = src/main.c src/tcp_syn_monitor.c src/circular_buffer.c src/packet_handler.c

# Object Files
OBJECTS = $(SOURCES:.c=.o)

# Assembly Files
ASSEMBLYS = $(SOURCES:.c=.s)

# Compiler
CC = gcc

# Compiler Flags
CFLAGS = -Wall -O3 -Iinclude

# Linker Flags
LDFLAGS = -lpcap

# Default target
all: $(ASSEMBLYS) $(OBJECTS) $(TARGET)

# Generate assembly files
%.s: %.c
	$(CC) $(CFLAGS) -S $< -o $@

# Generate object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Generate the binary
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Only leave the binary
bo: binonly
binonly:
	rm -f $(ASSEMBLYS)
	rm -f $(OBJECTS)

# Return to a state before any build
clean:
	rm -f $(ASSEMBLYS)
	rm -f $(OBJECTS)
	rm -f $(TARGET)

