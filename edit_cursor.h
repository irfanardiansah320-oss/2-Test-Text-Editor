#ifndef EDIT_CURSOR_H
#define EDIT_CURSOR_H

#include "linkedlist.h"

void runEditor(const char *filename);
void saveFile(Cursor *cursor, const char *filename);

#endif