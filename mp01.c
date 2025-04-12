#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int exp_x, exp_y, exp_z;
    float coeff;
    struct node* next;
} Node;

typedef struct polynomial {
    int id_num;
    Node* head;
} Polynomial;

#define MAX_POLYNOMIALS 1000
Polynomial polynomials[MAX_POLYNOMIALS];

Node* createNode() {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->exp_x = 0;
    newNode->exp_y = 0;
    newNode->exp_z = 0;
    newNode->coeff = 0.0;
    newNode->next = NULL;
    return newNode;
}

void zeropoly(Polynomial* p, int id) {
    Node* head = createNode();
    head->next = head;
    p->head = head;
    p->id_num = id;
}

void add_term(Polynomial* p, int ex, int ey, int ez, float coeff) {
    if (coeff == 0.0) return;

    Node *prev = p->head;
    Node *curr = p->head->next;

    while (curr != p->head && 
          (curr->exp_x > ex || 
          (curr->exp_x == ex && curr->exp_y > ey) || 
          (curr->exp_x == ex && curr->exp_y == ey && curr->exp_z > ez)) ) {
        prev = curr;
        curr = curr->next;
    }

    if (curr != p->head && curr->exp_x == ex && curr->exp_y == ey && curr->exp_z == ez) {
        curr->coeff += coeff;
        if (curr->coeff == 0.0) {
            prev->next = curr->next;
            free(curr);
        }
    } else {
        Node* newNode = createNode();
        newNode->exp_x = ex;
        newNode->exp_y = ey;
        newNode->exp_z = ez;
        newNode->coeff = coeff;
        newNode->next = curr;
        prev->next = newNode;
    }
}

void polyread(Polynomial* p, int id, int n) {
    zeropoly(p, id);
    for (int i = 0; i < n; i++) {
        int ex, ey, ez;
        float coef;
        scanf("%d %d %d %f", &ex, &ey, &ez, &coef);
        add_term(p, ex, ey, ez, coef);
    }
}

Polynomial polyadd(Polynomial p, Polynomial q, int id) {
    Polynomial result;
    zeropoly(&result, id);

    Node *a = p.head->next;
    Node *b = q.head->next;

    while (a != p.head || b != q.head) {
        int cmp;
        if (a == p.head) cmp = 1;
        else if (b == q.head) cmp = -1;
        else if (a->exp_x != b->exp_x) cmp = (b->exp_x - a->exp_x);
        else if (a->exp_y != b->exp_y) cmp = (b->exp_y - a->exp_y);
        else cmp = (b->exp_z - a->exp_z);

        int ex, ey, ez;
        float coef;

        if (cmp < 0) {
            ex = a->exp_x; ey = a->exp_y; ez = a->exp_z; coef = a->coeff;
            a = a->next;
        } else if (cmp > 0) {
            ex = b->exp_x; ey = b->exp_y; ez = b->exp_z; coef = b->coeff;
            b = b->next;
        } else {
            ex = a->exp_x; ey = a->exp_y; ez = a->exp_z;
            coef = a->coeff + b->coeff;
            a = a->next; b = b->next;
        }

        if (coef != 0.0) {
            add_term(&result, ex, ey, ez, coef);
        }
    }

    return result;
}

Polynomial polysub(Polynomial p, Polynomial q, int id) {
    Polynomial neg_q;
    zeropoly(&neg_q, id + 1000);
    Node *curr = q.head->next;
    while (curr != q.head) {
        add_term(&neg_q, curr->exp_x, curr->exp_y, curr->exp_z, -curr->coeff);
        curr = curr->next;
    }
    return polyadd(p, neg_q, id);
}

Polynomial multiply_by_term(Polynomial p, int ex, int ey, int ez, float coeff) {
    Polynomial result;
    zeropoly(&result, 0);

    Node *curr = p.head->next;
    while (curr != p.head) {
        int new_ex = curr->exp_x + ex;
        int new_ey = curr->exp_y + ey;
        int new_ez = curr->exp_z + ez;
        float new_coeff = curr->coeff * coeff;
        add_term(&result, new_ex, new_ey, new_ez, new_coeff);
        curr = curr->next;
    }
    return result;
}

Polynomial polydiv(Polynomial p, Polynomial q, int id) {
    Polynomial Q, R;
    zeropoly(&Q, id);
    zeropoly(&R, id + 1);

    Node *curr = p.head->next;
    while (curr != p.head) {
        add_term(&R, curr->exp_x, curr->exp_y, curr->exp_z, curr->coeff);
        curr = curr->next;
    }

    while (1) {
        Node *r_lead = R.head->next;
        if (r_lead == R.head) break;

        Node *b_lead = q.head->next;
        if (b_lead == q.head) break;

        if (r_lead->exp_x < b_lead->exp_x) break;
        if (r_lead->exp_x == b_lead->exp_x) {
            if (r_lead->exp_y < b_lead->exp_y) break;
            if (r_lead->exp_y == b_lead->exp_y) {
                if (r_lead->exp_z < b_lead->exp_z) break;
            }
        }

        int q_ex = r_lead->exp_x - b_lead->exp_x;
        int q_ey = r_lead->exp_y - b_lead->exp_y;
        int q_ez = r_lead->exp_z - b_lead->exp_z;
        float q_coeff = r_lead->coeff / b_lead->coeff;

        add_term(&Q, q_ex, q_ey, q_ez, q_coeff);

        Polynomial T = multiply_by_term(q, q_ex, q_ey, q_ez, q_coeff);
        Polynomial new_R = polysub(R, T, id + 2);
        R = new_R;
    }

    return Q;
}

Polynomial polymod(Polynomial p, Polynomial q, int id) {
    Polynomial Q, R;
    zeropoly(&Q, id + 3);
    zeropoly(&R, id + 4);

    Node *curr = p.head->next;
    while (curr != p.head) {
        add_term(&R, curr->exp_x, curr->exp_y, curr->exp_z, curr->coeff);
        curr = curr->next;
    }

    while (1) {
        Node *r_lead = R.head->next;
        if (r_lead == R.head) break;

        Node *b_lead = q.head->next;
        if (b_lead == q.head) break;

        if (r_lead->exp_x < b_lead->exp_x) break;
        if (r_lead->exp_x == b_lead->exp_x) {
            if (r_lead->exp_y < b_lead->exp_y) break;
            if (r_lead->exp_y == b_lead->exp_y) {
                if (r_lead->exp_z < b_lead->exp_z) break;
            }
        }

        int q_ex = r_lead->exp_x - b_lead->exp_x;
        int q_ey = r_lead->exp_y - b_lead->exp_y;
        int q_ez = r_lead->exp_z - b_lead->exp_z;
        float q_coeff = r_lead->coeff / b_lead->coeff;

        add_term(&Q, q_ex, q_ey, q_ez, q_coeff);

        Polynomial T = multiply_by_term(q, q_ex, q_ey, q_ez, q_coeff);
        Polynomial new_R = polysub(R, T, id + 5);
        R = new_R;
    }

    if (R.head->next == R.head) {
        add_term(&R, 0, 0, 0, 0.0);  
    }
    
    return R;
}

Polynomial polymul(Polynomial p, Polynomial q, int id) {
    Polynomial result;
    zeropoly(&result, id);

    Node *a = p.head->next;
    while (a != p.head) {
        Node *b = q.head->next;
        while (b != q.head) {
            int ex = a->exp_x + b->exp_x;
            int ey = a->exp_y + b->exp_y;
            int ez = a->exp_z + b->exp_z;
            float coef = a->coeff * b->coeff;
            add_term(&result, ex, ey, ez, coef);
            b = b->next;
        }
        a = a->next;
    }
    return result;
}

void polyprint(Polynomial p) {
    Node* curr = p.head->next;
    printf("---\n");
    while (curr != p.head) {
        printf("%d %d %d %.3f\n", curr->exp_x, curr->exp_y, curr->exp_z, curr->coeff);
        curr = curr->next;
    }
}

int main() {
    Polynomial result;
    zeropoly(&result, 0);  

    int next_id = 1;
    char op;

    while (1) {
        scanf(" %c", &op);
        if (op == '#') break;

        int terms_a, terms_b;
        Polynomial A, B, temp;

        
        scanf("%d", &terms_a);
        polyread(&A, next_id, terms_a);
        next_id++;

       
        scanf("%d", &terms_b);
        polyread(&B, next_id, terms_b);
        next_id++;

     
        switch (op) {
            case '+':
                temp = polyadd(A, B, next_id);
                break;
            case '-':
                temp = polysub(A, B, next_id);
                break;
            case '*':
                temp = polymul(A, B, next_id);
                break;
            case '/':
                temp = polydiv(A, B, next_id);
                break;
            case '%':
                temp = polymod(A, B, next_id);
                break;
            default:
                printf("Invalid operation.\n");
                continue;
        }

  
        result = temp;
        polyprint(result);
        if (result.head->next == result.head) {
            printf("0 0 0 0.000\n");
        }
        next_id++;
    }

    return 0;
}
