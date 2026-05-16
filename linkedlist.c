#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

Node *createNode(const char text[]) {
    Node *newNode = (Node *)malloc(sizeof(Node));

    if (newNode == NULL) {
        printf("Gagal alokasi memori\n");
        return NULL;
    }

    strncpy(newNode->data, text, MAX_COLS - 1);
    newNode->data[MAX_COLS - 1] = '\0';

    newNode->prev = NULL;
    newNode->next = NULL;

    return newNode;
}

void freeList(Cursor *cursor) {
    Node *temp = cursor->head;

    while (temp != NULL) {
        Node *hapus = temp;
        temp = temp->next;
        free(hapus);
    }

    cursor->head = NULL;
    cursor->current = NULL;
    cursor->rowCount = 0;
}

void appendNode(Cursor *cursor, const char *text) {

    Node *newNode = createNode(text);

    if (newNode == NULL) {
        return;
    }

    if (cursor->head == NULL) {

        cursor->head = newNode;
        cursor->current = newNode;
    }

    else {

        Node *tail = cursor->head;

        while (tail->next != NULL) {
            tail = tail->next;
        }

        tail->next = newNode;
        newNode->prev = tail;
    }

    cursor->rowCount++;
}