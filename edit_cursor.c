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