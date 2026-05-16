#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#define MAX_COLS 100

typedef struct Node {
    char data[MAX_COLS];
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *current;
    int cursorRow;
    int cursorCol;
    int rowCount;
} Cursor;

Node *createNode(const char text[]);
void appendNode(Cursor *cursor, const char *text);
void freeList(Cursor *cursor);

#endif