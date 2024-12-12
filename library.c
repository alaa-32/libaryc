#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

#define MAX_BOOKS 100
Book library[MAX_BOOKS];
int bookCount = 0;
Stack recentlyReturned;
Queue borrowQueue;

void LoadBooksFromFile() {
    FILE* file = fopen("books.txt", "r");
    if (file == NULL) {
        printf("No books file found. Starting with an empty library.\n");
        return;
    }
    bookCount = 0;
    while (fscanf(file, "%d %[^\n]%*c %[^\n]%*c %d", 
                  &library[bookCount].id, 
                  library[bookCount].title, 
                  library[bookCount].author, 
                  &library[bookCount].available) == 4) {
        bookCount++;
    }
    fclose(file);
}

void SaveBooksToFile() {
    FILE* file = fopen("books.txt", "w");
    if (file == NULL) {
        printf("Error saving books to file.\n");
        return;
    }
    for (int i = 0; i < bookCount; i++) {
        fprintf(file, "%d\n%s\n%s\n%d\n", 
                library[i].id, 
                library[i].title, 
                library[i].author, 
                library[i].available);
    }
    fclose(file);
}

void AddBook() {
    if (bookCount >= MAX_BOOKS) {
        printf("Library is full!\n");
        return;
    }
    Book b;
    printf("Enter Book ID: ");
    scanf("%d", &b.id);
    printf("Enter Book Title: ");
    scanf(" %[^\n]%*c", b.title);
    printf("Enter Author Name: ");
    scanf(" %[^\n]%*c", b.author);
    b.available = 1;
    library[bookCount++] = b;
    printf("Book added successfully!\n");
}

void BorrowBook() {
    int bookId;
    printf("Enter Book ID to borrow: ");
    scanf("%d", &bookId);
    for (int i = 0; i < bookCount; i++) {
        if (library[i].id == bookId) {
            if (library[i].available) {
                library[i].available = 0;
                printf("Book borrowed successfully!\n");
                return;
            } else {
                User u;
                printf("Book unavailable. Enter your User ID: ");
                scanf("%d", &u.userId);
                printf("Enter your Name: ");
                scanf(" %[^\n]%*c", u.name);
                u.requestedBookId = bookId;
                Enqueue(&borrowQueue, u);
                printf("You have been added to the waitlist.\n");
                return;
            }
        }
    }
    printf("Book not found!\n");
}

void ReturnBook() {
    int bookId;
    printf("Enter Book ID to return: ");
    scanf("%d", &bookId);
    for (int i = 0; i < bookCount; i++) {
        if (library[i].id == bookId) {
            library[i].available = 1;
            Push(&recentlyReturned, library[i]);
            printf("Book returned successfully!\n");
            return;
        }
    }
    printf("Book not found!\n");
}

void ProcessRequests() {
    if (isQEmpty(borrowQueue)) {
        printf("No pending borrow requests.\n");
        return;
    }
    User u;
    Dequeue(&borrowQueue, &u);
    for (int i = 0; i < bookCount; i++) {
        if (library[i].id == u.requestedBookId) {
            library[i].available = 0;
            printf("Request processed for User ID %d. Book '%s' is now borrowed.\n", u.userId, library[i].title);
            return;
        }
    }
}

void DisplayStack() {
    Stack temp = recentlyReturned;
    if (isSEmpty(temp)) {
        printf("No recently returned books.\n");
        return;
    }
    printf("Recently Returned Books:\n");
    while (temp != NULL) {
        printf("ID: %d, Title: %s, Author: %s\n", temp->Data.id, temp->Data.title, temp->Data.author);
        temp = temp->Next;
    }
}

void DisplayQueue() {
    if (isQEmpty(borrowQueue)) {
        printf("No users in the borrow request queue.\n");
        return;
    }
    printf("Borrow Request Queue:\n");
    EQueue temp = borrowQueue.Head;
    while (temp != NULL) {
        printf("User ID: %d, Name: %s, Requested Book ID: %d\n", temp->Data.userId, temp->Data.name, temp->Data.requestedBookId);
        temp = temp->Next;
    }
}

void SearchBook() {
    int bookId;
    printf("Enter Book ID to search: ");
    scanf("%d", &bookId);
    for (int i = 0; i < bookCount; i++) {
        if (library[i].id == bookId) {
            printf("Book found: ID: %d, Title: %s, Author: %s, %s\n",
                   library[i].id, library[i].title, library[i].author, library[i].available ? "Available" : "Unavailable");
            return;
        }
    }
    printf("Book not found!\n");
}

int main() {
    InitStack(&recentlyReturned);
    InitQueue(&borrowQueue);

    LoadBooksFromFile();

    int choice;
    do {
        printf("\nLibrary Management System\n");
        printf("1. Add New Book\n");
        printf("2. Borrow Book\n");
        printf("3. Return Book\n");
        printf("4. View Recently Returned Books\n");
        printf("5. Display Borrow Request Queue\n");
        printf("6. Process Borrow Requests\n");
        printf("7. Search for a Book\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: AddBook(); break;
            case 2: BorrowBook(); break;
            case 3: ReturnBook(); break;
            case 4: DisplayStack(); break;
            case 5: DisplayQueue(); break;
            case 6: ProcessRequests(); break;
            case 7: SearchBook(); break;
            case 8: SaveBooksToFile(); printf("Exiting program...\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 8);

    return 0;
}
