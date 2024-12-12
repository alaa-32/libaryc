#include <stdio.h>
#include <stdlib.h>
#include "library.h"

void InitQueue(Queue* Q) {
    Q->Head = NULL;
    Q->Tail = NULL;
}

void Enqueue(Queue* Q, User x) {
    EQueue V = (EQueue)malloc(sizeof(QNode));
    if (V == NULL) {
        printf("Error: Memory allocation failed!\n");
        return;
    }
    V->Data = x;
    V->Next = NULL;
    if (Q->Head == NULL) {
        Q->Head = V;
    } else {
        Q->Tail->Next = V;
    }
    Q->Tail = V;
}

void Dequeue(Queue* Q, User* x) {
    if (Q->Head == NULL) {
        printf("Error: Queue is empty!\n");
        return;
    }
    EQueue V = Q->Head;
    *x = V->Data;
    Q->Head = Q->Head->Next;
    if (Q->Head == NULL) {
        Q->Tail = NULL;
    }
    free(V);
}

int isQEmpty(Queue Q) {
    return Q.Head == NULL;
}
