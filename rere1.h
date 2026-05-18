#ifndef RERE1_H
#define RERE1_H

#include "linkedlist.h"

// Helper
Node* getNodeAt(Cursor *cursor, int index);

// Tampilan
void tampilkanNode(Cursor *cursor);
void simpanKeFile(Cursor *cursor, const char *filename);

// Menu utama
void openFile();
void deletefile();

// Editor linked list
void editLinkedList(Cursor *cursor, const char *filename);

#endif