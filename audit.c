#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include "audit.h"

static int today_string(char *buffer, size_t size)
{
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    return local != NULL && strftime(buffer, size, "%Y-%m-%d", local) > 0;
}

void audit_log_transaction(const char *type, int source_id, int destination_id,
                           double amount, int success, const char *message)
{
    char date[16], filename[64], timestamp[32];
    FILE *fp;
    time_t now;
    struct tm *local;
    if (type == NULL || message == NULL || !today_string(date, sizeof(date))) return;
    (void)snprintf(filename, sizeof(filename), "transactions_%s.csv", date);
    fp = fopen(filename, "a");
    if (fp == NULL) return;
    if (ftell(fp) == 0L) (void)fprintf(fp, "timestamp,type,source_account,destination_account,amount,status,message\n");
    now = time(NULL);
    local = localtime(&now);
    if (local == NULL || strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", local) == 0)
        (void)snprintf(timestamp, sizeof(timestamp), "%s", date);
    (void)fprintf(fp, "%s,%s,%d,%d,%.2f,%s,\"%s\"\n", timestamp, type,
                  source_id, destination_id, amount, success ? "SUCCESS" : "FAILED", message);
    (void)fclose(fp);
}

int generate_daily_audit_report(void)
{
    char date[16], filename[64], line[512];
    FILE *fp;
    int total = 0, successful = 0, failed = 0;
    if (!today_string(date, sizeof(date))) return -1;
    (void)snprintf(filename, sizeof(filename), "transactions_%s.csv", date);
    fp = fopen(filename, "r");
    if (fp == NULL) { printf("No transactions have been logged today.\n"); return 0; }
    (void)fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp) != NULL) {
        total++;
        if (strstr(line, ",SUCCESS,") != NULL) successful++;
        else if (strstr(line, ",FAILED,") != NULL) failed++;
    }
    (void)fclose(fp);
    printf("\n========== END-OF-DAY AUDIT REPORT =========\nDate: %s\nTotal transactions: %d\nSuccessful: %d\nFailed: %d\nLog: %s\n============================================\n",
           date, total, successful, failed, filename);
    return 0;
}
