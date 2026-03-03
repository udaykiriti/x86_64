#include "linked.h"
#include <stdlib.h>
#include <assert.h>

Node* create(int data) {
    Node* node = malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }
    node->data = data;
    node->next = NULL;
    return node;
}

void push(Node** head, int data) {
    assert(head != NULL);
    Node* new_node = create(data);
    if (new_node == NULL) {
        return;
    }
    new_node->next = *head;
    *head = new_node;
}

void append(Node** head, int data) {
    assert(head != NULL);
    Node* new_node = create(data);
    if (new_node == NULL) {
        return;
    }
    
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
    assert(head != NULL);
    
    if (*head == NULL) {
        return 0;
    }
    
    if ((*head)->data == data) {
        Node* temp = *head;
        *head = (*head)->next;
        free(temp);
        return 1;
    }
    
    for (Node* current = *head; current->next != NULL; current = current->next) {
        if (current->next->data == data) {
            Node* temp = current->next;
            current->next = temp->next;
            free(temp);
            return 1;
        }
    }
    return 0;
}

Node* find(Node* head, int data) {
    for (Node* current = head; current != NULL; current = current->next) {
        if (current->data == data) {
            return current;
        }
    }
    return NULL;
}

int len(Node* head) {
    int count = 0;
    for (Node* current = head; current != NULL; current = current->next) {
        count++;
    }
    return count;
}

void show(Node* head) {
    for (Node* current = head; current != NULL; current = current->next) {
        if (current->next != NULL) {
            printf("%d -> ", current->data);
        } else {
            printf("%d", current->data);
        }
    }
    printf("\n");
}

void purge(Node** head) {
    assert(head != NULL);
    
    Node* current = *head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}
