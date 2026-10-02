#include "Finance.h"

using namespace std;
//2. Операція поповнення
void deposit(Account& account, int money) {
    if (money <= 0) {
        throw "Некоректне значення";
    }

    account.balance += money;

    time_t now = time(0);
    tm local;
    localtime_s(&local, &now);

    Date date;
    date.day = local.tm_mday;
    date.month = local.tm_mon + 1;
    date.year = local.tm_year + 1900;

    Operation* temp = new Operation[account.operationSize + 1];
    int id = 1;
    for (int i = 0; i < account.operationSize; i++) {
        if (account.operations[i].id >= id) {
            id = account.operations[i].id + 1;
        }
    }
    for (int i = 0; i < account.operationSize; i++) {
        temp[i] = account.operations[i];
    }

    temp[account.operationSize].id = id;
    temp[account.operationSize].date = date;
    temp[account.operationSize].type = "Поповнення";
    temp[account.operationSize].amount = money;
    temp[account.operationSize].description = "Поповнення рахунку";

    delete[] account.operations;
    account.operations = temp;
    account.operationSize++;
}
//3. Категорії
void addCategory(Account& account, string const newcategory) {
    int id = 1;

    Category* temp = new Category[account.categorySize + 1];

    for (int i = 0; i < account.categorySize; i++) {
        temp[i] = account.categories[i];
    }

    for (int i = 0; i < account.categorySize; i++) {
        if (account.categories[i].id >= id) {
            id = account.categories[i].id + 1;
        }
    }

    temp[account.categorySize].id = id;
    temp[account.categorySize].name = newcategory;

    delete[] account.categories;
    account.categories = temp;
    account.categorySize++;
}
void showCategories(Account& account) {
    for (int i = 0; i < account.categorySize; i++) {
        cout << "Id: " << account.categories[i].id << "\n";
        cout << "Категорія: " << account.categories[i].name << endl;
    }
}
void deleteCategory(Account& account, int const cId) {
    int count = 0;
    bool found = false;
    if (account.categorySize == 0) {
        throw "Категорій немає, видалення не можливе";
    }
    Category* temp = new Category[account.categorySize - 1];
    for (int i = 0; i < account.categorySize; i++) {
        if (account.categories[i].id == cId) {
            found = true;
        }
    }
    if (!found) {
        throw "Категорії під цим індексом не має";
    }
    for (int i = 0; i < account.categorySize; i++) {
        if (account.categories[i].id == cId) {
            continue;
        }
        else {
            temp[count] = account.categories[i];
            count++;
        }
    }
    
    delete[] account.categories;
    account.categories = temp;
    account.categorySize--;
}
//4,5 Витрати
void addExpense(Account& account, double amount, string const categName) {
    if (amount <= 0) { 
        throw "Сума повинна бути більшою за 0"; 
    }
    if (amount > account.balance) { 
        throw "Недостатньо коштів на рахунку"; 
    }

    bool found = false;
    for (int i = 0; i < account.categorySize; i++) {
        if (account.categories[i].name == categName) {
            found = true;
            break;
        }
    }
    if (!found) { 
        throw "Категорію не знайдено"; 
    }

    Expense* temp = new Expense[account.expenseSize + 1];

    for (int i = 0; i < account.expenseSize; i++) {
        temp[i] = account.expenses[i];
    }

    temp[account.expenseSize].id = account.expenseSize + 1;

    time_t now = time(0);
    tm date;
    localtime_s(&date, &now);

    temp[account.expenseSize].date.day = date.tm_mday;
    temp[account.expenseSize].date.month = date.tm_mon + 1;
    temp[account.expenseSize].date.year = date.tm_year + 1900;
    temp[account.expenseSize].amount = amount;

    for (int i = 0; i < account.categorySize; i++) {
        if (account.categories[i].name == categName) {
            temp[account.expenseSize].category = account.categories[i];
            break;
        }
    }

    temp[account.expenseSize].description = "Витрати на " + categName;

    delete[] account.expenses;
    account.expenses = temp;
    account.expenseSize++;
    account.balance -= amount;

    Operation* tempOperations = new Operation[account.operationSize + 1];

    for (int i = 0; i < account.operationSize; i++) {
        tempOperations[i] = account.operations[i];
    }

    tempOperations[account.operationSize].id = account.operationSize + 1;
    tempOperations[account.operationSize].date = temp[account.expenseSize - 1].date;
    tempOperations[account.operationSize].type = "Витрати";
    tempOperations[account.operationSize].amount = amount;
    tempOperations[account.operationSize].category = categName;
    tempOperations[account.operationSize].description = "Витрати на " + categName;

    delete[] account.operations;
    account.operations = tempOperations;
    account.operationSize++;
}
void editExpense(Account& account, int id, double amount, string const categName, string description) {
    bool found = false;
    int expenseIndex = 0;
    double oldExpenseAmount = 0;

    for (int i = 0; i < account.expenseSize; i++) {
        if (account.expenses[i].id == id) {
            found = true;
            expenseIndex = i;
            oldExpenseAmount = account.expenses[i].amount;
            break;
        }
    }
    if (!found) {
        throw "Витрати з таким ID не існує";
    }
    if (amount <= 0) {
        throw "Некоректне значення";
    }
    double newBalance = account.balance + oldExpenseAmount - amount;
    if (newBalance < 0) {
        throw "Недостатньо коштів";
    }
    bool found2 = false;
    Category newCategory;
    for (int i = 0; i < account.categorySize; i++) {
        if (account.categories[i].name == categName) {
            newCategory = account.categories[i];
            found2 = true;
            break;
        }
    }
    if (!found2) {
        throw "Категорії з такою назвою не існує";
    }
    Expense* temp = new Expense[account.expenseSize];
    for (int i = 0; i < account.expenseSize; i++) {
        temp[i] = account.expenses[i];
    }
    temp[expenseIndex].amount = amount;
    temp[expenseIndex].category = newCategory;
    temp[expenseIndex].description = description;
    account.balance = newBalance;
    delete[] account.expenses;
    account.expenses = temp;
}
void deleteExpense(Account& account, int const eId) {
    int count = 0;
    bool found = false;
    if (account.expenseSize == 0) {
        throw "Витрат немає, видалення не можливе";
    }
    Expense* temp = new Expense[account.expenseSize - 1];
    for (int i = 0; i < account.expenseSize; i++) {
        if (account.expenses[i].id == eId) {
            found = true;
            account.balance += account.expenses[i].amount;
        }
    }
    if (!found) {
        delete[] temp;
        throw "Витрати під цим індексом не має";
    }
    for (int i = 0; i < account.expenseSize; i++) {
        if (account.expenses[i].id == eId) {
            continue;
        }
        else {
            temp[count] = account.expenses[i];
            count++;
        }
    }

    delete[] account.expenses;
    account.expenses = temp;
    account.expenseSize--;
}
//6. Історія операцій та пошук
void showOperations(Account& account) {
    cout << "Рахунок: " << account.name << "\n";
    for (int i = 0; i < account.operationSize; i++) {
        cout << "Id: " << account.operations[i].id << "\n";
        cout << "Дата: " << account.operations[i].date.day << "." << account.operations[i].date.month << "." << account.operations[i].date.year << "\n";
        cout << "Тип: " << account.operations[i].type << "\n";
        cout << "Кількість: " << account.operations[i].amount << "\n";
        cout << "Категорія: " << account.operations[i].category << "\n";
        cout << "Опис: " << account.operations[i].description << endl;
    }
}
void showAllOperations(Account* accounts, int accountSize) {
    for (int i = 0; i < accountSize; i++) {
        cout << "Рахунок: " << accounts[i].name << "\n";
        for (int j = 0; j < accounts[i].operationSize; j++) {
            cout << "Id: " << accounts[i].operations[j].id << "\n";
            cout << "Дата: " << accounts[i].operations[j].date.day << "." << accounts[i].operations[j].date.month << "." << accounts[i].operations[j].date.year << "\n";
            cout << "Тип: " << accounts[i].operations[j].type << "\n";
            cout << "Кількість: " << accounts[i].operations[j].amount << "\n";
            cout << "Категорія: " << accounts[i].operations[j].category << "\n";
            cout << "Опис: " << accounts[i].operations[j].description << endl;
        }
    }
}
void filterByCategory(Account* accounts, int accountSize, string const categoryname) {
    double sum = 0;
    bool found = false;
    for (int i = 0; i < accountSize; i++) {

        for (int j = 0; j < accounts[i].categorySize; j++) {
            if (accounts[i].categories[j].name == categoryname) {
                cout << "Id категорії: " << accounts[i].categories[j].id << "\n";
                found = true;
                break;
            }
        }
        if (found) {
            for (int j = 0; j < accounts[i].operationSize; j++) {
                if (accounts[i].operations[j].category == categoryname) {
                    cout << "Витрата: " << accounts[i].operations[j].amount << endl;
                    sum += accounts[i].operations[j].amount;
                }
            }
        }
    }
    if (!found) {
        throw "Такої категорії не існує";
    }
    if (sum == 0) {
        throw "У цієї категорії немає витрат";
    }
    cout << "Всі витрати на категорію: " << sum << endl;
}
void filterByDate(Account* accounts, int accountSize, Date const from, Date const to) {
    bool foundAny = false;
    for (int i = 0; i < accountSize; i++) {
        bool found = false;
        for (int j = 0; j < accounts[i].operationSize; j++) {
            if (accounts[i].operations[j].date.year > from.year || (accounts[i].operations[j].date.year == from.year && accounts[i].operations[j].date.month > from.month) || (accounts[i].operations[j].date.year == from.year && accounts[i].operations[j].date.month == from.month && accounts[i].operations[j].date.day >= from.day)) {
                if (accounts[i].operations[j].date.year < to.year || (accounts[i].operations[j].date.year == to.year && accounts[i].operations[j].date.month < to.month) || (accounts[i].operations[j].date.year == to.year && accounts[i].operations[j].date.month == to.month && accounts[i].operations[j].date.day <= to.day)) {
                    if (!found) {
                        cout << "Рахунок: " << accounts[i].name << "\n";
                        found = true;
                        foundAny = true;
                    }
                    cout << "Id: " << accounts[i].operations[j].id << "\n";
                    cout << "Дата: " << accounts[i].operations[j].date.day << "." << accounts[i].operations[j].date.month << "." << accounts[i].operations[j].date.year << "\n";
                    cout << "Тип: " << accounts[i].operations[j].type << "\n";
                    cout << "Кількість: " << accounts[i].operations[j].amount << "\n";
                    cout << "Категорія: " << accounts[i].operations[j].category << "\n";
                    cout << "Опис: " << accounts[i].operations[j].description << endl;
                }
            }
        }
    }
    if (!foundAny) {
        throw "У цьому періоді не було здійснено жодної операції";
    }
}
void searchByDescription(Account* accounts, int accountSize, string const description) {
    bool found = false;

    for (int i = 0; i < accountSize; i++) {
        for (int j = 0; j < accounts[i].operationSize; j++) {
            if (accounts[i].operations[j].description == description) {
                if (!found) {
                    cout << "Рахунок: " << accounts[i].name << "\n";
                    found = true;
                }
                cout << "Id: " << accounts[i].operations[j].id << "\n";
                cout << "Дата: " << accounts[i].operations[j].date.day << "." << accounts[i].operations[j].date.month << "." << accounts[i].operations[j].date.year << "\n";
                cout << "Тип: " << accounts[i].operations[j].type << "\n";
                cout << "Кількість: " << accounts[i].operations[j].amount << "\n";
                cout << "Категорія: " << accounts[i].operations[j].category << "\n";
                cout << accounts[i].operations[j].description << endl;
            }
        }
    }

    if (!found) {
        throw "Операції з таким описом не знайдено";
    }
}
//7,8,9 Звіти та рейтинги
void showReport(Account* accounts, int accountSize) {
    time_t now = time(0);
    tm local;
    localtime_s(&local, &now);

    Date to;
    to.day = local.tm_mday;
    to.month = local.tm_mon + 1;
    to.year = local.tm_year + 1900;

    Date from = to;

    // ==================== ЗВІТ ЗА ДЕНЬ ====================
    cout << "\n========== ЗВІТ ЗА ДЕНЬ ==========\n";

    double total = 0;
    int count = 0;
    double avg = 0;
    bool first = true;
    int maxAccount = 0;
    int maxIndex = 0;
    int minAccount = 0;
    int minIndex = 0;

    for (int i = 0; i < accountSize; i++) {
        for (int j = 0; j < accounts[i].operationSize; j++) {
            Date operationDate = accounts[i].operations[j].date;

            bool afterFrom = false;
            bool beforeTo = false;

            if (operationDate.year > from.year) {
                afterFrom = true;
            }
            else if (operationDate.year == from.year) {
                if (operationDate.month > from.month) {
                    afterFrom = true;
                }
                else if (operationDate.month == from.month && operationDate.day >= from.day) {
                    afterFrom = true;
                }
            }

            if (operationDate.year < to.year) {
                beforeTo = true;
            }
            else if (operationDate.year == to.year) {
                if (operationDate.month < to.month) {
                    beforeTo = true;
                }
                else if (operationDate.month == to.month && operationDate.day <= to.day) {
                    beforeTo = true;
                }
            }

            if (afterFrom && beforeTo) {
                total += accounts[i].operations[j].amount;
                count++;

                if (first) {
                    maxAccount = i;
                    maxIndex = j;
                    minAccount = i;
                    minIndex = j;
                    first = false;
                }
                else {
                    if (accounts[i].operations[j].amount >
                        accounts[maxAccount].operations[maxIndex].amount) {
                        maxAccount = i;
                        maxIndex = j;
                    }

                    if (accounts[i].operations[j].amount <
                        accounts[minAccount].operations[minIndex].amount) {
                        minAccount = i;
                        minIndex = j;
                    }
                }
            }
        }
    }

    if (count > 0) {
        avg = total / count;
    }
    else {
        cout << "Витрат за цей період немає.\n";
    }

    cout << "Всього витрачено: " << total << " грн\n";
    cout << "Кількість витрат: " << count << endl;

    if (count > 0) {
        string* categories = {};
        int categorySize = 0;

        for (int i = 0; i < accountSize; i++) {
            for (int j = 0; j < accounts[i].categorySize; j++) {
                bool found = false;

                for (int k = 0; k < categorySize; k++) {
                    if (categories[k] == accounts[i].categories[j].name) {
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    string* temp = new string[categorySize + 1];

                    for (int k = 0; k < categorySize; k++) {
                        temp[k] = categories[k];
                    }

                    temp[categorySize] = accounts[i].categories[j].name;

                    delete[] categories;
                    categories = temp;
                    categorySize++;
                }
            }
        }

        cout << "\nВитрати за категоріями:\n";

        for (int i = 0; i < categorySize; i++) {
            double sum = 0;

            for (int j = 0; j < accountSize; j++) {
                for (int k = 0; k < accounts[j].operationSize; k++) {
                    Date operationDate = accounts[j].operations[k].date;

                    bool afterFrom = false;
                    bool beforeTo = false;

                    if (operationDate.year > from.year) {
                        afterFrom = true;
                    }
                    else if (operationDate.year == from.year) {
                        if (operationDate.month > from.month) {
                            afterFrom = true;
                        }
                        else if (operationDate.month == from.month && operationDate.day >= from.day) {
                            afterFrom = true;
                        }
                    }

                    if (operationDate.year < to.year) {
                        beforeTo = true;
                    }
                    else if (operationDate.year == to.year) {
                        if (operationDate.month < to.month) {
                            beforeTo = true;
                        }
                        else if (operationDate.month == to.month && operationDate.day <= to.day) {
                            beforeTo = true;
                        }
                    }

                    if (afterFrom && beforeTo) {
                        if (accounts[j].operations[k].category == categories[i]) {
                            sum += accounts[j].operations[k].amount;
                        }
                    }
                }
            }

            cout << categories[i] << ": " << sum << " грн\n";
        }

        delete[] categories;

        cout << "\nВитрати за рахунками:\n";

        for (int i = 0; i < accountSize; i++) {
            double sum = 0;

            for (int j = 0; j < accounts[i].operationSize; j++) {
                Date operationDate = accounts[i].operations[j].date;

                bool afterFrom = false;
                bool beforeTo = false;

                if (operationDate.year > from.year) {
                    afterFrom = true;
                }
                else if (operationDate.year == from.year) {
                    if (operationDate.month > from.month) {
                        afterFrom = true;
                    }
                    else if (operationDate.month == from.month && operationDate.day >= from.day) {
                        afterFrom = true;
                    }
                }

                if (operationDate.year < to.year) {
                    beforeTo = true;
                }
                else if (operationDate.year == to.year) {
                    if (operationDate.month < to.month) {
                        beforeTo = true;
                    }
                    else if (operationDate.month == to.month && operationDate.day <= to.day) {
                        beforeTo = true;
                    }
                }

                if (afterFrom && beforeTo) {
                    sum += accounts[i].operations[j].amount;
                }
            }

            cout << accounts[i].name << ": " << sum << " грн\n";
        }

        cout << "\nНайбільша витрата: "
            << accounts[maxAccount].operations[maxIndex].amount << " грн\n";

        cout << "Найменша витрата: "
            << accounts[minAccount].operations[minIndex].amount << " грн\n";

        cout << "Середня витрата: " << avg << " грн\n";
    }

    // ==================== ЗВІТ ЗА ТИЖДЕНЬ ====================
    tm weekDate = local;
    weekDate.tm_mday -= 6;
    mktime(&weekDate);

    from.day = weekDate.tm_mday;
    from.month = weekDate.tm_mon + 1;
    from.year = weekDate.tm_year + 1900;

    to.day = local.tm_mday;
    to.month = local.tm_mon + 1;
    to.year = local.tm_year + 1900;

    cout << "\n========== ЗВІТ ЗА ТИЖДЕНЬ ==========\n";

    total = 0;
    count = 0;
    avg = 0;
    first = true;
    maxAccount = 0;
    maxIndex = 0;
    minAccount = 0;
    minIndex = 0;

    for (int i = 0; i < accountSize; i++) {
        for (int j = 0; j < accounts[i].operationSize; j++) {
            Date operationDate = accounts[i].operations[j].date;

            bool afterFrom = false;
            bool beforeTo = false;

            if (operationDate.year > from.year) {
                afterFrom = true;
            }
            else if (operationDate.year == from.year) {
                if (operationDate.month > from.month) {
                    afterFrom = true;
                }
                else if (operationDate.month == from.month && operationDate.day >= from.day) {
                    afterFrom = true;
                }
            }

            if (operationDate.year < to.year) {
                beforeTo = true;
            }
            else if (operationDate.year == to.year) {
                if (operationDate.month < to.month) {
                    beforeTo = true;
                }
                else if (operationDate.month == to.month && operationDate.day <= to.day) {
                    beforeTo = true;
                }
            }

            if (afterFrom && beforeTo) {
                total += accounts[i].operations[j].amount;
                count++;

                if (first) {
                    maxAccount = i;
                    maxIndex = j;
                    minAccount = i;
                    minIndex = j;
                    first = false;
                }
                else {
                    if (accounts[i].operations[j].amount >
                        accounts[maxAccount].operations[maxIndex].amount) {
                        maxAccount = i;
                        maxIndex = j;
                    }

                    if (accounts[i].operations[j].amount <
                        accounts[minAccount].operations[minIndex].amount) {
                        minAccount = i;
                        minIndex = j;
                    }
                }
            }
        }
    }

    if (count > 0) {
        avg = total / count;
    }
    else {
        cout << "Витрат за цей період немає.\n";
    }

    cout << "Всього витрачено: " << total << " грн\n";
    cout << "Кількість витрат: " << count << endl;

    if (count > 0) {
        string* categories = {};
        int categorySize = 0;

        for (int i = 0; i < accountSize; i++) {
            for (int j = 0; j < accounts[i].categorySize; j++) {
                bool found = false;

                for (int k = 0; k < categorySize; k++) {
                    if (categories[k] == accounts[i].categories[j].name) {
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    string* temp = new string[categorySize + 1];

                    for (int k = 0; k < categorySize; k++) {
                        temp[k] = categories[k];
                    }

                    temp[categorySize] = accounts[i].categories[j].name;

                    delete[] categories;
                    categories = temp;
                    categorySize++;
                }
            }
        }

        cout << "\nВитрати за категоріями:\n";

        for (int i = 0; i < categorySize; i++) {
            double sum = 0;

            for (int j = 0; j < accountSize; j++) {
                for (int k = 0; k < accounts[j].operationSize; k++) {
                    Date operationDate = accounts[j].operations[k].date;

                    bool afterFrom = false;
                    bool beforeTo = false;

                    if (operationDate.year > from.year) {
                        afterFrom = true;
                    }
                    else if (operationDate.year == from.year) {
                        if (operationDate.month > from.month) {
                            afterFrom = true;
                        }
                        else if (operationDate.month == from.month && operationDate.day >= from.day) {
                            afterFrom = true;
                        }
                    }

                    if (operationDate.year < to.year) {
                        beforeTo = true;
                    }
                    else if (operationDate.year == to.year) {
                        if (operationDate.month < to.month) {
                            beforeTo = true;
                        }
                        else if (operationDate.month == to.month && operationDate.day <= to.day) {
                            beforeTo = true;
                        }
                    }

                    if (afterFrom && beforeTo) {
                        if (accounts[j].operations[k].category == categories[i]) {
                            sum += accounts[j].operations[k].amount;
                        }
                    }
                }
            }

            cout << categories[i] << ": " << sum << " грн\n";
        }

        delete[] categories;

        cout << "\nВитрати за рахунками:\n";

        for (int i = 0; i < accountSize; i++) {
            double sum = 0;

            for (int j = 0; j < accounts[i].operationSize; j++) {
                Date operationDate = accounts[i].operations[j].date;

                bool afterFrom = false;
                bool beforeTo = false;

                if (operationDate.year > from.year) {
                    afterFrom = true;
                }
                else if (operationDate.year == from.year) {
                    if (operationDate.month > from.month) {
                        afterFrom = true;
                    }
                    else if (operationDate.month == from.month && operationDate.day >= from.day) {
                        afterFrom = true;
                    }
                }

                if (operationDate.year < to.year) {
                    beforeTo = true;
                }
                else if (operationDate.year == to.year) {
                    if (operationDate.month < to.month) {
                        beforeTo = true;
                    }
                    else if (operationDate.month == to.month && operationDate.day <= to.day) {
                        beforeTo = true;
                    }
                }

                if (afterFrom && beforeTo) {
                    sum += accounts[i].operations[j].amount;
                }
            }

            cout << accounts[i].name << ": " << sum << " грн\n";
        }

        cout << "\nНайбільша витрата: "
            << accounts[maxAccount].operations[maxIndex].amount << " грн\n";

        cout << "Найменша витрата: "
            << accounts[minAccount].operations[minIndex].amount << " грн\n";

        cout << "Середня витрата: " << avg << " грн\n";
    }

    // ==================== ЗВІТ ЗА 30 ДНІВ ====================
    tm monthDate = local;
    monthDate.tm_mday -= 29;
    mktime(&monthDate);

    from.day = monthDate.tm_mday;
    from.month = monthDate.tm_mon + 1;
    from.year = monthDate.tm_year + 1900;

    to.day = local.tm_mday;
    to.month = local.tm_mon + 1;
    to.year = local.tm_year + 1900;

    cout << "\n========== ЗВІТ ЗА 30 ДНІВ ==========\n";

    total = 0;
    count = 0;
    avg = 0;
    first = true;
    maxAccount = 0;
    maxIndex = 0;
    minAccount = 0;
    minIndex = 0;

    for (int i = 0; i < accountSize; i++) {
        for (int j = 0; j < accounts[i].operationSize; j++) {
            Date operationDate = accounts[i].operations[j].date;

            bool afterFrom = false;
            bool beforeTo = false;

            if (operationDate.year > from.year) {
                afterFrom = true;
            }
            else if (operationDate.year == from.year) {
                if (operationDate.month > from.month) {
                    afterFrom = true;
                }
                else if (operationDate.month == from.month && operationDate.day >= from.day) {
                    afterFrom = true;
                }
            }

            if (operationDate.year < to.year) {
                beforeTo = true;
            }
            else if (operationDate.year == to.year) {
                if (operationDate.month < to.month) {
                    beforeTo = true;
                }
                else if (operationDate.month == to.month && operationDate.day <= to.day) {
                    beforeTo = true;
                }
            }

            if (afterFrom && beforeTo) {
                total += accounts[i].operations[j].amount;
                count++;

                if (first) {
                    maxAccount = i;
                    maxIndex = j;
                    minAccount = i;
                    minIndex = j;
                    first = false;
                }
                else {
                    if (accounts[i].operations[j].amount >
                        accounts[maxAccount].operations[maxIndex].amount) {
                        maxAccount = i;
                        maxIndex = j;
                    }

                    if (accounts[i].operations[j].amount <
                        accounts[minAccount].operations[minIndex].amount) {
                        minAccount = i;
                        minIndex = j;
                    }
                }
            }
        }
    }

    if (count > 0) {
        avg = total / count;
    }
    else {
        cout << "Витрат за цей період немає.\n";
    }

    cout << "Всього витрачено: " << total << " грн\n";
    cout << "Кількість витрат: " << count << endl;

    if (count > 0) {
        string* categories = {};
        int categorySize = 0;

        for (int i = 0; i < accountSize; i++) {
            for (int j = 0; j < accounts[i].categorySize; j++) {
                bool found = false;

                for (int k = 0; k < categorySize; k++) {
                    if (categories[k] == accounts[i].categories[j].name) {
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    string* temp = new string[categorySize + 1];

                    for (int k = 0; k < categorySize; k++) {
                        temp[k] = categories[k];
                    }

                    temp[categorySize] = accounts[i].categories[j].name;

                    delete[] categories;
                    categories = temp;
                    categorySize++;
                }
            }
        }

        cout << "\nВитрати за категоріями:\n";

        for (int i = 0; i < categorySize; i++) {
            double sum = 0;

            for (int j = 0; j < accountSize; j++) {
                for (int k = 0; k < accounts[j].operationSize; k++) {
                    Date operationDate = accounts[j].operations[k].date;

                    bool afterFrom = false;
                    bool beforeTo = false;

                    if (operationDate.year > from.year) {
                        afterFrom = true;
                    }
                    else if (operationDate.year == from.year) {
                        if (operationDate.month > from.month) {
                            afterFrom = true;
                        }
                        else if (operationDate.month == from.month && operationDate.day >= from.day) {
                            afterFrom = true;
                        }
                    }

                    if (operationDate.year < to.year) {
                        beforeTo = true;
                    }
                    else if (operationDate.year == to.year) {
                        if (operationDate.month < to.month) {
                            beforeTo = true;
                        }
                        else if (operationDate.month == to.month && operationDate.day <= to.day) {
                            beforeTo = true;
                        }
                    }

                    if (afterFrom && beforeTo) {
                        if (accounts[j].operations[k].category == categories[i]) {
                            sum += accounts[j].operations[k].amount;
                        }
                    }
                }
            }

            cout << categories[i] << ": " << sum << " грн\n";
        }

        delete[] categories;

        cout << "\nВитрати за рахунками:\n";

        for (int i = 0; i < accountSize; i++) {
            double sum = 0;

            for (int j = 0; j < accounts[i].operationSize; j++) {
                Date operationDate = accounts[i].operations[j].date;

                bool afterFrom = false;
                bool beforeTo = false;

                if (operationDate.year > from.year) {
                    afterFrom = true;
                }
                else if (operationDate.year == from.year) {
                    if (operationDate.month > from.month) {
                        afterFrom = true;
                    }
                    else if (operationDate.month == from.month && operationDate.day >= from.day) {
                        afterFrom = true;
                    }
                }

                if (operationDate.year < to.year) {
                    beforeTo = true;
                }
                else if (operationDate.year == to.year) {
                    if (operationDate.month < to.month) {
                        beforeTo = true;
                    }
                    else if (operationDate.month == to.month && operationDate.day <= to.day) {
                        beforeTo = true;
                    }
                }

                if (afterFrom && beforeTo) {
                    sum += accounts[i].operations[j].amount;
                }
            }

            cout << accounts[i].name << ": " << sum << " грн\n";
        }

        cout << "\nНайбільша витрата: "
            << accounts[maxAccount].operations[maxIndex].amount << " грн\n";

        cout << "Найменша витрата: "
            << accounts[minAccount].operations[minIndex].amount << " грн\n";

        cout << "Середня витрата: " << avg << " грн\n";
    }

    // ==================== ЗВІТ ЗА ВЕСЬ ЧАС ====================
    from.day = 1;
    from.month = 1;
    from.year = 1;

    to.day = local.tm_mday;
    to.month = local.tm_mon + 1;
    to.year = local.tm_year + 1900;

    cout << "\n========== ЗВІТ ЗА ВЕСЬ ЧАС ==========\n";

    total = 0;
    count = 0;
    avg = 0;
    first = true;
    maxAccount = 0;
    maxIndex = 0;
    minAccount = 0;
    minIndex = 0;

    for (int i = 0; i < accountSize; i++) {
        for (int j = 0; j < accounts[i].operationSize; j++) {
            Date operationDate = accounts[i].operations[j].date;

            bool afterFrom = false;
            bool beforeTo = false;

            if (operationDate.year > from.year) {
                afterFrom = true;
            }
            else if (operationDate.year == from.year) {
                if (operationDate.month > from.month) {
                    afterFrom = true;
                }
                else if (operationDate.month == from.month && operationDate.day >= from.day) {
                    afterFrom = true;
                }
            }

            if (operationDate.year < to.year) {
                beforeTo = true;
            }
            else if (operationDate.year == to.year) {
                if (operationDate.month < to.month) {
                    beforeTo = true;
                }
                else if (operationDate.month == to.month && operationDate.day <= to.day) {
                    beforeTo = true;
                }
            }

            if (afterFrom && beforeTo) {
                total += accounts[i].operations[j].amount;
                count++;

                if (first) {
                    maxAccount = i;
                    maxIndex = j;
                    minAccount = i;
                    minIndex = j;
                    first = false;
                }
                else {
                    if (accounts[i].operations[j].amount >
                        accounts[maxAccount].operations[maxIndex].amount) {
                        maxAccount = i;
                        maxIndex = j;
                    }

                    if (accounts[i].operations[j].amount <
                        accounts[minAccount].operations[minIndex].amount) {
                        minAccount = i;
                        minIndex = j;
                    }
                }
            }
        }
    }

    if (count > 0) {
        avg = total / count;
    }
    else {
        cout << "Витрат за цей період немає.\n";
    }

    cout << "Всього витрачено: " << total << " грн\n";
    cout << "Кількість витрат: " << count << endl;

    if (count > 0) {
        string* categories = {};
        int categorySize = 0;

        for (int i = 0; i < accountSize; i++) {
            for (int j = 0; j < accounts[i].categorySize; j++) {
                bool found = false;

                for (int k = 0; k < categorySize; k++) {
                    if (categories[k] == accounts[i].categories[j].name) {
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    string* temp = new string[categorySize + 1];

                    for (int k = 0; k < categorySize; k++) {
                        temp[k] = categories[k];
                    }

                    temp[categorySize] = accounts[i].categories[j].name;

                    delete[] categories;
                    categories = temp;
                    categorySize++;
                }
            }
        }

        cout << "\nВитрати за категоріями:\n";

        for (int i = 0; i < categorySize; i++) {
            double sum = 0;

            for (int j = 0; j < accountSize; j++) {
                for (int k = 0; k < accounts[j].operationSize; k++) {
                    Date operationDate = accounts[j].operations[k].date;

                    bool afterFrom = false;
                    bool beforeTo = false;

                    if (operationDate.year > from.year) {
                        afterFrom = true;
                    }
                    else if (operationDate.year == from.year) {
                        if (operationDate.month > from.month) {
                            afterFrom = true;
                        }
                        else if (operationDate.month == from.month && operationDate.day >= from.day) {
                            afterFrom = true;
                        }
                    }

                    if (operationDate.year < to.year) {
                        beforeTo = true;
                    }
                    else if (operationDate.year == to.year) {
                        if (operationDate.month < to.month) {
                            beforeTo = true;
                        }
                        else if (operationDate.month == to.month && operationDate.day <= to.day) {
                            beforeTo = true;
                        }
                    }

                    if (afterFrom && beforeTo) {
                        if (accounts[j].operations[k].category == categories[i]) {
                            sum += accounts[j].operations[k].amount;
                        }
                    }
                }
            }

            cout << categories[i] << ": " << sum << " грн\n";
        }

        delete[] categories;

        cout << "\nВитрати за рахунками:\n";

        for (int i = 0; i < accountSize; i++) {
            double sum = 0;

            for (int j = 0; j < accounts[i].operationSize; j++) {
                Date operationDate = accounts[i].operations[j].date;

                bool afterFrom = false;
                bool beforeTo = false;

                if (operationDate.year > from.year) {
                    afterFrom = true;
                }
                else if (operationDate.year == from.year) {
                    if (operationDate.month > from.month) {
                        afterFrom = true;
                    }
                    else if (operationDate.month == from.month && operationDate.day >= from.day) {
                        afterFrom = true;
                    }
                }

                if (operationDate.year < to.year) {
                    beforeTo = true;
                }
                else if (operationDate.year == to.year) {
                    if (operationDate.month < to.month) {
                        beforeTo = true;
                    }
                    else if (operationDate.month == to.month && operationDate.day <= to.day) {
                        beforeTo = true;
                    }
                }

                if (afterFrom && beforeTo) {
                    sum += accounts[i].operations[j].amount;
                }
            }

            cout << accounts[i].name << ": " << sum << " грн\n";
        }

        cout << "\nНайбільша витрата: "
            << accounts[maxAccount].operations[maxIndex].amount << " грн\n";

        cout << "Найменша витрата: "
            << accounts[minAccount].operations[minIndex].amount << " грн\n";

        cout << "Середня витрата: " << avg << " грн\n";
    }
}
void topExpenses(Account* account, int accountSize) {
    time_t now = time(0);
    tm local;
    localtime_s(&local, &now);

    Date to;
    Date from = to;

    tm weekDate = local;
    weekDate.tm_mday -= 6;
    mktime(&weekDate);

    from.day = weekDate.tm_mday;
    from.month = weekDate.tm_mon + 1;
    from.year = weekDate.tm_year + 1900;

    to.day = local.tm_mday;
    to.month = local.tm_mon + 1;
    to.year = local.tm_year + 1900;

    double max = 0;
    double max2 = 0;
    double max3 = 0;

    int maxAccount = -1;
    int maxIndex = -1;
    int max2Account = -1;
    int max2Index = -1;
    int max3Account = -1;
    int max3Index = -1;

    cout << "\n========== ТОП-3 ВИТРАТИ ЗА ТИЖДЕНЬ ==========\n";

    for (int i = 0; i < accountSize; i++) {
        for (int j = 0; j < account[i].expenseSize; j++) {
            Date operationDate = account[i].expenses[j].date;

            bool afterFrom = false;
            bool beforeTo = false;

            if (operationDate.year > from.year) {
                afterFrom = true;
            }
            else if (operationDate.year == from.year) {
                if (operationDate.month > from.month) {
                    afterFrom = true;
                }
                else if (operationDate.month == from.month && operationDate.day >= from.day) {
                    afterFrom = true;
                }
            }

            if (operationDate.year < to.year) {
                beforeTo = true;
            }
            else if (operationDate.year == to.year) {
                if (operationDate.month < to.month) {
                    beforeTo = true;
                }
                else if (operationDate.month == to.month && operationDate.day <= to.day) {
                    beforeTo = true;
                }
            }

            if (afterFrom && beforeTo) {
                double neww = account[i].expenses[j].amount;

                if (max < neww) {
                    max3 = max2;
                    max3Account = max2Account;
                    max3Index = max2Index;

                    max2 = max;
                    max2Account = maxAccount;
                    max2Index = maxIndex;

                    max = neww;
                    maxAccount = i;
                    maxIndex = j;
                }
                else if (max2 < neww) {
                    max3 = max2;
                    max3Account = max2Account;
                    max3Index = max2Index;

                    max2 = neww;
                    max2Account = i;
                    max2Index = j;
                }
                else if (max3 < neww) {
                    max3 = neww;
                    max3Account = i;
                    max3Index = j;
                }
            }
        }
    }

    if (maxIndex != -1) {
        cout << "1. " << max << " грн --- " << account[maxAccount].expenses[maxIndex].category.name << " --- " << account[maxAccount].expenses[maxIndex].description << endl;
    }

    if (max2Index != -1) {
        cout << "2. " << max2 << " грн --- " << account[max2Account].expenses[max2Index].category.name << " --- " << account[max2Account].expenses[max2Index].description << endl;
    }

    if (max3Index != -1) {
        cout << "3. " << max3 << " грн --- " << account[max3Account].expenses[max3Index].category.name << " --- " << account[max3Account].expenses[max3Index].description << endl;
    }

    tm monthDate = local;
    monthDate.tm_mday -= 29;
    mktime(&monthDate);

    from.day = monthDate.tm_mday;
    from.month = monthDate.tm_mon + 1;
    from.year = monthDate.tm_year + 1900;

    to.day = local.tm_mday;
    to.month = local.tm_mon + 1;
    to.year = local.tm_year + 1900;

    max = 0;
    max2 = 0;
    max3 = 0;

    maxAccount = -1;
    maxIndex = -1;
    max2Account = -1;
    max2Index = -1;
    max3Account = -1;
    max3Index = -1;

    cout << "\n========== ТОП-3 ЗА 30 ДНІВ ==========\n";

    for (int i = 0; i < accountSize; i++) {
        for (int j = 0; j < account[i].expenseSize; j++) {
            Date operationDate = account[i].expenses[j].date;

            bool afterFrom = false;
            bool beforeTo = false;

            if (operationDate.year > from.year) {
                afterFrom = true;
            }
            else if (operationDate.year == from.year) {
                if (operationDate.month > from.month) {
                    afterFrom = true;
                }
                else if (operationDate.month == from.month && operationDate.day >= from.day) {
                    afterFrom = true;
                }
            }

            if (operationDate.year < to.year) {
                beforeTo = true;
            }
            else if (operationDate.year == to.year) {
                if (operationDate.month < to.month) {
                    beforeTo = true;
                }
                else if (operationDate.month == to.month && operationDate.day <= to.day) {
                    beforeTo = true;
                }
            }

            if (afterFrom && beforeTo) {
                double neww = account[i].expenses[j].amount;

                if (max < neww) {
                    max3 = max2;
                    max3Account = max2Account;
                    max3Index = max2Index;

                    max2 = max;
                    max2Account = maxAccount;
                    max2Index = maxIndex;

                    max = neww;
                    maxAccount = i;
                    maxIndex = j;
                }
                else if (max2 < neww) {
                    max3 = max2;
                    max3Account = max2Account;
                    max3Index = max2Index;

                    max2 = neww;
                    max2Account = i;
                    max2Index = j;
                }
                else if (max3 < neww) {
                    max3 = neww;
                    max3Account = i;
                    max3Index = j;
                }
            }
        }
    }

    if (maxIndex != -1) {
        cout << "1. " << max << " грн --- " << account[maxAccount].expenses[maxIndex].category.name << " --- " << account[maxAccount].expenses[maxIndex].description << endl;
    }
    if (max2Index != -1) {
        cout << "2. " << max2 << " грн --- " << account[max2Account].expenses[max2Index].category.name << " --- " << account[max2Account].expenses[max2Index].description << endl;
    }
    if (max3Index != -1) {
        cout << "3. " << max3 << " грн --- " << account[max3Account].expenses[max3Index].category.name << " --- " << account[max3Account].expenses[max3Index].description << endl;
    }
}
void topCategories(Account* account, int accountSize) {
    time_t now = time(0);
    tm local;
    localtime_s(&local, &now);

    Date to;
    Date from = to;

    tm weekDate = local;
    weekDate.tm_mday -= 6;
    mktime(&weekDate);

    from.day = weekDate.tm_mday;
    from.month = weekDate.tm_mon + 1;
    from.year = weekDate.tm_year + 1900;

    to.day = local.tm_mday;
    to.month = local.tm_mon + 1;
    to.year = local.tm_year + 1900;

    string* categoryNames = nullptr;
    double* categoryAmounts = nullptr;
    int categorySize = 0;

    for (int i = 0; i < accountSize; i++) {
        for (int j = 0; j < account[i].expenseSize; j++) {
            Date operationDate = account[i].expenses[j].date;

            bool afterFrom = false;
            bool beforeTo = false;

            if (operationDate.year > from.year) {
                afterFrom = true;
            }
            else if (operationDate.year == from.year) {
                if (operationDate.month > from.month) {
                    afterFrom = true;
                }
                else if (operationDate.month == from.month && operationDate.day >= from.day) {
                    afterFrom = true;
                }
            }

            if (operationDate.year < to.year) {
                beforeTo = true;
            }
            else if (operationDate.year == to.year) {
                if (operationDate.month < to.month) {
                    beforeTo = true;
                }
                else if (operationDate.month == to.month && operationDate.day <= to.day) {
                    beforeTo = true;
                }
            }

            if (afterFrom && beforeTo) {
                string category = account[i].expenses[j].category.name;
                bool found = false;

                for (int k = 0; k < categorySize; k++) {
                    if (categoryNames[k] == category) {
                        categoryAmounts[k] += account[i].expenses[j].amount;
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    string* newNames = new string[categorySize + 1];
                    double* newAmounts = new double[categorySize + 1];

                    for (int k = 0; k < categorySize; k++) {
                        newNames[k] = categoryNames[k];
                        newAmounts[k] = categoryAmounts[k];
                    }

                    newNames[categorySize] = category;
                    newAmounts[categorySize] = account[i].expenses[j].amount;

                    delete[] categoryNames;
                    delete[] categoryAmounts;

                    categoryNames = newNames;
                    categoryAmounts = newAmounts;
                    categorySize++;
                }
            }
        }
    }

    double max = 0;
    double max2 = 0;
    double max3 = 0;
    int maxIndex = -1;
    int max2Index = -1;
    int max3Index = -1;

    for (int i = 0; i < categorySize; i++) {
        double neww = categoryAmounts[i];

        if (max < neww) {
            max3 = max2;
            max3Index = max2Index;

            max2 = max;
            max2Index = maxIndex;

            max = neww;
            maxIndex = i;
        }
        else if (max2 < neww) {
            max3 = max2;
            max3Index = max2Index;

            max2 = neww;
            max2Index = i;
        }
        else if (max3 < neww) {
            max3 = neww;
            max3Index = i;
        }
    }

    cout << "\n========== ТОП-3 КАТЕГОРІЇ ЗА ТИЖДЕНЬ ==========\n";

    if (maxIndex != -1) {
        cout << "1. " << categoryNames[maxIndex] << " --- " << max << " грн" << endl;
    }

    if (max2Index != -1) {
        cout << "2. " << categoryNames[max2Index] << " --- " << max2 << " грн" << endl;
    }

    if (max3Index != -1) {
        cout << "3. " << categoryNames[max3Index] << " --- " << max3 << " грн" << endl;
    }

    delete[] categoryNames;
    delete[] categoryAmounts;

    tm monthDate = local;
    monthDate.tm_mday -= 29;
    mktime(&monthDate);

    from.day = monthDate.tm_mday;
    from.month = monthDate.tm_mon + 1;
    from.year = monthDate.tm_year + 1900;

    to.day = local.tm_mday;
    to.month = local.tm_mon + 1;
    to.year = local.tm_year + 1900;

    categoryNames = nullptr;
    categoryAmounts = nullptr;
    categorySize = 0;

    for (int i = 0; i < accountSize; i++) {
        for (int j = 0; j < account[i].expenseSize; j++) {
            Date operationDate = account[i].expenses[j].date;

            bool afterFrom = false;
            bool beforeTo = false;

            if (operationDate.year > from.year) {
                afterFrom = true;
            }
            else if (operationDate.year == from.year) {
                if (operationDate.month > from.month) {
                    afterFrom = true;
                }
                else if (operationDate.month == from.month && operationDate.day >= from.day) {
                    afterFrom = true;
                }
            }

            if (operationDate.year < to.year) {
                beforeTo = true;
            }
            else if (operationDate.year == to.year) {
                if (operationDate.month < to.month) {
                    beforeTo = true;
                }
                else if (operationDate.month == to.month && operationDate.day <= to.day) {
                    beforeTo = true;
                }
            }

            if (afterFrom && beforeTo) {
                string category = account[i].expenses[j].category.name;
                bool found = false;

                for (int k = 0; k < categorySize; k++) {
                    if (categoryNames[k] == category) {
                        categoryAmounts[k] += account[i].expenses[j].amount;
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    string* newNames = new string[categorySize + 1];
                    double* newAmounts = new double[categorySize + 1];

                    for (int k = 0; k < categorySize; k++) {
                        newNames[k] = categoryNames[k];
                        newAmounts[k] = categoryAmounts[k];
                    }

                    newNames[categorySize] = category;
                    newAmounts[categorySize] = account[i].expenses[j].amount;

                    delete[] categoryNames;
                    delete[] categoryAmounts;

                    categoryNames = newNames;
                    categoryAmounts = newAmounts;
                    categorySize++;
                }
            }
        }
    }

    max = 0;
    max2 = 0;
    max3 = 0;
    maxIndex = -1;
    max2Index = -1;
    max3Index = -1;

    for (int i = 0; i < categorySize; i++) {
        double neww = categoryAmounts[i];

        if (max < neww) {
            max3 = max2;
            max3Index = max2Index;

            max2 = max;
            max2Index = maxIndex;

            max = neww;
            maxIndex = i;
        }
        else if (max2 < neww) {
            max3 = max2;
            max3Index = max2Index;

            max2 = neww;
            max2Index = i;
        }
        else if (max3 < neww) {
            max3 = neww;
            max3Index = i;
        }
    }

    cout << "\n========== ТОП-3 КАТЕГОРІЇ ЗА 30 ДНІВ ==========\n";

    if (maxIndex != -1) {
        cout << "1. " << categoryNames[maxIndex] << " --- " << max << " грн" << endl;
    }
    if (max2Index != -1) {
        cout << "2. " << categoryNames[max2Index] << " --- " << max2 << " грн" << endl;
    }
    if (max3Index != -1) {
        cout << "3. " << categoryNames[max3Index] << " --- " << max3 << " грн" << endl;
    }

    delete[] categoryNames;
    delete[] categoryAmounts;
}

//Рахунки
void addAccount(Account*& accounts, int& size, string const name, int const type, string const card) {
    Account* temp = new Account[size + 1];
    int id = 1;
    time_t now = time(0);
    tm local;
    localtime_s(&local, &now);

    tm futureDate = local;
    futureDate.tm_year += 5;
    mktime(&futureDate);
    for (int i = 0; i < size; i++) {
        temp[i] = accounts[i];
    }
    for (int i = 0; i < size; i++) {
        if (temp[i].id >= id) {
            id = temp[i].id + 1;
        }
    }
    temp[size].id = id;
    temp[size].name = name;
    temp[size].card.number = card;
    temp[size].card.expiry.day = futureDate.tm_mday;
    temp[size].card.expiry.month = futureDate.tm_mon + 1;
    temp[size].card.expiry.year = futureDate.tm_year + 1900;
    if (type == 1) {
        temp[size].type = CASH;
        temp[size].card.number = "None";
        temp[size].card.expiry.day = 0;
        temp[size].card.expiry.month = 0;
        temp[size].card.expiry.year = 0;
    }
    else if (type == 2) {
        temp[size].type = DEBIT_CARD;
    }
    else if (type == 3) {
        temp[size].type = CREDIT_CARD;
    }
    delete[] accounts;
    accounts = temp;
    size++;
}
void showAccounts(Account* accounts, int size) {
    for (int i = 0; i < size; i++) {
        cout << "Id: " << accounts[i].id << endl;
        cout << "Name: " << accounts[i].name << endl;
        cout << "Type: " << accounts[i].type << endl;
        cout << "Balance: " << accounts[i].balance << endl;
        if (accounts[i].type != CASH) {
            cout << "Card: " << accounts[i].card.number << endl;
            cout << "Expirity: " << accounts[i].card.expiry.day << "." << accounts[i].card.expiry.month << "." << accounts[i].card.expiry.year << endl;
        }
    }
}
void showAccount(Account* accounts, int size, int id) {
    bool found = false;
    for (int i = 0; i < size; i++) {
        if (accounts[i].id == id) {
            found = true;
            cout << "Id: " << accounts[i].id << endl;
            cout << "Name: " << accounts[i].name << endl;
            cout << "Type: " << accounts[i].type << endl;
            cout << "Balance: " << accounts[i].balance << endl;
            if (accounts[i].type != CASH) {
                cout << "Card: " << accounts[i].card.number << endl;
                cout << "Expirity: " << accounts[i].card.expiry.day << "." << accounts[i].card.expiry.month << "." << accounts[i].card.expiry.year << endl;
            }
        }
    }
    if (!found) {
        throw "Акаунту з таким айді не існує";
    }
}
void editAccount(Account* accounts, int size, int id, string name, string card) {
     bool found = false;

    for (int i = 0; i < size; i++) {
        if (accounts[i].id == id) {
            found = true;

            if (name != "") {
                accounts[i].name = name;
            }

            if (accounts[i].type != CASH && card != "") {
                accounts[i].card.number = card;
            }

            break;
        }
    }

    if (!found) {
        throw "Акаунту з таким айді не існує";
    }
}
void deleteAccount(Account*& accounts, int& size, int id) {
    bool found = false;
    int index = -1;

    for (int i = 0; i < size; i++) {
        if (accounts[i].id == id) {
            found = true;
            index = i;
            break;
        }
    }

    if (!found) {
        throw "Акаунту з таким айді не існує";
    }

    Account* temp = new Account[size - 1];

    for (int i = 0; i < index; i++) {
        temp[i] = accounts[i];
    }

    for (int i = index; i < size - 1; i++) {
        temp[i] = accounts[i + 1];
    }

    delete[] accounts;
    accounts = temp;
    size--;
}