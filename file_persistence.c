#include <stdio.h>
#include "file_persistence.h"
#include "linked_list.h"

int load_accounts_from_file(AccountList *list, const char *filename)
{
    FILE *fp;
    Account account;
    if (list == NULL || filename == NULL) return -1;
    fp = fopen(filename, "rb");
    if (fp == NULL) return 0;
    for (;;) {
        size_t count = fread(&account, sizeof(account), 1, fp);
        if (count == 1) {
            if (append_account(list, &account) == NULL) { fclose(fp); return -1; }
            continue;
        }
        if (ferror(fp) || !feof(fp)) { fclose(fp); return -1; }
        break;
    }
    fclose(fp);
    return 0;
}

int save_accounts_to_file(const AccountList *list, const char *filename)
{
    FILE *fp;
    const Node *current;
    if (list == NULL || filename == NULL) return -1;
    fp = fopen(filename, "wb");
    if (fp == NULL) return -1;
    for (current = list->head; current != NULL; current = current->next) {
        if (fwrite(&current->data, sizeof(current->data), 1, fp) != 1) { fclose(fp); return -1; }
    }
    return fclose(fp) == 0 ? 0 : -1;
}
