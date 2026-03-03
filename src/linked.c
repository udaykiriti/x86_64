#include "linked.h"
#include <stdlib.h>

Node* create(int data) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node) {
        node->data = data;
        node->next = NULL;
    }
    return node;
}

void push(Node** head, int data) {
    Node* new_node = create(data);
    if (new_node) {
        new_node->next = *head;
        *head = new_node;
    }
}

void append(Node** head, int data) {
    Node* new_node = create(data);
    if (!new_node) return;
    
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    
    Node* current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
}

int del(Node** head, int data) {
    if (*head == NULL) return 0;
    
    if ((*head)->data == data) {
        Node* temp = *head;
        *head = (*head)->next;
        free(temp);
        return 1;
    }
    
    Node* current = *head;
    while (current->next != NULL) {
        if (current->next->data == data) {
            Node* temp = current->next;
            current->next = temp->next;
            free(temp);
            return 1;
        }
        current = current->next;
    }
    return 0;
}

Node* find(Node* head, int data) {
    Node* current = head;
    while (current != NULL) {
        if (current->data == data) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int len(Node* head) {
    int count = 0;
    Node* current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

void show(Node* head) {
    Node* current = head;
    while (current != NULL) {
        if (current->next != NULL) {
            printf("%d -> ", current->data);
        } else {
            printf("%d", current->data);
        }
        current = current->next;
    }
    printf("\n");
}

void purge(Node** head) {
    Node* current = *head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}
