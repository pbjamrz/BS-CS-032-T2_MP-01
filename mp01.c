#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int exp_x, exp_y, exp_z;
    int coeff;
    struct node* next;
} Node;

typedef struct polynomial {
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

void zeropoly(Polynomial* p, int id) {
    Node* t = createNode();
    p->head = t;
    p->id_num = id;
    t->exp_x = 0;
    t->exp_y = 0;
    t->exp_z = 1;
    t->coeff = -1;
    t->next = t; // Circular link
}

void polyread(Polynomial* p, int id, int n) {
    zeropoly(p, id);
    
    for (int i = 0; i < n; i++) {
        int ex, ey, ez, coef;
        scanf("%d %d %d %d", &ex, &ey, &ez, &coef);
        
        Node* v = createNode();
        v->exp_x = ex;
        v->exp_y = ey;
        v->exp_z = ez;
        v->coeff = coef;
        
        Node* b = p->head;
        Node* a = p->head->next;
        

        //Ensures linked list is in canonical form
        while (a != p->head && (a->exp_x > v->exp_x || 
                                    (a->exp_x == v->exp_x && a->exp_y > v->exp_y) || 
                                    (a->exp_x == v->exp_x && a->exp_y == v->exp_y && a->exp_z > v->exp_z))) {
            b = a;
            a = a->next;
        }
        
        b->next = v;
        v->next = a;
    }

}

void polyadd(Polynomial *p, Polynomial *q, int id) {
    Node *a = p->head->next;
    Node *b = q->head->next;
    Node *prev = q->head;

    while (a != p->head) {
        while (b != q->head && (b->exp_x > a->exp_x ||
                                (b->exp_x == a->exp_x && b->exp_y > a->exp_y) ||
                                (b->exp_x == a->exp_x && b->exp_y == a->exp_y && b->exp_z > a->exp_z))) {
            prev = b;
            b = b->next;
        }

        if (b != q->head && b->exp_x == a->exp_x && b->exp_y == a->exp_y && b->exp_z == a->exp_z) {
            b->coeff += a->coeff;
            if (b->coeff == 0) {
                prev->next = b->next;
                free(b);
                b = prev->next;
            } 
            else {
                prev = b;
                b = b->next;
            }
        } 
        else {
            Node* t = createNode();
            t->exp_x = a->exp_x;
            t->exp_y = a->exp_y;
            t->exp_z = a->exp_z;
            t->coeff = a->coeff;
            
            prev->next = t;
            t->next = b;
            prev = t;
        }

        a = a->next;
    }

    q->id_num = id;
}


//Test print
void printPolynomial(Polynomial *P) {
    Node *curr = P->head->next;
    printf("Polynomial ID: %d\n", P->id_num);
    while (curr != P->head) {
        printf("%d x^%d y^%d z^%d ", curr->coeff, curr->exp_x, curr->exp_y, curr->exp_z);
        curr = curr->next;
    }
    printf("\n");
}

int main() {
    Polynomial p, q;
    int id1 = 1, id2 = 2;
    

    //Test polynomial addition
    printf("# of Terms, P1");
    int n1; scanf("%d", &n1);
    printf("Enter exp_x exp_y exp_z coeff:\n");
    polyread(&p, id1, n1);
    
    printf("# of Terms, P2");
    int n2; scanf("%d", &n2);
    printf("Enter exp_x exp_y exp_z coeff:\n");
    polyread(&q, id2, n2);
    
    printf("P1:\n");
    printPolynomial(&p);
    printf("P2:\n");
    printPolynomial(&q);
    
    polyadd(&p, &q, 3);
    printf("SUM:\n");
    printPolynomial(&q);
    
    return 0;
}

