/*
 * Program: main
 * Model  : C driver for x86_64 ASM routines
 * Goal   : run basic assembly hello flow
 */

#include <stdio.h>

extern void _hello(void);

int main(void) {
    _hello();
    return 0;
}
