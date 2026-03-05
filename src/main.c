/*
 * Main entry point for the application.
 * Demonstrates linked list, stack operations, and SIMD operations.
 */

#include <stdio.h>
#include <stdint.h>
#include "linked.h"
#include "stack.h"
#include "simd.h"

extern void _hello(void);
extern int asm_size(Stack* s);

int main(void) {
    _hello();

    Node* list = NULL;
    
    printf(" Linked List \n\n");
    
    printf("Append: 10, 20, 30, 40, 50\n");
    append(&list, 10);
    append(&list, 20);
    append(&list, 30);
    append(&list, 40);
    append(&list, 50);
    printf("List: ");
    show(list);
    printf("C len(): %d\n", len(list));
    printf("ASM len(): %d\n\n", asm_len(list));
    
    printf("Push: 5\n");
    push(&list, 5);
    printf("List: ");
    show(list);
    printf("C len(): %d\n", len(list));
    printf("ASM len(): %d\n\n", asm_len(list));
    
    printf("Find 30: %s\n", find(list, 30) ? "Found" : "Not Found");
    printf("Find 100: %s\n\n", find(list, 100) ? "Found" : "Not Found");
    
    printf("Remove 30\n");
    del(&list, 30);
    printf("List: ");
    show(list);
    printf("C len(): %d\n", len(list));
    printf("ASM len(): %d\n\n", asm_len(list));
    
    printf("Remove 5\n");
    del(&list, 5);
    printf("List: ");
    show(list);
    printf("C len(): %d\n", len(list));
    printf("ASM len(): %d\n\n", asm_len(list));

    printf("[=== SIMD Demo ===]\n\n");
    
    int v1[4] = {1, 2, 3, 4};
    int v2[4] = {10, 20, 30, 40};
    int vo[4] = {0, 0, 0, 0};

    asm_vec4_add_i32(v1, v2, vo);
    printf("SIMD add: [%d, %d, %d, %d]\n", vo[0], vo[1], vo[2], vo[3]);

    uint64_t t0 = asm_rdtsc_serialized();
    for (int i = 0; i < 1000000; i++) {
        asm_vec4_add_i32(v1, v2, vo);
    }
    uint64_t t1 = asm_rdtsc_serialized();
    printf("SIMD loop cycles (1,000,000 iters): %llu\n\n",
           (unsigned long long)(t1 - t0));

    purge(&list);

    printf("\n[=== Stack Demo ===]\n\n");
    
    Stack* st = s_new();
    
    printf("push: 10, 20, 30\n");
    s_push(st, 10);
    s_push(st, 20);
    s_push(st, 30);
    s_show(st);
    printf("size (C): %d\n", s_size(st));
    printf("size (ASM): %d\n", asm_size(st));
    printf("top: %d\n\n", s_top(st));
    
    printf("pop: %d\n", s_pop(st));
    printf("pop: %d\n\n", s_pop(st));
    s_show(st);
    printf("size (C): %d\n", s_size(st));
    printf("size (ASM): %d\n", asm_size(st));
    
    s_free(st);

    return 0;
}
