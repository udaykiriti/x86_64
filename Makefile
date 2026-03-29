TARGET = bin/program
.DEFAULT_GOAL := all

# Layout
SRCDIR = src
APPDIR = $(SRCDIR)/app
ASMDIR = $(SRCDIR)/asm
LABDIR = $(SRCDIR)/labs
INCDIR_C = include/c
INCDIR_ASM = include/asm
OBJDIR = obj
BINDIR = bin

# Sources
C_SRCS = $(wildcard $(APPDIR)/*.c)
ASM_SRCS = $(wildcard $(ASMDIR)/*.asm)
MAP_SRC = $(LABDIR)/mmap/map.c

# Objects
C_OBJS = $(patsubst $(APPDIR)/%.c,$(OBJDIR)/%.o,$(C_SRCS))
ASM_OBJS = $(patsubst $(ASMDIR)/%.asm,$(OBJDIR)/%.o,$(ASM_SRCS))
MAP_OBJ = $(OBJDIR)/map.o
OBJS = $(C_OBJS) $(ASM_OBJS) $(MAP_OBJ)

# Tools
CC = gcc
AS = nasm

# Flags
CPPFLAGS = -I $(INCDIR_C)/
CFLAGS = -O2 -Wall -Wextra -Wpedantic
ASFLAGS = -f elf64 -I $(INCDIR_ASM)/
DEPFLAGS = -MMD -MP
DEPS = $(C_OBJS:.o=.d) $(MAP_OBJ:.o=.d)

all: $(TARGET)

# Main program
$(TARGET): $(OBJS) | $(BINDIR)
	$(CC) -o $@ $^

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

$(BINDIR):
	mkdir -p $(BINDIR)

run: $(TARGET)
	./$(TARGET)

help:
	@echo "Targets:"
	@echo "  make       - build the program"
	@echo "  make run   - build and run the program"
	@echo "  make test  - run smoke checks across all modes"
	@echo "  make clean - remove build artifacts"
	@echo "  make help  - show this message"

test: $(TARGET)
	@set -e; \
	for mode in hello anon buf file cow shared simd bitwise sysinfo all; do \
		echo "==> smoke: $$mode"; \
		./$(TARGET) $$mode; \
	done

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all run help test clean

-include $(DEPS)
