#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

#define MAX_BOOKS 100
Book library[MAX_BOOKS]; // Array to store book details
int bookCount = 0;
Stack recentlyReturned; // Stack to manage recently returned books
Queue borrowQueue;  // Queue to manage borrow requests

void LoadBooksFromFile() {
    FILE* file = fopen("books.txt", "r");
    if (file == NULL) {
        printf("No books file found. Starting with an empty library.\n");
        return;
    }
    bookCount = 0;
    // Read book details from the file
    while (fscanf(file, "%d %[^\n]%*c %[^\n]%*c %d", 
                  &library[bookCount].id, 
                  library[bookCount].title, 
                  library[bookCount].author, 
                  &library[bookCount].available) == 4) {
        bookCount++;
    }
    fclose(file);
}
          //  save the library data into a file  
void SaveBooksToFile() {
    FILE *file = fopen("library.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
      // Write each book's details to the file
    for (int i = 0; i < bookCount; i++) {
        fprintf(file, "%d|%s|%s|%d\n", library[i].id, library[i].title, library[i].author, library[i].available);
    }

    fclose(file);
    printf("Library saved to file successfully.\n");
}

// Function to add a new book to the library
void AddBook() {
    if (bookCount >= MAX_BOOKS) { //see if the library is full
        printf("Library is full!\n");
        return;
    }
    Book b; // Create a new book structure
    printf("Enter Book ID: ");
    scanf("%d", &b.id);
    printf("Enter Book Title: ");
    scanf(" %[^\n]%*c", b.title);
    printf("Enter Author Name: ");
    scanf(" %[^\n]%*c", b.author);
    b.available = 1; //  book is  available
    library[bookCount++] = b;// Add the book to the library array and increment count
    SaveBooksToFile();
    printf("Book added successfully!\n");
}

void BorrowBook() {
    int bookId;
    printf("Enter Book ID to borrow: ");
    scanf("%d", &bookId);

    for (int i = 0; i < bookCount; i++) { //search for the book
        if (library[i].id == bookId) {
            if (library[i].available) {
                library[i].available = 0; // Mark book as borrowed
                printf("Book borrowed successfully!\n");
                SaveBooksToFile(); // Save changes
                return;
            } else { //the book isnt available
                User u;
                printf("Book is currently unavailable. Enter your User ID: ");
                scanf("%d", &u.userId);
                printf("Enter your Name: ");
                scanf(" %[^\n]%*c", u.name);
                u.requestedBookId = bookId;// Link the book to the user's request
                Enqueue(&borrowQueue, u); // Add to queue
                printf("You have been added to the borrow request queue.\n");
                return;
            }
        }
    }
    printf("Book not found!\n");
}

// return a book

void ReturnBook() {
    int bookId;
    printf("Enter Book ID to return: ");
    scanf("%d", &bookId);

    for (int i = 0; i < bookCount; i++) {
        if (library[i].id == bookId) {
            library[i].available = 1; // Mark book as available
            Push(&recentlyReturned, library[i]); // Add to recently returned stack
            printf("Book returned successfully!\n");
            SaveBooksToFile();
            ProcessRequests(); // Check if someone is waiting for the book
            return;
        }
    }
    printf("Book not found!\n");
}

// Function to process borrow requests
void ProcessRequests() {
    if (isQEmpty(borrowQueue)) {// if there are no requests
        printf("No pending borrow requests.\n");
        return;
    }

    Queue tempQueue;
    InitQueue(&tempQueue); // Temporary queue to process requests
    int bookProcessed = 0;

    while (!isQEmpty(borrowQueue)) {
        User u;
        Dequeue(&borrowQueue, &u);// Get the first user in the queue

        for (int i = 0; i < bookCount; i++) {
            if (library[i].id == u.requestedBookId && library[i].available) {
                library[i].available = 0; // Borrow the book for the user
                printf("Request processed for User ID %d. Book '%s' is now borrowed.\n", u.userId, library[i].title);
                SaveBooksToFile();
                bookProcessed = 1;
                break;
            }
        }

       // If book not available, add user back to queue
        if (!bookProcessed) {
            Enqueue(&tempQueue, u);
        }
    }

    // Replace the original queue with the updated temporary queue
    borrowQueue = tempQueue;

    if (!bookProcessed) {
        printf("No books available to fulfill pending requests.\n");
    }
}

//  display the stack of recently returned books
void DisplayStack() {
    Stack temp = recentlyReturned;
    if (isSEmpty(temp)) { // Check if stack is empty
        printf("No recently returned books.\n");
        return;
    }
    printf("Recently Returned Books:\n");
    while (temp != NULL) {
        printf("ID: %d, Title: %s, Author: %s\n", temp->Data.id, temp->Data.title, temp->Data.author);
        temp = temp->Next; // Move to the next book in the stack
    }
}
//  display the borrow request queue
void DisplayQueue() {
    if (isQEmpty(borrowQueue)) {
        printf("No users in the borrow request queue.\n");
        return;
    }
    printf("Borrow Request Queue:\n");
    EQueue temp = borrowQueue.Head;
    while (temp != NULL) {
        printf("User ID: %d, Name: %s, Requested Book ID: %d\n", temp->Data.userId, temp->Data.name, temp->Data.requestedBookId);
        temp = temp->Next;// Move to the next user in the queue
    }
}

void SearchBook() {
    int bookId;
    printf("Enter Book ID to search: ");
    scanf("%d", &bookId);
    for (int i = 0; i < bookCount; i++) { // Check each book
        if (library[i].id == bookId) {
            printf("Book found: ID: %d, Title: %s, Author: %s, %s\n",
                   library[i].id, library[i].title, library[i].author, library[i].available ? "Available" : "Unavailable");
            return;
        }
    }
    printf("Book not found!\n");
}

int main() {
    InitStack(&recentlyReturned);// Initialize stack
    InitQueue(&borrowQueue);// Initialize queue
      LoadBooksFromFile(); // Load books from file

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
