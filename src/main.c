/*
 * Main entry point for the application.
 * Calls into assembly routines defined in src/hello.asm.
 */

#include <stdio.h>
#include "math_utils.h"

extern void _hello(void);

int main(void) {
    _hello();

    int a = 10;
    int b = 5;
    
    int sum = add(a, b);
    int prod = multiply(a, b);

    printf("Math in C: %d + %d = %d\n", a, b, sum);
    printf("Math in C: %d * %d = %d\n", a, b, prod);

    return 0;
}
