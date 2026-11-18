#ifndef BANK_H
#define BANK_H

typedef enum { ACCOUNT_ACTIVE = 0, ACCOUNT_CLOSED = 1 } AccountStatus;
typedef struct Account {
    int account_number;
    char holder_name[50];
    double balance;
    AccountStatus status;
} Account;
typedef struct Node { Account data; struct Node *next; } Node;
typedef struct { Node *head; int total_accounts; } AccountList;
#endif
