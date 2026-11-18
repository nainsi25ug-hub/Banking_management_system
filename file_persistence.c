#include <stdio.h>
#include <stdlib.h>
#include "file_persistence.h"
#include "linked_list.h"   // append_account use karne ke liye

/*
 * Loads all accounts from the binary file into the linked list.
 * Uses append_account() from Linked List module.
 * Returns 0 on success, -1 on failure.
 */
int load_accounts_from_file(AccountList *list, const char *filename)
{
    FILE *fp = fopen(filename, "rb");

    // File nahi mili to empty list se start (pehli baar normal hai)
    if (fp == NULL) {
        return 0;
    }

    Account acc;

    while (fread(&acc, sizeof(Account), 1, fp) == 1) {
        // Linked List wale ka function use kar rahe hain
        if (append_account(list, &acc) == 0) {
            fclose(fp);
            return -1;  // Memory allocation fail ho gaya
        }
    }

    fclose(fp);
    return 0;
}

/*
 * Saves the entire linked list into a binary file.
 * Overwrites the old file.
 */
int save_accounts_to_file(const AccountList *list, const char *filename)
{
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        return -1;
    }

    Node *curr = list->head;

    while (curr != NULL) {
        if (fwrite(&curr->data, sizeof(Account), 1, fp) != 1) {
            fclose(fp);
            return -1;
        }
        curr = curr->next;
    }

    fclose(fp);
    return 0;
}