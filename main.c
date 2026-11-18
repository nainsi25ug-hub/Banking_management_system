#include <stdio.h>
#include "bank.h"
#include "file_persistence.h"
#include "linked_list.h"
#include "ui.h"

int main(void)
{
    AccountList accounts;
    initialize_account_list(&accounts);
    if (load_accounts_from_file(&accounts, "accounts.dat") != 0) {
        fprintf(stderr, "Error loading accounts.dat; check the data file.\n");
        free_account_list(&accounts);
        return 1;
    }
    printf("Loaded %d account(s).\n", accounts.total_accounts);
    run_main_menu(&accounts);
    if (save_accounts_to_file(&accounts, "accounts.dat") != 0) {
        fprintf(stderr, "Error saving accounts.dat.\n");
        free_account_list(&accounts);
        return 1;
    }
    free_account_list(&accounts);
    puts("Accounts saved. Goodbye.");
    return 0;
}
