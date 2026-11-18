# Simple Banking Management System

A modular C command-line banking application integrating the five project modules.

## Build and run

```sh
make
./banking_app
```

Run `make clean` to remove object files and the executable. Account records are stored in `accounts.dat` (binary format). A daily CSV transaction log is created as `transactions_YYYY-MM-DD.csv` in the current working directory.

## Modules

1. **Linked List** (`linked_list.c/.h`): dynamically allocates, appends, searches, closes, removes and frees account nodes.
2. **File Persistence** (`file_persistence.c/.h`): loads accounts at startup and saves them on exit.
3. **Transaction Engine** (`transaction.c/.h`): deposit, withdrawal and atomic account-to-account transfer with validation.
4. **UI & Input Handler** (`bankingApplication.c`, `ui.h`): teller menus and safe line-based input parsing.
5. **Search & Reporting** (`module5.c/.h`, `audit.c/.h`): search by ID/name, daily transaction audit CSV, and end-of-day summary.

Account IDs start at 1001 and increase above the highest existing ID. Closing an account is a soft delete. Successful and rejected deposit, withdrawal and transfer attempts are logged.
