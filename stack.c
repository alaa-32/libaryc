#include <stdio.h>
#include <stdlib.h>
#include "library.h"

void InitStack(Stack* P) {
    *P = NULL;
}

void Push(Stack* S, Book x) {
    Stack V = (Stack)malloc(sizeof(SNode));
    if (V == NULL) {
        printf("Error: Memory allocation failed!\n");
        return;
    }
    V->Data = x;
    V->Next = *S;
    *S = V;
}

void Pop(Stack* S, Book* x) {
    if (*S == NULL) {
        printf("Error: Stack is empty!\n");
        return;
    }
    Stack V = *S;
    *x = V->Data;
    *S = V->Next;
    free(V);
}

int isSEmpty(Stack S) {
    return S == NULL;
}

