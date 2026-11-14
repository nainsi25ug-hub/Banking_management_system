#ifndef BANKING_H
#define BANKING_H

typedef enum {
    ACCOUNT_ACTIVE,
    ACCOUNT_CLOSED
} AccountStatus;

typedef struct Account {
    int account_number;
    char holder_name[50];
    double balance;
    AccountStatus status;
} Account;

typedef struct Node {
    Account data;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    int total_accounts;
} AccountList;

#endif /* BANKING_H */