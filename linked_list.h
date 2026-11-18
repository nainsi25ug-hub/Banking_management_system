#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include "bank.h"
void initialize_account_list(AccountList *list);
Node *find_account_node(const AccountList *list, int account_number);
Node *append_account(AccountList *list, const Account *account);
int create_account(AccountList *list, const char *holder_name, double initial_balance, int *created_account_number);
int close_account(AccountList *list, int account_number);
int remove_account(AccountList *list, int account_number);
void free_account_list(AccountList *list);
#endif
