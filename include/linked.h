#ifndef LINKED_H
#define LINKED_H

#include <stdio.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* create(int data);
void push(Node** head, int data);
void append(Node** head, int data);
int del(Node** head, int data);
Node* find(Node* head, int data);
int len(Node* head);
int asm_len(Node* head);
void show(Node* head);
void purge(Node** head);

#endif
