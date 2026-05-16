#ifndef IRFAN1_H
#define IRFAN1_H

#include "linkedlist.h"   // karena menggunakan Node *

// Membuat file baru (dokumen kosong) dan menjalankan editor
void createNewFile(void);

// Keluar dari program
void exitEditor(void);

// Menjalankan editor teks untuk file tertentu
// head: pointer ke head linked list (dokumen)
// filename: nama file untuk menyimpan
// isNewFile: 1 jika file baru, 0 jika membuka file yang sudah ada (opsional)
void runEditor(Node **head, const char *filename, int isNewFile);

// Menangani editing teks (backspace, enter, karakter biasa)
// ch: karakter yang ditekan
// head: pointer ke head linked list
// cursorRow, cursorCol: posisi kursor (akan diubah)
// rowCount: jumlah baris saat ini (akan diubah)
void handleTextEditing(int ch, Node **head, int *cursorRow, int *cursorCol, int *rowCount);

#endif