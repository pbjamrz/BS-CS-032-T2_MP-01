#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int exp_x;
    int exp_y;
    int exp_z;
    int coeff;
    struct node* next;

} Node;

typedef struct polynomial {
    int id_num; // the identifier of the polynomial
    Node* head;

} Polynomial;

Node* createNode() {
    Node* new_node=(Node*)malloc(sizeof(Node));

    new_node->exp_x = 0;
    new_node->exp_y = 0;
    new_node->exp_z = 0;
    new_node->coeff = 0;
    new_node->next = NULL;

    return new_node;
}
