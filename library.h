#ifndef LIBRARY_H
#define LIBRARY_H

#define MAX_TITLE 100
#define MAX_AUTHOR 100
#define MAX_NAME 100

// Book structure
typedef struct {
    int id;
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    int available; // 1 = available, 0 = unavailable
} Book;

// User structure
typedef struct {
    int userId;
    char name[MAX_NAME];
    int requestedBookId;
} User;

// Stack operations
typedef struct SNode {
    Book Data;
    struct SNode* Next;
} SNode, *Stack;

void InitStack(Stack* P);
void Push(Stack* S, Book x);
void Pop(Stack* S, Book* x);
int isSEmpty(Stack S);

// Queue operations
typedef struct QNode {
    User Data;
    struct QNode* Next;
} QNode, *EQueue;

typedef struct {
    EQueue Head, Tail;
} Queue;

void InitQueue(Queue* Q);
void Enqueue(Queue* Q, User x);
void Dequeue(Queue* Q, User* x);
int isQEmpty(Queue Q);

// Library management functions
void LoadBooksFromFile();
void SaveBooksToFile();
void AddBook();
void BorrowBook();
void ReturnBook();
void ProcessRequests();
void DisplayStack();
void DisplayQueue();
void SearchBook();

#endif
