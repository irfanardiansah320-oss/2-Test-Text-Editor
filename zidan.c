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
    char find[100], replace[100];
    char buffer[1000];
    char result[100000] = "";   /* buffer hasil lebih besar */

    printf("\n\033[1;36m=== FIND AND REPLACE ===\033[0m\n");

    printf("Masukkan nama file  : ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("\033[1;31mFile '%s' tidak ditemukan!\033[0m\n", filename);
        return;
    }

    printf("Kata yang dicari    : ");
    fgets(find, sizeof(find), stdin);
    find[strcspn(find, "\n")] = '\0';

    if (strlen(find) == 0) {
        printf("Input tidak boleh kosong!\n");
        fclose(fp);
        return;
    }

    printf("Kata pengganti      : ");
    fgets(replace, sizeof(replace), stdin);
    replace[strcspn(replace, "\n")] = '\0';

    if (strlen(replace) > 50) {
        printf("\n[!] Kata pengganti maksimal 50 karakter!\n");
        fclose(fp);
        return;
    }

    int found = 0;

    while (fgets(buffer, sizeof(buffer), fp)) {
        char temp[1000];
        char *pos, *start = buffer;

        while ((pos = strstr(start, find)) != NULL) {
            found++;

            /* salin bagian sebelum kata yang ditemukan */
            strncpy(temp, start, pos - start);
            temp[pos - start] = '\0';

            strcat(result, temp);
            strcat(result, replace);

            start = pos + strlen(find);
        }
        /* salin sisa baris setelah penggantian terakhir */
        strcat(result, start);
    }

    fclose(fp);

    if (found == 0) {
        printf("\033[1;33mKata '%s' tidak ditemukan dalam file!\033[0m\n", find);
        return;
    }

    fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Gagal membuka file untuk ditulis!\n");
        return;
    }

    fputs(result, fp);
    fclose(fp);

    printf("\033[1;32mBerhasil! %d kata '%s' diganti dengan '%s'.\033[0m\n",
           found, find, replace);
}
