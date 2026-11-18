#include <float.h>
#include <math.h>

Node* find_account(AccountList *list, int account_number) //first helper function
{
    if (list == NULL)
    {
    return NULL;
    }
    Node *current = list->head;

    while (current != NULL)
    {
        if (current->data.account_number == account_number)
        {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

int deposit(AccountList *list, int account_number, double amount) //2nd function
{
    Node *current = find_account(list, account_number);

    if (current == NULL)
    {
        return 0;
    }

if (current->data.status == ACCOUNT_CLOSED)
{
    return 0;
}

if (!isfinite(current->data.balance) || current->data.balance < 0)
{
    return 0;
}

if (!isfinite(amount) || amount <= 0)
{
    return 0;
}

if (amount > DBL_MAX - current->data.balance)
{
    return 0;
}

current->data.balance += amount;
return 1;
}

int withdraw(AccountList *list, int account_number, double amount) //our third function
{
    Node *current = find_account(list, account_number);

    if (current == NULL)
    {
        return 0;
    }

if (current->data.status == ACCOUNT_CLOSED)
{
    return 0;
}

if (!isfinite(current->data.balance) || current->data.balance < 0)
{
    return 0;
}

if (!isfinite(amount) || amount <= 0)
{
    return 0;
}

if (current->data.balance < amount)
{
    return 0;
}

current->data.balance -= amount;
return 1;
}

int transfer(AccountList *list, int source_id, int destination_id, double amount) // our fourth func
{
    if (list == NULL)
    {
    return 0;
    }

    Node *source = find_account(list, source_id);
    Node *destination = find_account(list, destination_id);

    if (source == NULL || destination == NULL)
    {
        return 0;
    }

    if (source->data.status == ACCOUNT_CLOSED || destination->data.status == ACCOUNT_CLOSED)
    {
        return 0;
    }

if (!isfinite(source->data.balance) ||
    !isfinite(destination->data.balance) ||
    source->data.balance < 0 ||
    destination->data.balance < 0)
{
    return 0;
}

    if (source_id == destination_id)
    {
    return 0;
    }

    if (!isfinite(amount) || amount <= 0)
    {
    return 0;
    }

    if (source->data.balance < amount)
    {
        return 0;
    }

    if (amount > DBL_MAX - destination->data.balance)
    {   
    return 0;
    }


    source->data.balance -= amount;
    destination->data.balance += amount;

    return 1;
}