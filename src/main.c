/*
 * Main entry point for the application.
 * Calls into assembly routines defined in src/hello.asm.
 */

#include <stdio.h>
#include "math.h"
#include "fastmath.h"
#include "string.h"

extern void _hello(void);

int main(void) {
    _hello();

    int a = 10 ,  b = 5;
    
    printf("C Math: %d + %d = %d\n", a, b, add(a, b));
    printf("C Math: %d * %d = %d\n", a, b, multiply(a, b));

    // Factorial demo
    printf("C Math: factorial(5) = %d\n", factorial(5));

    // Math Assembly
    printf("ASM Math: %d + %d = %d\n", a, b, asm_add(a, b));
    printf("ASM Math: %d - %d = %d\n", a, b, asm_sub(a, b));
    printf("ASM Math: factorial(5) = %d\n", asm_factorial(5));

    // String demo
    char text[] = "GitHub";
    printf("String: '%s' (len: %d)\n", text, str_len(text));
    
    str_rev(text);
    printf("Reversed: '%s'\n", text);

    return 0;
}
