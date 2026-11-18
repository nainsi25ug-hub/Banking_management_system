#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"
#include "linked_list.h"
#include "transaction.h"
#include "module5.h"
#include "audit.h"

bool read_line(const char *prompt, char *buffer, size_t size)
{
    size_t length;
    int ch;
    if (buffer == NULL || size < 2) return false;
    if (prompt != NULL) { printf("%s", prompt); (void)fflush(stdout); }
    if (fgets(buffer, (int)size, stdin) == NULL) { buffer[0] = '\0'; return false; }
    length = strlen(buffer);
    if (length > 0 && buffer[length - 1] == '\n') buffer[length - 1] = '\0';
    else while ((ch = getchar()) != '\n' && ch != EOF) { }
    return true;
}

bool read_int(const char *prompt, int *out)
{
    char buffer[80], *end;
    long value;
    if (out == NULL) return false;
    for (;;) {
        if (!read_line(prompt, buffer, sizeof(buffer))) return false;
        errno = 0;
        value = strtol(buffer, &end, 10);
        while (isspace((unsigned char)*end)) end++;
        if (end != buffer && *end == '\0' && errno != ERANGE && value >= INT_MIN && value <= INT_MAX) {
            *out = (int)value;
            return true;
        }
        puts("Please enter a valid whole number.");
    }
}

bool read_double(const char *prompt, double *out)
{
    char buffer[80], *end;
    double value;
    if (out == NULL) return false;
    for (;;) {
        if (!read_line(prompt, buffer, sizeof(buffer))) return false;
        errno = 0;
        value = strtod(buffer, &end);
        while (isspace((unsigned char)*end)) end++;
        if (end != buffer && *end == '\0' && errno != ERANGE && isfinite(value)) {
            *out = value;
            return true;
        }
        puts("Please enter a valid finite amount.");
    }
}

static void show_account(const Account *account)
{
    if (account == NULL) return;
    printf("\nAccount ID : %d\nHolder     : %s\nBalance    : %.2f\nStatus     : %s\n",
           account->account_number, account->holder_name, account->balance,
           account->status == ACCOUNT_ACTIVE ? "ACTIVE" : "CLOSED");
}

static void list_accounts(const AccountList *list)
{
    const Node *current;
    int count = 0;
    puts("\n---------------- ACTIVE ACCOUNTS ----------------");
    printf("%-12s %-32s %14s\n", "Account ID", "Account Holder", "Balance");
    for (current = list->head; current != NULL; current = current->next) {
        if (current->data.status == ACCOUNT_ACTIVE) {
            printf("%-12d %-32.32s %14.2f\n", current->data.account_number,
                   current->data.holder_name, current->data.balance);
            count++;
        }
    }
    printf("Active accounts: %d\n", count);
}

static void account_menu(AccountList *list, int id)
{
    int choice;
    for (;;) {
        Node *node = find_account_by_id(list, id);
        if (node == NULL) return;
        printf("\n=== ACCOUNT MENU: %d (%s) ===\n1. View details\n2. Deposit\n3. Withdraw\n4. Transfer\n5. Close account\n6. Back\n",
               id, node->data.holder_name);
        if (!read_int("Choice: ", &choice)) return;
        if (choice == 1) show_account(&node->data);
        else if (choice == 2 || choice == 3) {
            double amount;
            if (!read_double("Amount: ", &amount)) return;
            if (choice == 2 ? deposit(list, id, amount) : withdraw(list, id, amount)) puts("Transaction successful.");
            else puts("Transaction failed. Check account status, balance and amount.");
        } else if (choice == 4) {
            int target;
            double amount;
            if (!read_int("Destination account ID: ", &target) || !read_double("Amount: ", &amount)) return;
            if (transfer(list, id, target, amount)) puts("Transfer successful.");
            else puts("Transfer failed. No balances were changed.");
        } else if (choice == 5) {
            char confirm[8];
            if (!read_line("Close this account? (y/n): ", confirm, sizeof(confirm))) return;
            if (confirm[0] == 'y' || confirm[0] == 'Y') {
                if (close_account(list, id)) {
                    audit_log_transaction("CLOSE", id, 0, 0.0, 1, "Account closed");
                    puts("Account closed.");
                    return;
                }
                puts("Unable to close account.");
            }
        } else if (choice == 6) return;
        else puts("Invalid choice.");
    }
}

static void search_menu(AccountList *list)
{
    int choice, id;
    char name[50];
    Node *found;
    puts("\nSearch by: 1. Account ID  2. Holder name");
    if (!read_int("Choice: ", &choice)) return;
    if (choice == 1) {
        if (!read_int("Account ID: ", &id)) return;
        found = find_account_by_id(list, id);
        if (found == NULL) puts("Account not found.");
        else if (found->data.status == ACCOUNT_CLOSED) puts("This account is closed.");
        else { show_account(&found->data); account_menu(list, id); }
    } else if (choice == 2) {
        if (read_line("Enter full or partial holder name: ", name, sizeof(name)))
            search_accounts_by_name(list, name);
    } else puts("Invalid search option.");
}

void run_main_menu(AccountList *list)
{
    int choice;
    for (;;) {
        puts("\n========== MAIN TELLER MENU ==========\n1. List active accounts\n2. Create account\n3. Search / select account\n4. Generate end-of-day audit report\n5. Save and exit");
        if (!read_int("Choice: ", &choice)) return;
        if (choice == 1) list_accounts(list);
        else if (choice == 2) {
            char name[50];
            double initial;
            int id;
            if (!read_line("Customer full name: ", name, sizeof(name)) ||
                !read_double("Initial deposit (0 or more): ", &initial)) return;
            if (create_account(list, name, initial, &id)) {
                printf("Account created. New account ID: %d\n", id);
                audit_log_transaction("OPEN", id, 0, initial, 1, "Account created");
            } else puts("Could not create account. Check name and initial deposit.");
        } else if (choice == 3) search_menu(list);
        else if (choice == 4) {
            if (generate_end_of_day_report() != 0) puts("Could not generate audit report.");
        } else if (choice == 5) return;
        else puts("Invalid choice.");
    }
}
