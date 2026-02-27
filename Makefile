%: %.asm
	nasm -f elf64 $< -o $@.o
	ld $@.o -o $@

run: 
	./$(filter-out $@,$(MAKECMDGOALS))

# Prevent make from thinking filename is a real target
%:
	@:
