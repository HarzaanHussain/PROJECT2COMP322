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
    Node* temp = *head, *prev = NULL;

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

    // Insert the new process into the children list of the parent
    insert(&pcb[parent].children, p);

    printf("Created process: %d (parent: %d)\n", p, parent);
}

void destroy(int p) {
    // Recursively destroy all child processes and their descendants
    Node* temp = pcb[p].children;
    while (temp != NULL) {
        destroy(temp->index);
        temp = temp->next;
    }

    // Remove this process from its parent's child list
    if (pcb[p].parent != p) {
        Node* parentChildren = pcb[pcb[p].parent].children;
        if (parentChildren != NULL && parentChildren->index == p) {
            Node* temp = parentChildren->next;
            free(parentChildren);
            pcb[pcb[p].parent].children = temp;
        } else {
            Node* temp = parentChildren;
            Node* prev = NULL;

            while (temp != NULL && temp->index != p) {
                prev = temp;
                temp = temp->next;
            }

            if (temp != NULL) {
                prev->next = temp->next;
                free(temp);
            }
        }
    }

    // Free memory allocated for this process's child list
    pcb[p].children = NULL;

    // Set parent to -1 to denote deletion
    pcb[p].parent = -1;

    printf("Destroyed process: %d\n", p);
}


int main() {
    // Create parent process
    create(0, 0);

    // Create 1st child of PCB[0] at PCB[1]
    create(1, 0);

    // Create 2nd child of PCB[0] at PCB[2]
    create(2, 0);

    // Create 1st child of PCB[2] at PCB[3]
    create(3, 2);

    // Create 3rd child of PCB[0] at PCB[4]
    create(4, 0);

    // Destroy parent process
    destroy(0);

    return 0;
}
