#ifndef IRFAN1_H
#define IRFAN1_H

#include "linkedlist.h"   // karena menggunakan Node *

// Membuat file baru (dokumen kosong) dan menjalankan editor
void createNewFile(void);

// Keluar dari program
void exitEditor(void);

// Menangani editing teks (backspace, enter, karakter biasa)
// ch: karakter yang ditekan
// cursor: cursor editor dengan linked list dan posisi kursor
void handleTextEditing(int ch, Cursor *cursor);

#endif