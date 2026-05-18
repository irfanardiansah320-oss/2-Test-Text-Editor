#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "irfan1.h"
#include "edit_cursor.h"
#include "linkedlist.h"

// =========================
// HELPER: INSERT NODE DI TENGAH
// =========================
static void insertNodeAt(Cursor *cursor, int index, const char line[]) {
    Node *newNode = createNode(line);
    if (newNode == NULL) return;

    if (cursor->head == NULL || index == 0) {
        newNode->next = cursor->head;
        if (cursor->head != NULL) cursor->head->prev = newNode;
        cursor->head = newNode;
        cursor->rowCount++;
        return;
    }

    // Cari node pada posisi index-1
    Node *prev = cursor->head;
    int i = 0;
    while (prev->next != NULL && i < index - 1) {
        prev = prev->next;
        i++;
    }

    newNode->next = prev->next;
    newNode->prev = prev;
    if (prev->next != NULL) prev->next->prev = newNode;
    prev->next = newNode;
    cursor->rowCount++;
}

// =========================
// HELPER: HAPUS NODE DI TENGAH
// =========================
static void deleteNodeAt(Cursor *cursor, int index) {
    if (cursor->head == NULL) return;

    Node *target = cursor->head;
    int i = 0;
    while (target != NULL && i < index) {
        target = target->next;
        i++;
    }
    if (target == NULL) return;

    if (target->prev != NULL) target->prev->next = target->next;
    else cursor->head = target->next;

    if (target->next != NULL) target->next->prev = target->prev;

    free(target);
    cursor->rowCount--;
}

// =========================
// CREATE NEW FILE
// =========================
void createNewFile(void) {
    char filename[100];

    printf("Masukkan Nama File : ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';

    // Buat file kosong dulu agar bisa dibuka oleh runEditor
    FILE *f = fopen(filename, "w");
    if (f) fclose(f);

    runEditor(filename);
    printf("File berhasil disimpan.\n");
}

// =========================
// EXIT EDITOR
// =========================
void exitEditor(void) {
    printf("Terimakasih!\n");
    exit(0);
}

// =========================
// HANDLE TEXT EDITING
// =========================
void handleTextEditing(int ch, Cursor *cursor) {
    Node *curr = cursor->current;
    if (curr == NULL) return;

    // BACKSPACE
    if (ch == 8) {
        if (cursor->cursorCol > 0) {
            // Hapus karakter di dalam baris
            int len = strlen(curr->data);
            if (cursor->cursorCol <= len) {
                for (int i = cursor->cursorCol; i <= len; i++)
                    curr->data[i - 1] = curr->data[i];
                cursor->cursorCol--;
            }
        } else if (cursor->cursorRow > 0) {
            // Gabung dengan baris sebelumnya
            Node *prev = curr->prev;
            if (prev == NULL) return;

            if (strlen(prev->data) + strlen(curr->data) < MAX_COLS) {
                int prevLen = strlen(prev->data);
                strcat(prev->data, curr->data);
                deleteNodeAt(cursor, cursor->cursorRow);
                cursor->cursorRow--;
                cursor->current = prev;
                cursor->cursorCol = prevLen;
            }
        }
    }

    // ENTER
    else if (ch == 13) {
        char tail[MAX_COLS] = "";
        if (cursor->cursorCol < (int)strlen(curr->data)) {
            strcpy(tail, &curr->data[cursor->cursorCol]);
        }
        curr->data[cursor->cursorCol] = '\0';

        insertNodeAt(cursor, cursor->cursorRow + 1, tail);

        // Update current ke node baru
        cursor->current = curr->next;
        cursor->cursorRow++;
        cursor->cursorCol = 0;
    }

    // KARAKTER BIASA (ASCII 32-126)
    else if (ch >= 32 && ch <= 126) {
        // Jika kolom mentok, buat baris baru (word-wrap)
        if (cursor->cursorCol >= MAX_COLS - 1) {
            char tail[MAX_COLS] = "";
            if (cursor->cursorCol < (int)strlen(curr->data)) {
                strcpy(tail, &curr->data[cursor->cursorCol]);
            }
            curr->data[cursor->cursorCol] = '\0';

            insertNodeAt(cursor, cursor->cursorRow + 1, tail);
            cursor->cursorRow++;
            cursor->cursorCol = 0;
            cursor->current = curr->next;
            curr = cursor->current;
            if (curr == NULL) return;
        }

        // Sisipkan karakter di posisi kursor
        int len = strlen(curr->data);
        if (len < MAX_COLS - 1) {
            for (int i = len; i >= cursor->cursorCol; i--)
                curr->data[i + 1] = curr->data[i];
            curr->data[cursor->cursorCol] = (char)ch;
            cursor->cursorCol++;
        }
    }
}