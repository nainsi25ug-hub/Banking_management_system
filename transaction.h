#ifndef TRANSACTION_H
#define TRANSACTION_H
#include "bank.h"
Node *find_account(AccountList *list, int account_number);
int deposit(AccountList *list, int account_number, double amount);
int withdraw(AccountList *list, int account_number, double amount);
int transfer(AccountList *list, int source_id, int destination_id, double amount);
#endif
