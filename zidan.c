/*
 * ============================================================
 *  zidan.c  —  Cursor Movement & Find-Replace
 * ============================================================
 *
 *  Berisi:
 *    1. handleCursorMovement()  — navigasi panah menggunakan linked list
 *    2. findAndReplace()        — cari & ganti kata dalam file
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "edit_cursor.h"
#include "zidan.h"

/* ===========================================================
 *  handleCursorMovement()
 *  -----------------------------------------------------------
 *  Menangani tombol panah (atas/bawah/kiri/kanan).
 *
 *  PERBEDAAN DENGAN VERSI ARRAY:
 *  ==============================
 *
 *  VERSI ARRAY (lama):
 *  -------------------
 *  Naik   : (*cursorRow)--;
 *           → harus hitung strlen(text[*cursorRow]) untuk clamp kolom
 *  Turun  : (*cursorRow)++;
 *           → sama, hitung panjang baris baru
 *  Akses baris pakai indeks: text[*cursorRow]
 *
 *  VERSI LINKED LIST (baru):
 *  -------------------------
 *  Naik   : ed->current = ed->current->prev;
 *           ed->cursorRow--;
 *           → langsung akses baris sebelumnya lewat pointer!
 *  Turun  : ed->current = ed->current->next;
 *           ed->cursorRow++;
 *  Akses baris pakai: ed->current->data
 *
 *  Kuncinya: tidak perlu lagi indeks integer untuk navigasi baris.
 *  Cukup ikuti pointer prev / next.
 *
 *  KODE TOMBOL (Windows _getch() setelah prefix 224):
 *    72 = panah ATAS
 *    80 = panah BAWAH
 *    75 = panah KIRI
 *    77 = panah KANAN
 * =========================================================== */
void handleCursorMovement(int ch,Cursor *cur)
{
    // ATAS
    if (ch == 72) {

        if (cur->current->prev != NULL) {

            cur->current = cur->current->prev;

            cur->cursorRow--;

            int len = strlen(cur->current->data);

            if (cur->cursorCol > len) {

                cur->cursorCol = len;
            }
        }
    }

    // BAWAH
    else if (ch == 80) {

        if (cur->current->next != NULL) {

            cur->current = cur->current->next;

            cur->cursorRow++;

            int len = strlen(cur->current->data);

            if (cur->cursorCol > len) {

                cur->cursorCol = len;
            }
        }
    }

    // KIRI
    else if (ch == 75) {

        if (cur->cursorCol > 0) {

            cur->cursorCol--;
        }

        else if (cur->current->prev != NULL) {

            cur->current = cur->current->prev;

            cur->cursorRow--;

            cur->cursorCol = strlen(cur->current->data);
        }
    }

    // KANAN
    else if (ch == 77) {

        int len = strlen(cur->current->data);

        if (cur->cursorCol < len) {

            cur->cursorCol++;
        }

        else if (cur->current->next != NULL) {

            cur->current = cur->current->next;

            cur->cursorRow++;

            cur->cursorCol = 0;
        }
    }
}

/* ===========================================================
 *  findAndReplace()
 *  -----------------------------------------------------------
 *  Operasi ini bekerja langsung pada FILE (bukan linked list
 *  editor), sehingga tidak perlu diubah strukturnya.
 *  Tetap menggunakan file I/O biasa.
 * =========================================================== */
void findAndReplace() {

    char filename[100];
    char find[100];
    char replace[100];
    char buffer[MAX_COLS];

    printf("\n=== FIND AND REPLACE ===\n");

    printf("Masukkan nama file : ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';

    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("File tidak ditemukan!\n");
        return;
    }

    printf("Kata yang dicari : ");
    fgets(find, sizeof(find), stdin);
    find[strcspn(find, "\n")] = '\0';

    printf("Kata pengganti : ");
    fgets(replace, sizeof(replace), stdin);
    replace[strcspn(replace, "\n")] = '\0';

/* VALIDASI PANJANG REPLACE */

    if (strlen(replace) > 20) {
        printf("Peringatan! Kata pengganti maksimal 20 karakter.\n");
        return;
}

    /* =========================
       BUAT LINKED LIST MANUAL
       ========================= */

    Node *head = NULL;
    Node *tail = NULL;

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {

        buffer[strcspn(buffer, "\n")] = '\0';

        Node *newNode = (Node *)malloc(sizeof(Node));

        strcpy(newNode->data, buffer);

        newNode->next = NULL;
        newNode->prev = NULL;

        if (head == NULL) {

            head = newNode;
            tail = newNode;
        }

        else {

            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    fclose(fp);

    /* =========================
       FIND AND REPLACE
       ========================= */

    int found = 0;

    Node *temp = head;

    while (temp != NULL) {

        char hasil[MAX_COLS * 2] = "";

        char *pos;
        char *start = temp->data;

        while ((pos = strstr(start, find)) != NULL) {

            found++;

            strncat(hasil, start, pos - start);

            strcat(hasil, replace);

            start = pos + strlen(find);
        }

        strcat(hasil, start);

        strcpy(temp->data, hasil);

        temp = temp->next;
    }

    if (found == 0) {

        printf("Kata tidak ditemukan!\n");

        return;
    }

    /* =========================
       SIMPAN KE FILE
       ========================= */

    fp = fopen(filename, "w");

    if (fp == NULL) {
        printf("Gagal membuka file!\n");
        return;
    }

    temp = head;

    while (temp != NULL) {

        fprintf(fp, "%s", temp->data);

        if (temp->next != NULL) {
            fprintf(fp, "\n");
        }

        temp = temp->next;
    }

    fclose(fp);

    printf("Berhasil! %d kata diganti.\n", found);

    /* =========================
       FREE MEMORY
       ========================= */

    temp = head;

    while (temp != NULL) {

        Node *hapus = temp;

        temp = temp->next;

        free(hapus);
    }
}