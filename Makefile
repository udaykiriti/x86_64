TARGET = program

# Directories
SRCDIR  = src
INCDIR  = include
OBJDIR  = obj

# Source files
ASM_SRCS = $(wildcard $(SRCDIR)/*.asm)

# Derived object files
OBJS = $(patsubst $(SRCDIR)/%.asm, $(OBJDIR)/%.o, $(ASM_SRCS))

# Tools
AS      = nasm
LD      = ld

# Flags
ASFLAGS = -f elf64 -I $(INCDIR)/
LDFLAGS =

#  Targets 

all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

# Assemble each .asm ->  obj/
$(OBJDIR)/%.o: $(SRCDIR)/%.asm | $(OBJDIR)
	$(AS) $(ASFLAGS) -o $@ $<

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: all clean
