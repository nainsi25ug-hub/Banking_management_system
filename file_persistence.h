#ifndef FILE_PERSISTENCE_H
#define FILE_PERSISTENCE_H

#include "bank.h"

// Load all accounts from binary file into the linked list
// Returns 0 on success, -1 on failure
int load_accounts_from_file(AccountList *list, const char *filename);

// Save the entire linked list into binary file
// Returns 0 on success, -1 on failure
int save_accounts_to_file(const AccountList *list, const char *filename);

#endif