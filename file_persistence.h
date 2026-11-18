#ifndef FILE_PERSISTENCE_H
#define FILE_PERSISTENCE_H
#include "bank.h"
int load_accounts_from_file(AccountList *list, const char *filename);
int save_accounts_to_file(const AccountList *list, const char *filename);
#endif
