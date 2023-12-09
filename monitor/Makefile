# Name of the output binary
TARGET = tcp_syn_monitor

# Source Files
SOURCES = main.c tcp_syn_monitor.c circular_buffer.c packet_handler.c

# Assembly files
ASSEMBLYS = $(SOURCES:.c=.s)

# Object Files
OBJECTS = $(SOURCES:.c=.o)

# Compiler
CC = gcc-12

# Compiler Flags
CFLAGS = -Wall -O3

# Linker Flags
LDFLAGS = -lpcap

all: $(ASSEMBLYS) $(OBJECTS) $(TARGET)

# Generate assembly files
%.s: %.c
	$(CC) $(CFLAGS) -S $< -o $@

# Generate object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Generate the binary
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

# Only leave the binary
bo:
	rm -f $(ASSEMBLYS)
	rm -f $(OBJECTS)
binonly:
	rm -f $(ASSEMBLYS)
	rm -f $(OBJECTS)

# Return to a state before any build
clean:
	rm -f $(ASSEMBLYS)
	rm -f $(OBJECTS)
	rm -f $(TARGET)
