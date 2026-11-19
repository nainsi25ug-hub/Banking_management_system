#ifndef BANK_H
#define BANK_H

/* Shared account status used by all project modules. */
typedef enum {
    ACCOUNT_ACTIVE,
    ACCOUNT_CLOSED
} AccountStatus;

/* Data belonging to one bank account. */
typedef struct Account {
    int account_number;
    char holder_name[50];
    double balance;
    AccountStatus status;
} Account;

/* One linked-list node stores one Account and a pointer to the next node. */
typedef struct Node {
    Account data;
    struct Node *next;
} Node;

/* The linked list is managed through its head pointer and account count. */
typedef struct {
    Node *head;
    int total_accounts;
} AccountList;

#endif
 