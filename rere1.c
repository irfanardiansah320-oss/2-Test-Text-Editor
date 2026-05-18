#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"
#include "edit_cursor.h"
#include "rere1.h"

// =========================
// HELPER: GET NODE AT INDEX
// =========================
Node* getNodeAt(Cursor *cursor, int index) {
    Node *curr = cursor->head;
    int i = 0;
    while (curr != NULL && i < index) {
        curr = curr->next;
        i++;
    }
    return curr;
}

// =========================
// TAMPILKAN ISI LINKED LIST
// =========================
void tampilkanNode(Cursor *cursor) {
    if (cursor->head == NULL) {
        printf("(Kosong)\n");
        return;
    }
    Node *curr = cursor->head;
    int no = 1;
    while (curr != NULL) {
        printf("%3d. %s\n", no++, curr->data);
        curr = curr->next;
    }
}

// =========================
// SIMPAN KE FILE
// =========================
void simpanKeFile(Cursor *cursor, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        printf("\033[1;31mGagal menyimpan file!\033[0m\n");
        return;
    }
    Node *curr = cursor->head;
    while (curr != NULL) {
        fprintf(f, "%s", curr->data);
        if (curr->next != NULL) fprintf(f, "\n");
        curr = curr->next;
    }
    fclose(f);
    printf("\033[1;32mFile telah disimpan.\033[0m\n");
}

// =========================
// OPEN FILE (PAKAI LINKED LIST)
// =========================
void openFile() {
    char filename[100];
    printf("\n\033[1;36m=== OPEN FILE ===\033[0m\n");
    printf("Masukkan nama file: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("\033[1;31mFile '%s' not found!\033[0m\n", filename);
        return;
    }

    // Baca file ke linked list
    Cursor cursor = {NULL, NULL, 0, 0, 0};
    char buffer[MAX_COLS];
    while (fgets(buffer, MAX_COLS, file)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        appendNode(&cursor, buffer);
    }
    fclose(file);

    // Tampilkan dari linked list
    printf("\033[1;33mFile contents:\033[0m\n");
    printf("----------------------------------------\n");
    tampilkanNode(&cursor);
    printf("\n----------------------------------------\n");

    char choice;
    printf("\nApakah kamu ingin mengedit file ini? (y/n): ");
    scanf(" %c", &choice);
    getchar();

    if (choice == 'y' || choice == 'Y') {
        freeList(&cursor); // bebaskan dulu, runEditor akan load ulang
        runEditor(filename);
        printf("\033[1;32mFile '%s' telah disimpan.\033[0m\n", filename);
    } else {
        printf("File tidak diedit.\n");
        freeList(&cursor);
    }
}

// =========================
// DELETE FILE
// =========================
// void deletefile() {
//     char filename[100];
//     char confirm;

//     printf("\n\033[1;31m=== HAPUS FILE ===\033[0m\n");
//     printf("Masukkan nama file yang akan dihapus: ");
//     fgets(filename, sizeof(filename), stdin);
//     filename[strcspn(filename, "\n")] = '\0';

//     printf("Apakah Anda yakin ingin menghapus '%s'? (y/n): ", filename);
//     scanf(" %c", &confirm);
//     getchar();

//     if (confirm == 'y' || confirm == 'Y') {
//         if (remove(filename) == 0) {
//             printf("\033[1;32mFile '%s' berhasil dihapus.\033[0m\n", filename);
//         } else {
//             printf("\033[1;31mGagal menghapus file '%s'. Pastikan file ada.\033[0m\n", filename);
//         }
//     } else {
//         printf("Penghapusan dibatalkan.\n");
//     }
