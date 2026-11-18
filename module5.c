#include <ctype.h>
#include <stdio.h>
#include "module5.h"
#include "audit.h"

Node *find_account_by_id(AccountList *list, int id)
{
    Node *current;
    if (list == NULL) return NULL;
    for (current = list->head; current != NULL; current = current->next)
        if (current->data.account_number == id) return current;
    return NULL;
}

static int contains_case_insensitive(const char *text, const char *query)
{
    size_t i, j;
    if (text == NULL || query == NULL || query[0] == '\0') return 0;
    for (i = 0; text[i] != '\0'; i++) {
        for (j = 0; query[j] != '\0' && text[i + j] != '\0'; j++)
            if (tolower((unsigned char)text[i + j]) != tolower((unsigned char)query[j])) break;
        if (query[j] == '\0') return 1;
    }
    return 0;
}

void search_accounts_by_name(const AccountList *list, const char *name)
{
    const Node *current;
    int count = 0;
    if (list == NULL || name == NULL || name[0] == '\0') { puts("Enter a valid name."); return; }
    printf("\n%-12s | %-32s | %12s | %-8s\n", "Account ID", "Account Holder", "Balance", "Status");
    puts("--------------------------------------------------------------------------");
    for (current = list->head; current != NULL; current = current->next) {
        if (contains_case_insensitive(current->data.holder_name, name)) {
            printf("%-12d | %-32s | %12.2f | %-8s\n", current->data.account_number,
                   current->data.holder_name, current->data.balance,
                   current->data.status == ACCOUNT_ACTIVE ? "ACTIVE" : "CLOSED");
            count++;
        }
    }
    if (count == 0) printf("No accounts matched '%s'.\n", name);
    else printf("%d matching account(s) found.\n", count);
}

int generate_end_of_day_report(void) { return generate_daily_audit_report(); }
