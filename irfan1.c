#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "irfan1.h"
#include "edit_cursor.h"      // untuk fungsi runEditor, dll.
#include "linkedlist.h"       // Node, MAX_COLS, createNode, appendNode, getNodeAt, freeList

// Fungsi pembantu lokal (insert & delete node di tengah)
void insertNodeLocal(Node **head, int index, const char line[]) {
    Node *newNode = createNode(line);
    if (index == 0 || *head == NULL) {
        newNode->next = *head;
        *head = newNode;
        return;
    }
    Node *prev = getNodeAt(*head, index - 1);
    if (prev == NULL) {
        appendNode(head, line);
        return;
    }
    newNode->next = prev->next;
    prev->next = newNode;
}

void deleteNodeLocal(Node **head, int index) {
    if (*head == NULL) return;
    if (index == 0) {
        Node *temp = *head;
        *head = (*head)->next;
        free(temp);
        return;
    }
    Node *prev = getNodeAt(*head, index - 1);
    if (prev == NULL || prev->next == NULL) return;
    Node *toDelete = prev->next;
    prev->next = toDelete->next;
    free(toDelete);
}

// createNewFile & exitEditor (tidak berubah)
void createNewFile(void) {
    char filename[100];
    Node *head = NULL;                // linked list kosong untuk dokumen baru

    printf("Masukkan Nama File : ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';
    
    runEditor(&head, filename, 1);   // pastikan runEditor sudah pakai linked list
    printf("File berhasil disimpan.\n");
}

void exitEditor() {
    printf("Terimakasih!\n");
    exit(0);
}

// handleTextEditing VERSI LINKED LIST (TANPA MAX_ROWS)
void handleTextEditing(int ch, Node **head, int *cursorRow, int *cursorCol, int *rowCount) {
    Node *curr = getNodeAt(*head, *cursorRow);
    if (curr == NULL) return;

    //  BACKSPACE 
    if (ch == 8) {
        if (*cursorCol > 0) {
            // hapus karakter di dalam baris
            int len = strlen(curr->line);
            if (*cursorCol <= len) {
                for (int i = *cursorCol; i <= len; i++)
                    curr->line[i-1] = curr->line[i];
                (*cursorCol)--;
            }
        }
        else if (*cursorRow > 0) {
            // gabung dengan baris sebelumnya
            Node *prev = getNodeAt(*head, *cursorRow - 1);
            if (prev == NULL) return;

            // pastikan gabungan tidak melebihi kapasitas MAX_COLS
            if (strlen(prev->line) + strlen(curr->line) < MAX_COLS) {
                int prevLen = strlen(prev->line);
                strcat(prev->line, curr->line);
                deleteNodeLocal(head, *cursorRow);
                (*cursorRow)--;
                (*cursorCol) = prevLen;
                (*rowCount)--;
            } 
        }
    }

    // ENTER 
    else if (ch == 13) {
        // TIDAK ADA PENGECEKAN MAX_ROWS! Linked list bisa terus tumbuh.
        char tail[MAX_COLS] = "";
        if (*cursorCol < strlen(curr->line)) {
            strcpy(tail, &curr->line[*cursorCol]);
        }
        curr->line[*cursorCol] = '\0';

        insertNodeLocal(head, *cursorRow + 1, tail);
        (*cursorRow)++;
        (*cursorCol) = 0;
        (*rowCount)++;
    }

    // KARAKTER BIASA (ASCII 32-126)
    else if (ch >= 32 && ch <= 126) {
        // Jika kolom mentok, buat baris baru (word-wrap)
        if (*cursorCol >= MAX_COLS - 1) {
            char tail[MAX_COLS] = "";
            if (*cursorCol < (int)strlen(curr->line)) {
                strcpy(tail, &curr->line[*cursorCol]);
            }
            curr->line[*cursorCol] = '\0';

            insertNodeLocal(head, *cursorRow + 1, tail);
            (*rowCount)++;
            (*cursorRow)++;
            *cursorCol = 0;

            curr = getNodeAt(*head, *cursorRow);
            if (curr == NULL) return;
        }

        // Sisipkan karakter di posisi kursor
        int len = strlen(curr->line);
        if (len < MAX_COLS - 1) {
            for (int i = len; i >= *cursorCol; i--)
                curr->line[i+1] = curr->line[i];
            curr->line[*cursorCol] = ch;
            (*cursorCol)++;
        } 
    }
}