#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cctype>
#include <windows.h>
using namespace std;


enum AccountType {
    CASH,
    DEBIT_CARD,
    CREDIT_CARD
};


struct Date {
    int day = 0;
    int month = 0;
    int year = 0;
};


struct Card {
    string number = "None";
    Date expiry;
};


struct Category {
    int id = 0;
    string name = "Other";
};


struct Expense {
    int id = 0;
    double amount = 0;
    Date date;
    Category category;
    string description = "None";
};


struct Operation {
    int id = 0;
    Date date;
    string type = "None";
    double amount = 0;
    string category = "Other";
    string description = "None";
};


struct Account {
    int id = 0;
    string name = "None";
    AccountType type = CASH;
    double balance = 0;
    Card card;

    Category* categories = nullptr;
    int categorySize = 0;

    Expense* expenses = nullptr;
    int expenseSize = 0;

    Operation* operations = nullptr;
    int operationSize = 0;
};


// Accounts
void addAccount(Account*& accounts, int& size, string const name, int const type, string const card);
void showAccounts(Account* accounts, int size);
void showAccount(Account* accounts, int size, int id);
void editAccount(Account* accounts, int size, int id, string name, string card);
void deleteAccount(Account*& accounts, int& size, int id);


// Categories
void addCategory(Account& account, string const newcategory);
void showCategories(Account& account);
void deleteCategory(Account& account, int cId);


// Expenses
void addExpense(Account& account, double amount, string const categName);
void editExpense(Account& account, int id, double amount, string const categName, string description);
void deleteExpense(Account& account, int id);


// Operations
void deposit(Account& account, int money);
void showOperations(Account& account);


// Search and filtering
void showAllOperations(Account* accounts, int accountSize);
void filterByCategory(Account* accounts, int accountSize, string const categoryname);
void filterByDate(Account* accounts, int accountSize, Date const from, Date const to);
void searchByDescription(Account* accounts, int accountSize, string const description);


// Reports
void showReport(Account* account, int accountSize);
void topExpenses(Account* account, int accountSize);
void topCategories(Account* account, int accountSize);