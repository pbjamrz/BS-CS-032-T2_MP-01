#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int exp_x, exp_y, exp_z;
    int coeff;
    struct Node* next;
} Node;

typedef struct Polynomial {
    int id_num; // Identifier of the polynomial
    Node* head;
} Polynomial;

Node* createNode() {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->exp_x = 0;
    newNode->exp_y = 0;
    newNode->exp_z = 0;
    newNode->coeff = 0;
    newNode->next = NULL;
    return newNode;
}

void zeropoly(Polynomial* P, int id) {
    Node* t = createNode();
    P->head = t;
    P->id_num = id;
    t->exp_x = 0;
    t->exp_y = 0;
    t->exp_z = 1;
    t->coeff = -1;
    t->next = t; // Circular link
}

void polyread(Polynomial* P, int id, int n) {
    zeropoly(P, id);
    
    for (int i = 0; i < n; i++) {
        int ex, ey, ez, coef;
        scanf("%d %d %d %d", &ex, &ey, &ez, &coef);
        
        Node* v = createNode();
        v->exp_x = ex;
        v->exp_y = ey;
        v->exp_z = ez;
        v->coeff = coef;
        
        Node* b = P->head;
        Node* a = P->head->next;
        

        //Ensures linked list is in canonical form
        while (a != P->head && (a->exp_x > v->exp_x || 
                                    (a->exp_x == v->exp_x && a->exp_y > v->exp_y) || 
                                    (a->exp_x == v->exp_x && a->exp_y == v->exp_y && a->exp_z > v->exp_z))) {
            b = a;
            a = a->next;
        }
        
        b->next = v;
        v->next = a;
    }
}
