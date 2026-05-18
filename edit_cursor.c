#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include "edit_cursor.h"
#include "zidan.h"
#include "irfan1.h"
#include "linkedlist.h"

/* Handle konsol Windows untuk SetConsoleCursorPosition */
static HANDLE hConsole;

// =========================
// LOAD FILE KE LINKED LIST
// =========================
static void loadFile(Cursor *cursor, const char *filename) { // Fungsi dibuat oleh Rayhan
    FILE *f = fopen(filename, "r");
    if (!f) return;

    char buffer[MAX_COLS];
    while (fgets(buffer, MAX_COLS, f)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        appendNode(cursor, buffer);
    }
    fclose(f);

    // Jika file kosong, tambah satu baris kosong
    if (cursor->rowCount == 0) appendNode(cursor, "");
}

// =========================
// SIMPAN FILE DARI LINKED LIST
// =========================
void saveFile(Cursor *cursor, const char *filename) { // Fungsi dibuat oleh Irfan
    FILE *f = fopen(filename, "w");
    if (!f) return;

    Node *cur = cursor->head;
    while (cur != NULL) {
        fprintf(f, "%s", cur->data);
        if (cur->next != NULL) fprintf(f, "\n");
        cur = cur->next;
    }
    fclose(f);
}

// =========================
// RENDER LAYAR
// =========================
static void render(Cursor *cursor) {
    /* 1. Pindah ke pojok kiri atas */
    COORD topLeft = {0, 0};
    SetConsoleCursorPosition(hConsole, topLeft);

    /* 2. Traversal linked list: cetak setiap baris */
    Node *cur = cursor->head;
    while (cur != NULL) {
        printf("%s", cur->data);
        printf("\x1b[K");           /* hapus sisa baris di layar */
        if (cur->next != NULL) printf("\n");
        cur = cur->next;
    }

    /* 3. Hapus sisa layar di bawah baris terakhir */
    printf("\x1b[J");

    /* 4. Kembalikan cursor konsol ke posisi kursor editor */
    COORD pos = {(SHORT)cursor->cursorCol, (SHORT)cursor->cursorRow};
    SetConsoleCursorPosition(hConsole, pos);
}

// =========================
// FUNGSI UTAMA EDITOR
// =========================
void runEditor(const char *filename) { // Fungsi dibuat oleh Rayhan
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Inisialisasi cursor
    Cursor cursor = {NULL, NULL, 0, 0, 0};

    // Load file ke linked list
    loadFile(&cursor, filename);

    cursor.cursorRow = 0;
    cursor.cursorCol = 0;

    int ch;
    while (1) {
        render(&cursor);
        ch = _getch();

        if (ch == 27) { // ESC untuk keluar dan simpan, dibuat oleh Rayhan
            saveFile(&cursor, filename);
            break;
        }

        if (ch == 224) { // Memanggil fungsi cursor movement dari zidan.c
            ch = _getch();
            handleCursorMovement(ch, &cursor);
        } else {
            handleTextEditing(ch, &cursor); // Memanggil fungsi handleTextEditing dari irfan1.c
        }
    }

    freeList(&cursor);
}