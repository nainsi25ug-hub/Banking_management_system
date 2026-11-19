#include <limits.h>   /* INT_MAX, used for overflow checks. */
#include <math.h>     /* isfinite(), used to reject NaN and infinity. */
#include <stdlib.h>   /* malloc() and free(). */
#include <string.h>   /* strlen(), memcpy(), memchr(), strspn(). */
#include "linked_list.h"

/* Sets the list to an empty state: no nodes and zero accounts. */
void initialize_account_list(AccountList *list)
{
    if (list != NULL) { list->head = NULL; list->total_accounts = 0; }
}

/* Walks through the list and returns the node with the given account number.
   Returns NULL if the list is invalid or no such account exists. */
Node *find_account_node(AccountList *list, int account_number)
{
    Node *current;

    if (list == NULL) return NULL;

    /* Start at the head and follow the next pointers until NULL. */
    for (current = list->head; current != NULL; current = current->next)
        if (current->data.account_number == account_number) return current;

    return NULL;
}

/* Validates an Account, allocates a new node with malloc() and adds it at the
   end of the list. Returns the new node, or NULL if validation or malloc fails. */
Node *append_account(AccountList *list, const Account *account)
{
    Node *node, *tail;

    /* Reject bad input: NULL pointers, invalid ID, name without '\0' or empty,
       NaN/negative balance, unknown status, or an ID that already exists. */
    if (list == NULL || account == NULL || list->total_accounts == INT_MAX || account->account_number <= 0 ||
        memchr(account->holder_name, '\0', sizeof(account->holder_name)) == NULL ||
        account->holder_name[0] == '\0' || !isfinite(account->balance) || account->balance < 0.0 ||
        (account->status != ACCOUNT_ACTIVE && account->status != ACCOUNT_CLOSED) ||
        find_account_node(list, account->account_number) != NULL) return NULL;

    /* Allocate memory for one new node. */
    node = malloc(sizeof(*node));
    if (node == NULL) return NULL;

    /* Copy the account data into the node; it will be the last node, so next is NULL. */
    node->data = *account;
    node->next = NULL;

    if (list->head == NULL) list->head = node;   /* Empty list: new node becomes the head. */
    else {
        /* Otherwise move to the last node and link the new node after it. */
        for (tail = list->head; tail->next != NULL; tail = tail->next) { }
        tail->next = node;
    }

    list->total_accounts++;
    return node;
}

/* Creates a new active account with the next free account number (starting at 1001)
   and appends it to the list. Returns 1 on success and 0 on failure.
   The new account number is stored in *created_id when created_id is not NULL. */
int create_account(AccountList *list, const char *name, double balance, int *created_id)
{
    Account account = {0};
    Node *current;
    int id = 1001;
    size_t length;

    /* Validate the list, the name pointer and the initial balance. */
    if (list == NULL || name == NULL || !isfinite(balance) || balance < 0.0) return 0;

    /* The name must fit in holder_name[50] and must not be empty or only spaces. */
    length = strlen(name);
    if (length == 0 || length >= sizeof(account.holder_name) || strspn(name, " \t\r\n") == length) return 0;

    /* The new ID is one more than the highest existing ID, so IDs are never reused. */
    for (current = list->head; current != NULL; current = current->next) {
        if (current->data.account_number >= id) {
            if (current->data.account_number == INT_MAX) return 0;   /* No larger ID is possible. */
            id = current->data.account_number + 1;
        }
    }

    /* Fill in the account details. */
    account.account_number = id;
    memcpy(account.holder_name, name, length + 1);   /* +1 also copies the '\0'. */
    account.balance = balance;
    account.status = ACCOUNT_ACTIVE;

    if (append_account(list, &account) == NULL) return 0;
    if (created_id != NULL) *created_id = id;
    return 1;
}

/* Soft delete: marks the account as closed but keeps the node in the list.
   Returns 1 on success, 0 if the account is not found or is already closed. */
int close_account(AccountList *list, int id)
{
    Node *node = find_account_node(list, id);

    if (node == NULL || node->data.status == ACCOUNT_CLOSED) return 0;

    node->data.status = ACCOUNT_CLOSED;
    return 1;
}

/* Hard delete: unlinks the node with the given account number and frees its memory.
   Returns 1 if a node was removed, 0 otherwise. */
int remove_account(AccountList *list, int id)
{
    Node *current, *previous = NULL;

    if (list == NULL) return 0;

    /* previous always points to the node just before current. */
    for (current = list->head; current != NULL; previous = current, current = current->next) {
        if (current->data.account_number == id) {
            if (previous == NULL) list->head = current->next;   /* Removing the first node. */
            else previous->next = current->next;                /* Skip over the removed node. */

            free(current);
            list->total_accounts--;
            return 1;
        }
    }

    return 0;
}

/* Frees every node in the list and resets it to the empty state (no memory leaks). */
void free_account_list(AccountList *list)
{
    Node *current, *next;

    if (list == NULL) return;

    for (current = list->head; current != NULL; current = next) {
        next = current->next;   /* Save next first, because current is freed right after. */
        free(current);
    }

    list->head = NULL;
    list->total_accounts = 0;
}