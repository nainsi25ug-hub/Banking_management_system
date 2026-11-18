#ifndef UI_H
#define UI_H

#include <stdbool.h>
#include <stddef.h>
#include "bank.h"

bool read_line(const char *prompt, char *buffer, size_t size);
bool read_int(const char *prompt, int *out_value);
bool read_double(const char *prompt, double *out_value);

void print_active_accounts(const AccountList *list);
void print_account_details(const Account *acc);

void run_account_menu(AccountList *list, Account *acc);
void run_main_menu(AccountList *list);

#endif /* UI_H */
