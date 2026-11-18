#ifndef AUDIT_H
#define AUDIT_H
void audit_log_transaction(const char *type, int source_id, int destination_id,
                           double amount, int success, const char *message);
int generate_daily_audit_report(void);
#endif
