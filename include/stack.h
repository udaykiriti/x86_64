#ifndef STACK_H
#define STACK_H

typedef struct SNode {
    int val;
    struct SNode* next;
} SNode;

typedef struct {
    SNode* top;
} Stack;

Stack* s_new(void);
void s_push(Stack* s, int val);
int s_pop(Stack* s);
int s_top(Stack* s);
int s_size(Stack* s);
void s_show(Stack* s);
void s_free(Stack* s);

#endif
