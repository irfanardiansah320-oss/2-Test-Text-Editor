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

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    int screenWidth  = csbi.dwSize.X;
    int screenHeight = csbi.dwSize.Y - 1; // -1 biar aman dari baris terakhir terminal

    // =============================================
    // SCROLL OFFSET — ini kunci fix-nya
    // Menyimpan baris mana yang tampil paling atas
    // =============================================
    static int scrollOffset = 0;

    // Kalau kursor naik melebihi atas tampilan → geser tampilan ke atas
    if (cursor->cursorRow < scrollOffset) {
        scrollOffset = cursor->cursorRow;
    }
    // Kalau kursor turun melebihi bawah tampilan → geser tampilan ke bawah
    if (cursor->cursorRow >= scrollOffset + screenHeight) {
        scrollOffset = cursor->cursorRow - screenHeight + 1;
    }

    // =============================================
    // Mulai traversal dari node di posisi scrollOffset
    // (bukan dari head!)
    // =============================================
    Node *cur = cursor->head;
    int i = 0;
    while (cur != NULL && i < scrollOffset) {
        cur = cur->next;
        i++;
    }

    // Cetak baris yang masuk dalam jendela tampilan
    int row = 0;
    while (cur != NULL && row < screenHeight) {

        gotoxy(0, row);
        printf("%s", cur->data);

        int textLen   = (int)strlen(cur->data);
        int sisaKolom = screenWidth - textLen;

        if (sisaKolom > 0) {
            COORD pos = {(SHORT)textLen, (SHORT)row};
            DWORD written;
            FillConsoleOutputCharacter(hConsole, ' ', sisaKolom, pos, &written);
        }

        cur = cur->next;
        row++;
    }

    // Bersihkan baris kosong di bawah konten
    for (int r = row; r < screenHeight; r++) {
        COORD baris = {0, (SHORT)r};
        DWORD written;
        FillConsoleOutputCharacter(hConsole, ' ', screenWidth, baris, &written);
    }

    // =============================================
    // Kembalikan kursor ke posisi VISUAL
    // bukan cursorRow mentah, tapi dikurangi scrollOffset
    // =============================================
    int visualRow = cursor->cursorRow - scrollOffset;
    gotoxy(cursor->cursorCol, visualRow);
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