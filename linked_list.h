#ifndef LINKED_LIST_H  // Prevents multiple inclusion of this header file.
#define LINKED_LIST_H

#include "bank.h"  // Includes shared banking data structures.

// Initializes an empty account list.
void initialize_account_list(AccountList *list);

// Searches for an account using its account number.
Node *find_account_node(const AccountList *list, int account_number);

// Adds a new account node at the end of the list.
Node *append_account(AccountList *list, const Account *account);

// Creates a new bank account and adds it to the list.
int create_account(AccountList *list, const char *holder_name, double initial_balance, int *created_account_number);

// Closes an account by changing its status to ACCOUNT_CLOSED.
int close_account(AccountList *list, int account_number);

// Removes an account node from the linked list.
int remove_account(AccountList *list, int account_number);

// Frees all nodes and resets the account list.
void free_account_list(AccountList *list);

#endif