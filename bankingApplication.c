#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ui.h"
#include "transaction.h"
#include "linked_list.h"

static void clear_input_buffer(void) {
    int c=getchar(); 
    while (c!= '\n' && c != EOF) 
    {
        c=getchar();
    }
}

bool read_line(const char *prompt, char *buffer, size_t size) 
{
    if (buffer == NULL || size == 0) 
    {
        return false;
    }

    if (prompt != NULL) 
    {
        printf("%s", prompt);
        fflush(stdout);
    }

    if (fgets(buffer, (int)size, stdin) == NULL) 
    {
        buffer[0] = '\0';
        return false;
    }

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') 
    {
        buffer[len - 1] = '\0';
    } 
    else 
    {
        clear_input_buffer();
    }
    return true;
}

bool read_int(const char *prompt, int *out_value) 
{
    char buf[64];
    while (true) {
        if (!read_line(prompt, buf, sizeof(buf))) 
        {
            return false;
        }
        char *start = buf;
        while (isspace((unsigned char)*start)) 
        {
            start++;
        }
        if (*start == '\0') 
        {
            printf("Invalid input. Please enter a whole number.\n");
            continue;
        }

        errno = 0;
        char *endptr = NULL;
        long val = strtol(start, &endptr, 10);

        while (isspace((unsigned char)*endptr)) 
        {
            endptr++;
        }

        if (errno == ERANGE || *endptr != '\0') 
        {
            printf("Invalid number. Please enter a valid integer.\n");
            continue;
        }
        *out_value = (int)val;
        return true;
    }
}

bool read_double(const char *prompt, double *out_value) 
{
    char buf[64];
    while (true) 
    {
        if (!read_line(prompt, buf, sizeof(buf))) 
        {
            return false;
        }
        char *start = buf;
        while (isspace((unsigned char)*start)) 
        {
            start++;
        }
        if (*start == '\0') 
        {
            printf("Invalid input. Please enter a numeric amount.\n");
            continue;
        }
        errno = 0;
        char *endptr = NULL;
        double val = strtod(start, &endptr);
        while (isspace((unsigned char)*endptr)) 
        {
            endptr++;
        }
        if (errno == ERANGE || *endptr != '\0') 
        {
            printf("Invalid amount. Please enter a valid decimal number.\n");
            continue;
        }
        *out_value = val;
        return true;
    }
}

void print_active_accounts(const AccountList *list) 
{
    if (list == NULL || list->head == NULL) 
    {
        printf("\nNo accounts found in the system.\n");
        return;
    }
    printf("\n====================================================================\n");
    printf("%-12s | %-32s | %15s\n", "Account ID", "Account Holder", "Balance ($)");
    printf("--------------------------------------------------------------------\n");
    int active_count = 0;
    const Node *curr = list->head;
    while (curr != NULL) 
    {
        if (curr->data.status == ACCOUNT_ACTIVE) 
        {
            printf("%-12d | %-32s | %15.2f\n",
                   curr->data.account_number,
                   curr->data.holder_name,
                   curr->data.balance);
            active_count++;
        }
        curr = curr->next;
    }
    printf("====================================================================\n");
    if (active_count == 0) 
    {
        printf("No active accounts available.\n");
    } else 
    {
        printf("Total Active Accounts: %d\n", active_count);
    }
}
void print_account_details(const Account *acc) 
{
    if (acc == NULL) 
    {
        return;
    }
    const char *status_str = (acc->status == ACCOUNT_ACTIVE) ? "ACTIVE" : "CLOSED";
    printf("\n--- Account Details ---\n");
    printf("Account Number : %d\n", acc->account_number);
    printf("Holder Name    : %s\n", acc->holder_name);
    printf("Balance        : $%.2f\n", acc->balance);
    printf("Status         : %s\n", status_str);
    printf("-----------------------\n");
}

void run_account_menu(AccountList *list, Account *acc) 
{
    if (list == NULL || acc == NULL) 
    {
        return;
    }
    int choice = 0;
    while (true) 
    {
        printf("\n=== Account Operations (ID: %d | %s) ===\n", acc->account_number, acc->holder_name);
        printf("1. View Account Details\n");
        printf("2. Deposit Funds\n");
        printf("3. Withdraw Funds\n");
        printf("4. Transfer Funds (To another Account ID)\n");
        printf("5. Close Account (Soft Delete / Mark Inactive)\n");
        printf("6. Go back to Main Menu\n");
        if (!read_int("Select an option (1-6): ", &choice)) 
        {
            return;
        }

        switch (choice) 
        {
            case 1:
                print_account_details(acc);
                break;

            case 2: 
            {
                double amount = 0.0;
                if (read_double("Enter deposit amount: ", &amount)) 
                {
                    deposit(list, acc->account_number, amount);
                }
                break;
            }

            case 3: 
            {
                double amount = 0.0;
                if (read_double("Enter withdrawal amount: ", &amount)) 
                {
                    withdraw(list, acc->account_number, amount);
                }
                break;
            }

            case 4: 
            {
                int target_id = 0;
                double amount = 0.0;
                if (read_int("Enter destination Account ID: ", &target_id) &&
                    read_double("Enter transfer amount: ", &amount)) 
                {
                    if (transfer(list, acc->account_number, target_id, amount))
                    {
                        printf("Transfer successful. New balance: $%.2f\n", acc->balance);
                    }
                    else
                    {
                        printf("Transfer failed. Check the destination account, amount and funds. No balances were changed.\n");
                    }
                }
            }

            case 5: 
            {
                char confirm[16];
                if (read_line("Are you sure you want to close this account? (y/n): ",
                              confirm, sizeof(confirm))) 
                {
                    if (confirm[0] == 'y' || confirm[0] == 'Y') 
                    {
                        acc->status = ACCOUNT_CLOSED;
                        printf("Account %d marked as CLOSED.\n", acc->account_number);
                        return; 
                    }
                }
                break;
            }

            case 6:
                return;

            default:
                printf("Invalid selection. Choose a number between 1 and 6.\n");
                break;
        }
    }
}

void run_main_menu(AccountList *list) 
{
    int choice = 0;
    while (true) 
    {
        printf("\n================ MAIN TELLER MENU ================\n");
        printf("1. List All Active Accounts\n");
        printf("2. Create a New Account\n");
        printf("3. Select / Search Account\n");
        printf("4. Generate End-of-Day Audit Report\n");
        printf("5. Save & Exit\n");
        printf("==================================================\n");
        if (!read_int("Select an option (1-5): ", &choice)) 
        {
            printf("\nInput stream closed. Exiting...\n");
            break;
        }
        switch (choice) 
        {
            case 1:
                print_active_accounts(list);
                break;

            case 2: 
            {
                char name[50];
                double initial_deposit = 0.0;
                if (read_line("Enter customer full name: ", name, sizeof(name)) &&
                    read_double("Enter initial deposit: ", &initial_deposit)) 
                {
                    create_account(list, name, initial_deposit, &new_id)
                }
                break;
            }
            
            case 3: 
            {
                int search_id = 0;
                int mode = 0;
                if (!read_int("1. Select by Account ID\n2. Search by name\nChoose (1-2): ", &mode))
                {
                    break;
                }
                if (mode == 2)
                {
                    char query[50];
                    if (!read_line("Enter name to search: ", query, sizeof(query)))
                    {
                        break;
                    }
                    search_accounts_by_name(list, query);
                }
                else if (mode != 1)
                {
                    printf("Invalid choice.\n");
                    break;
                }
                if (read_int("Enter Account ID to select: ", &search_id)) 
                {
                    Node *found = find_account(list, search_id);
                    if (found == NULL)
                    {
                        printf("No account found with ID %d.\n", search_id);
                    }
                    else if (found->data.status == ACCOUNT_CLOSED)
                    {
                        printf("Account %d is closed.\n", search_id);
                    }
                    else
                    {
                        run_account_menu(list, &found->data);
                    }
                }
                break;
            }

            case 4:
                generate_end_of_day_report();
                break;

            case 5:
                printf("Saving accounts and exiting...\n");
                return;

            default:
                printf("Invalid selection. Choose a number between 1 and 5.\n");
                break;
        }
    }
}
