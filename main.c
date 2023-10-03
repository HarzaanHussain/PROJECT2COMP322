#include <stdio.h>
#include <stdlib.h>

#define SIZE 5 // Assume the size of PCB array is 5

typedef struct Node {
    int index;
    struct Node* next;
} Node;

typedef struct PCB {
    int parent;
    Node* children;
    char state;
} PCB;

PCB pcb[SIZE]; // Array of PCB

void insert(Node** head, int index) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->index = index;
    new_node->next = *head;
    *head = new_node;
}

void delete(Node** head, int index) {
    Node* temp = *head, *prev;

    if (temp != NULL && temp->index == index) {
        *head = temp->next;
        free(temp);
        return;
    }

    while (temp != NULL && temp->index != index) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) return;

    prev->next = temp->next;
    free(temp);
}

void create(int p, int parent) {
    pcb[p].parent = parent;
    pcb[p].children = NULL;
    pcb[p].state = 'R'; // Assume the ready state is 'R'

    // Insert the new process to the children list of the parent
    insert(&pcb[parent].children, p);

    printf("Created process: %d (parent: %d)\n", p, parent);
}

void destroy(int p) {
    // Store the list of child processes in the children array
    Node* temp = pcb[p].children;
    while (temp != NULL) {
        // Recursively destroy all child processes
        destroy(temp->index);
        temp = temp->next;
    }

    // Remove the process from the children list of the parent
    if (pcb[p].parent != p) { // Avoid deleting self from its own children list
        delete(&pcb[pcb[p].parent].children, p);
    }

    printf("Destroyed process: %d\n", p);
}

int main() {
    create(0, 0); // Create parent process
    create(1, 0); // Creates 1st child of PCB[0] at PCB[1]
    create(2, 0); // Creates 2nd child of PCB[0] at PCB[2]
    create(3, 2); // Creates 1st child of PCB[2] at PCB[3]
    create(4, 0); // Creates 3rd child of PCB[0] at PCB[4]
    destroy(0); // Destroys all descendant of PCB[0], which includes processes PCB[1] through PCB[4]

    return 0;
}