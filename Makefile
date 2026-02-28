TARGET = program

# Dirs
SRCDIR  = src
INCDIR  = include
OBJDIR  = obj

# Src files
C_SRCS   = $(wildcard $(SRCDIR)/*.c)
ASM_SRCS = $(wildcard $(SRCDIR)/*.asm)

# Derived obj files
C_OBJS   = $(patsubst $(SRCDIR)/%.c,   $(OBJDIR)/%.o, $(C_SRCS))
ASM_OBJS = $(patsubst $(SRCDIR)/%.asm, $(OBJDIR)/%.o, $(ASM_SRCS))
OBJS     = $(C_OBJS) $(ASM_OBJS)

# Tools
CC      = gcc
AS      = nasm

# Flags
CFLAGS  = -Wall -O2 -I $(INCDIR)/
ASFLAGS = -f elf64 -I $(INCDIR)/

#  Targets 
all: $(TARGET)

# Link (gcc handles CRT startup for C main)
$(TARGET): $(OBJS)
	$(CC) -o $@ $^

# Compile each .c -> obj/
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Assemble each .asm ->  obj/
$(OBJDIR)/%.o: $(SRCDIR)/%.asm | $(OBJDIR)
	$(AS) $(ASFLAGS) -o $@ $<

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: all clean
