#include <stddef.h>
#include <float.h>
#include <math.h>
#include "transaction.h"
#include "linked_list.h"
#include "audit.h"

Node *find_account(AccountList *list, int id) { return find_account_node(list, id); }

int deposit(AccountList *list, int id, double amount)
{
    Node *account = find_account(list, id);
    int ok = account != NULL && account->data.status == ACCOUNT_ACTIVE &&
        isfinite(account->data.balance) && account->data.balance >= 0.0 &&
        isfinite(amount) && amount > 0.0 && amount <= DBL_MAX - account->data.balance;
    if (ok) account->data.balance += amount;
    audit_log_transaction("DEPOSIT", id, 0, amount, ok, ok ? "Deposit completed" : "Deposit rejected");
    return ok;
}

int withdraw(AccountList *list, int id, double amount)
{
    Node *account = find_account(list, id);
    int ok = account != NULL && account->data.status == ACCOUNT_ACTIVE &&
        isfinite(account->data.balance) && account->data.balance >= 0.0 &&
        isfinite(amount) && amount > 0.0 && account->data.balance >= amount;
    if (ok) account->data.balance -= amount;
    audit_log_transaction("WITHDRAW", id, 0, amount, ok, ok ? "Withdrawal completed" : "Withdrawal rejected");
    return ok;
}

int transfer(AccountList *list, int from, int to, double amount)
{
    Node *source = find_account(list, from), *destination = find_account(list, to);
    int ok = source != NULL && destination != NULL && source != destination &&
        source->data.status == ACCOUNT_ACTIVE && destination->data.status == ACCOUNT_ACTIVE &&
        isfinite(source->data.balance) && source->data.balance >= 0.0 &&
        isfinite(destination->data.balance) && destination->data.balance >= 0.0 &&
        isfinite(amount) && amount > 0.0 && source->data.balance >= amount &&
        amount <= DBL_MAX - destination->data.balance;
    if (ok) { source->data.balance -= amount; destination->data.balance += amount; }
    audit_log_transaction("TRANSFER", from, to, amount, ok, ok ? "Transfer completed" : "Transfer rejected");
    return ok;
}
