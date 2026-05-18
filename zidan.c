#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "edit_cursor.h"
#include "zidan.h"

void findAndReplace() {

    char filename[50];
    char find[20];
    char replace[20];
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

    if (strlen(replace) > 20) {
        printf("Peringatan! Kata pengganti maksimal 20 karakter.\n");
        fclose(fp);
        return;
    }

    /* =========================
       BUAT LINKED LIST
       pakai createNode() yang sudah ada
       ========================= */

    Node *head = NULL;
    Node *tail = NULL;

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {

        buffer[strcspn(buffer, "\n")] = '\0';

        
        Node *newNode = createNode(buffer);

        
        if (newNode == NULL) {
            printf("Gagal alokasi memori!\n");
            fclose(fp);
            return;
        }

        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next    = newNode;
            newNode->prev = tail;
            tail          = newNode;
        }
    }

    fclose(fp);

    /* FIND AND REPLACE */

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

        /* bebaskan memori sebelum return */
        temp = head;
        while (temp != NULL) {
            Node *hapus = temp;
            temp = temp->next;
            free(hapus);
        }
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
        if (temp->next != NULL) fprintf(fp, "\n");
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