#ifndef MODULE5_H
#define MODULE5_H
#include "bank.h"
Node *find_account_by_id(AccountList *list, int account_number);
void search_accounts_by_name(const AccountList *list, const char *name);
int generate_end_of_day_report(void);
#endif
