TARGET = program
MMAP_APP = map
.DEFAULT_GOAL := all

# Layout
SRCDIR = src
APPDIR = $(SRCDIR)/app
ASMDIR = $(SRCDIR)/asm
LABDIR = $(SRCDIR)/labs
INCDIR = include
OBJDIR = obj

# Sources
C_SRCS = $(wildcard $(APPDIR)/*.c)
ASM_SRCS = $(wildcard $(ASMDIR)/*.asm)
MAP_SRC = $(LABDIR)/map.c

# Objects
C_OBJS = $(patsubst $(APPDIR)/%.c,$(OBJDIR)/%.o,$(C_SRCS))
ASM_OBJS = $(patsubst $(ASMDIR)/%.asm,$(OBJDIR)/%.o,$(ASM_SRCS))
MAP_OBJ = $(OBJDIR)/map.o
OBJS = $(C_OBJS) $(ASM_OBJS)

# Tools
CC = gcc
AS = nasm

# Flags
CPPFLAGS = -I $(INCDIR)/
CFLAGS = -O2 -Wall -Wextra -Wpedantic
ASFLAGS = -f elf64 -I $(INCDIR)/
DEPFLAGS = -MMD -MP
DEPS = $(C_OBJS:.o=.d) $(MAP_OBJ:.o=.d)

all: $(TARGET)

# Main program
$(TARGET): $(OBJS)
	$(CC) -o $@ $^

$(MMAP_APP): $(MAP_OBJ)
	$(CC) -o $@ $<

# C build rule
$(OBJDIR)/%.o: $(APPDIR)/%.c | $(OBJDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) -c -o $@ $<

# ASM library build rule
$(OBJDIR)/%.o: $(ASMDIR)/%.asm | $(OBJDIR)
	$(AS) $(ASFLAGS) -o $@ $<

# Lab build rule
$(MAP_OBJ): $(MAP_SRC) | $(OBJDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) -c -o $@ $<

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(TARGET) $(MMAP_APP)

.PHONY: all clean $(MMAP_APP)

-include $(DEPS)
