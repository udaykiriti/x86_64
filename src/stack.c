#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

// C functions
Stack* s_new(void) {
    Stack* s = (Stack*)malloc(sizeof(Stack));
    s->top = NULL;
    return s;
}

void s_push(Stack* s, int val) {
    SNode* n = (SNode*)malloc(sizeof(SNode));
    n->val = val;
    n->next = s->top;
    s->top = n;
}

int s_pop(Stack* s) {
    if (!s->top) return -1;
    SNode* temp = s->top;
    int val = temp->val;
    s->top = temp->next;
    free(temp);
    return val;
}

int s_top(Stack* s) {
    return s->top ? s->top->val : -1;
}

int s_size(Stack* s) {
    int cnt = 0;
    SNode* n = s->top;
    while (n) {
        cnt++;
        n = n->next;
    }
    return cnt;
}

void s_show(Stack* s) {
    printf("Stack: ");
    SNode* n = s->top;
    while (n) {
        printf("%d ", n->val);
        n = n->next;
    }
    printf("\n");
}

void s_free(Stack* s) {
    while (s->top) s_pop(s);
    free(s);
}

// ASM function declaration
extern int asm_size(Stack* s);
