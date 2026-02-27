TARGET = program

# Object files
OBJS = main.o hello.o

# Assembler and compiler
AS = nasm
CC = gcc

# Assembler and compiler flags
ASFLAGS = -f elf64
CFLAGS = -g

# Default 
all: $(TARGET)

# Linking rule
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compilation rule for C files
main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

# Assembling rule for assembly files
hello.o: hello.asm
	$(AS) $(ASFLAGS) -o hello.o hello.asm

clean:
	rm -f $(TARGET) $(OBJS)

# Phony targets
.PHONY: all clean
