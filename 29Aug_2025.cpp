#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Node {
    int coef, pow;
    struct Node* next;
} Node;

Node* newNode(int coef, int pow) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->coef = coef; n->pow = pow; n->next = NULL;
    return n;
}

void insertTerm(Node** head, int coef, int pow) {
    if (coef == 0) return;
    Node dummy = {0,0,*head};
    Node* prev = &dummy;
    Node* cur = *head;

    while (cur && cur->pow > pow) { prev = cur; cur = cur->next; }

    if (cur && cur->pow == pow) {
        cur->coef += coef;
        if (cur->coef == 0) { prev->next = cur->next; free(cur); }
    } else {
        Node* n = newNode(coef, pow);
        prev->next = n; n->next = cur;
    }
    *head = dummy.next;
}

Node* fromArrays(const int c[], const int p[], int n) {
    Node* P = NULL;
    for (int i = 0; i < n; i++) insertTerm(&P, c[i], p[i]);
    return P;
}

Node* addPoly(const Node* A, const Node* B) {
    Node* R = NULL;
    while (A || B) {
        if (B == NULL || (A && A->pow > B->pow)) { insertTerm(&R, A->coef, A->pow); A = A->next; }
        else if (A == NULL || (B && B->pow > A->pow)) { insertTerm(&R, B->coef, B->pow); B = B->next; }
        else { insertTerm(&R, A->coef + B->coef, A->pow); A = A->next; B = B->next; }
    }
    return R;
}

Node* multiplyPoly(const Node* A, const Node* B) {
    Node* R = NULL;
    for (const Node* pa = A; pa; pa = pa->next)
        for (const Node* pb = B; pb; pb = pb->next)
            insertTerm(&R, pa->coef * pb->coef, pa->pow + pb->pow);
    return R;
}

void printPoly(const Node* P) {
    if (!P) { printf("0\n"); return; }
    int first = 1;
    for (const Node* p = P; p; p = p->next) {
        int c = p->coef, e = p->pow;
        if (!first) printf(" %c ", (c>=0?'+':'-'));
        if (first && c<0) printf("-");
        int a = c>=0?c:-c;
        if (e==0) printf("%d",a);
        else { if (a!=1) printf("%d",a); printf("x"); if (e!=1) printf("^%d",e); }
        first = 0;
    }
    printf("\n");
}

int main(void) {
    Node* P1 = fromArrays((int[]){5,2,3,7}, (int[]){3,2,1,0}, 4);
    Node* P2 = fromArrays((int[]){4,1,9},   (int[]){3,2,0},   3);

    printf("P1(x) = "); printPoly(P1);
    printf("P2(x) = "); printPoly(P2);
    printf("P1+P2 = "); printPoly(addPoly(P1,P2));

    Node* Q1 = fromArrays((int[]){2,3}, (int[]){1,0}, 2);
    Node* Q2 = fromArrays((int[]){1,1}, (int[]){2,0}, 2);
    printf("Q1*Q2 = "); printPoly(multiplyPoly(Q1,Q2));
}