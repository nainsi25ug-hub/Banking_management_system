#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

void initialize_account_list(AccountList *list)
{
    if (list != NULL) { list->head = NULL; list->total_accounts = 0; }
}

Node *find_account_node(const AccountList *list, int account_number)
{
    Node *current;
    if (list == NULL) return NULL;
    for (current = list->head; current != NULL; current = current->next)
        if (current->data.account_number == account_number) return current;
    return NULL;
}

Node *append_account(AccountList *list, const Account *account)
{
    Node *node, *tail;
    if (list == NULL || account == NULL || account->account_number <= 0 ||
        memchr(account->holder_name, '\0', sizeof(account->holder_name)) == NULL ||
        account->holder_name[0] == '\0' || !isfinite(account->balance) || account->balance < 0.0 ||
        (account->status != ACCOUNT_ACTIVE && account->status != ACCOUNT_CLOSED) ||
        find_account_node(list, account->account_number) != NULL) return NULL;
    node = malloc(sizeof(*node));
    if (node == NULL) return NULL;
    node->data = *account;
    node->next = NULL;
    if (list->head == NULL) list->head = node;
    else {
        for (tail = list->head; tail->next != NULL; tail = tail->next) { }
        tail->next = node;
    }
    list->total_accounts++;
    return node;
}

int create_account(AccountList *list, const char *name, double balance, int *created_id)
{
    Account account = {0};
    Node *current;
    int id = 1001;
    size_t length;
    if (list == NULL || name == NULL || !isfinite(balance) || balance < 0.0) return 0;
    length = strlen(name);
    if (length == 0 || length >= sizeof(account.holder_name) || strspn(name, " \t\r\n") == length) return 0;
    for (current = list->head; current != NULL; current = current->next) {
        if (current->data.account_number >= id) {
            if (current->data.account_number == INT_MAX) return 0;
            id = current->data.account_number + 1;
        }
    }
    account.account_number = id;
    memcpy(account.holder_name, name, length + 1);
    account.balance = balance;
    account.status = ACCOUNT_ACTIVE;
    if (append_account(list, &account) == NULL) return 0;
    if (created_id != NULL) *created_id = id;
    return 1;
}

int close_account(AccountList *list, int id)
{
    Node *node = find_account_node(list, id);
    if (node == NULL || node->data.status == ACCOUNT_CLOSED) return 0;
    node->data.status = ACCOUNT_CLOSED;
    return 1;
}

int remove_account(AccountList *list, int id)
{
    Node *current, *previous = NULL;
    if (list == NULL) return 0;
    for (current = list->head; current != NULL; previous = current, current = current->next) {
        if (current->data.account_number == id) {
            if (previous == NULL) list->head = current->next;
            else previous->next = current->next;
            free(current);
            list->total_accounts--;
            return 1;
        }
    }
    return 0;
}

void free_account_list(AccountList *list)
{
    Node *current, *next;
    if (list == NULL) return;
    for (current = list->head; current != NULL; current = next) {
        next = current->next;
        free(current);
    }
    list->head = NULL;
    list->total_accounts = 0;
}
