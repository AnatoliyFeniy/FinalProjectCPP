#include "Finance.h"
using namespace std;


// ==================== ПЕРЕВІРКА ВВОДУ ====================

bool isEmpty(string text) {
    if (text.empty()) {
        return true;
    }

    for (int i = 0; i < text.length(); i++) {
        if (!isspace(text[i])) {
            return false;
        }
    }

    return true;
}


bool isNumber(string text) {
    if (text.empty()) {
        return false;
    }

    for (int i = 0; i < text.length(); i++) {
        if (!isdigit(text[i])) {
            return false;
        }
    }

    return true;
}


bool isDecimal(string text) {
    if (text.empty()) {
        return false;
    }

    bool dot = false;

    for (int i = 0; i < text.length(); i++) {
        if (text[i] == '.') {
            if (dot) {
                return false;
            }

            dot = true;
        }
        else if (!isdigit(text[i])) {
            return false;
        }
    }

    return true;
}


string inputName(string message) {
    string name;

    cout << message;
    getline(cin, name);

    if (isEmpty(name)) {
        throw "Поле не може бути порожнім";
    }

    for (int i = 0; i < name.length(); i++) {
        if (isdigit(name[i])) {
            throw "Ім'я не може містити цифри";
        }
    }

    return name;
}


string inputCard() {
    string card;

    cout << "Введіть номер картки: ";
    getline(cin, card);

    if (card.empty()) {
        throw "Номер картки не може бути порожнім";
    }

    if (card.length() != 16) {
        throw "Номер картки повинен містити рівно 16 цифр";
    }

    for (int i = 0; i < card.length(); i++) {
        if (!isdigit(card[i])) {
            throw "Номер картки повинен містити тільки цифри";
        }
    }

    return card;
}


int inputInt(string message) {
    string text;

    cout << message;
    getline(cin, text);

    if (isEmpty(text)) {
        throw "Поле не може бути порожнім";
    }

    if (!isNumber(text)) {
        throw "Потрібно ввести ціле число";
    }

    return stoi(text);
}


double inputDouble(string message) {
    string text;

    cout << message;
    getline(cin, text);

    if (isEmpty(text)) {
        throw "Поле не може бути порожнім";
    }

    if (!isDecimal(text)) {
        throw "Потрібно ввести число";
    }

    return stod(text);
}


// ==================== ЗБЕРЕЖЕННЯ ====================

void saveAccounts(Account* accounts, int size) {
    ofstream file("accounts.txt");

    if (!file.is_open()) {
        throw "Не вдалося відкрити accounts.txt";
    }

    file << size << endl;

    for (int i = 0; i < size; i++) {
        file << accounts[i].id << " "
            << accounts[i].name << " "
            << accounts[i].type << " "
            << accounts[i].balance << " "
            << accounts[i].card.number << " "
            << accounts[i].card.expiry.day << " "
            << accounts[i].card.expiry.month << " "
            << accounts[i].card.expiry.year << endl;
    }

    file.close();
}


void saveCategories(Account* accounts, int accountSize) {
    ofstream file("categories.txt");

    if (!file.is_open()) {
        throw "Не вдалося відкрити categories.txt";
    }

    int count = 0;

    for (int i = 0; i < accountSize; i++) {
        count += accounts[i].categorySize;
    }

    file << count << endl;

    for (int i = 0; i < accountSize; i++) {
        for (int j = 0; j < accounts[i].categorySize; j++) {
            file << accounts[i].id << " "
                << accounts[i].categories[j].id << " "
                << accounts[i].categories[j].name << endl;
        }
    }

    file.close();
}


void saveExpenses(Account* accounts, int accountSize) {
    ofstream file("expenses.txt");

    if (!file.is_open()) {
        throw "Не вдалося відкрити expenses.txt";
    }

    int count = 0;

    for (int i = 0; i < accountSize; i++) {
        count += accounts[i].expenseSize;
    }

    file << count << endl;

    for (int i = 0; i < accountSize; i++) {
        for (int j = 0; j < accounts[i].expenseSize; j++) {
            file << accounts[i].id << " "
                << accounts[i].expenses[j].id << " "
                << accounts[i].expenses[j].amount << " "
                << accounts[i].expenses[j].date.day << " "
                << accounts[i].expenses[j].date.month << " "
                << accounts[i].expenses[j].date.year << " "
                << accounts[i].expenses[j].category.id << " "
                << accounts[i].expenses[j].category.name << " "
                << accounts[i].expenses[j].description << endl;
        }
    }

    file.close();
}


void saveOperations(Account* accounts, int accountSize) {
    ofstream file("operations.txt");

    if (!file.is_open()) {
        throw "Не вдалося відкрити operations.txt";
    }

    int count = 0;

    for (int i = 0; i < accountSize; i++) {
        count += accounts[i].operationSize;
    }

    file << count << endl;

    for (int i = 0; i < accountSize; i++) {
        for (int j = 0; j < accounts[i].operationSize; j++) {
            file << accounts[i].id << " "
                << accounts[i].operations[j].id << " "
                << accounts[i].operations[j].date.day << " "
                << accounts[i].operations[j].date.month << " "
                << accounts[i].operations[j].date.year << " "
                << accounts[i].operations[j].type << " "
                << accounts[i].operations[j].amount << " "
                << accounts[i].operations[j].category << " "
                << accounts[i].operations[j].description << endl;
        }
    }

    file.close();
}


void saveAll(Account* accounts, int accountSize) {
    saveAccounts(accounts, accountSize);
    saveCategories(accounts, accountSize);
    saveExpenses(accounts, accountSize);
    saveOperations(accounts, accountSize);
}


// ==================== ЗАВАНТАЖЕННЯ ====================

void loadAccounts(Account*& accounts, int& size) {
    ifstream file("accounts.txt");

    if (!file.is_open()) {
        return;
    }

    file >> size;

    if (size > 0) {
        accounts = new Account[size];

        for (int i = 0; i < size; i++) {
            int type;

            file >> accounts[i].id
                >> accounts[i].name
                >> type
                >> accounts[i].balance
                >> accounts[i].card.number
                >> accounts[i].card.expiry.day
                >> accounts[i].card.expiry.month
                >> accounts[i].card.expiry.year;

            accounts[i].type = (AccountType)type;
        }
    }

    file.close();
}


void loadCategories(Account* accounts, int accountSize) {
    ifstream file("categories.txt");

    if (!file.is_open()) {
        return;
    }

    int count;
    file >> count;

    for (int i = 0; i < count; i++) {
        int accountId;
        int categoryId;
        string name;

        file >> accountId >> categoryId >> name;

        for (int j = 0; j < accountSize; j++) {
            if (accounts[j].id == accountId) {
                Category* temp = new Category[accounts[j].categorySize + 1];

                for (int k = 0; k < accounts[j].categorySize; k++) {
                    temp[k] = accounts[j].categories[k];
                }

                temp[accounts[j].categorySize].id = categoryId;
                temp[accounts[j].categorySize].name = name;

                delete[] accounts[j].categories;
                accounts[j].categories = temp;
                accounts[j].categorySize++;

                break;
            }
        }
    }

    file.close();
}


void loadExpenses(Account* accounts, int accountSize) {
    ifstream file("expenses.txt");

    if (!file.is_open()) {
        return;
    }

    int count;
    file >> count;

    for (int i = 0; i < count; i++) {
        int accountId;
        Expense expense;

        file >> accountId
            >> expense.id
            >> expense.amount
            >> expense.date.day
            >> expense.date.month
            >> expense.date.year
            >> expense.category.id
            >> expense.category.name
            >> expense.description;

        for (int j = 0; j < accountSize; j++) {
            if (accounts[j].id == accountId) {
                Expense* temp = new Expense[accounts[j].expenseSize + 1];

                for (int k = 0; k < accounts[j].expenseSize; k++) {
                    temp[k] = accounts[j].expenses[k];
                }

                temp[accounts[j].expenseSize] = expense;

                delete[] accounts[j].expenses;
                accounts[j].expenses = temp;
                accounts[j].expenseSize++;

                break;
            }
        }
    }

    file.close();
}


void loadOperations(Account* accounts, int accountSize) {
    ifstream file("operations.txt");

    if (!file.is_open()) {
        return;
    }

    int count;
    file >> count;

    for (int i = 0; i < count; i++) {
        int accountId;
        Operation operation;

        file >> accountId
            >> operation.id
            >> operation.date.day
            >> operation.date.month
            >> operation.date.year
            >> operation.type
            >> operation.amount
            >> operation.category
            >> operation.description;

        for (int j = 0; j < accountSize; j++) {
            if (accounts[j].id == accountId) {
                Operation* temp = new Operation[accounts[j].operationSize + 1];

                for (int k = 0; k < accounts[j].operationSize; k++) {
                    temp[k] = accounts[j].operations[k];
                }

                temp[accounts[j].operationSize] = operation;

                delete[] accounts[j].operations;
                accounts[j].operations = temp;
                accounts[j].operationSize++;

                break;
            }
        }
    }

    file.close();
}


// ==================== MAIN ====================

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    Account* accounts = nullptr;
    int accountSize = 0;

    try {
        loadAccounts(accounts, accountSize);
        loadCategories(accounts, accountSize);
        loadExpenses(accounts, accountSize);
        loadOperations(accounts, accountSize);
    }
    catch (const char* error) {
        cout << "Помилка завантаження: " << error << endl;
    }


    bool running = true;

    while (running) {
        cout << "\n========== ОСОБИСТІ ФІНАНСИ ==========\n";
        cout << "1. Рахунки\n";
        cout << "2. Поповнення рахунку\n";
        cout << "3. Категорії\n";
        cout << "4. Витрати\n";
        cout << "5. Історія операцій\n";
        cout << "6. Пошук та фільтрація\n";
        cout << "7. Звіти\n";
        cout << "8. Топ-3 витрати\n";
        cout << "9. Топ-3 категорії\n";
        cout << "10. Зберегти\n";
        cout << "0. Вихід\n";

        try {
            int choice = inputInt("Ваш вибір: ");

            switch (choice) {

                // ==================== РАХУНКИ ====================
            case 1: {
                cout << "\n========== РАХУНКИ ==========\n";
                cout << "1. Створити рахунок\n";
                cout << "2. Показати всі рахунки\n";
                cout << "3. Показати один рахунок\n";
                cout << "4. Редагувати рахунок\n";
                cout << "5. Видалити рахунок\n";
                cout << "0. Назад\n";

                int accountChoice = inputInt("Ваш вибір: ");

                switch (accountChoice) {

                case 1: {
                    string name = inputName("Введіть ім'я рахунку: ");

                    int type = inputInt("Тип рахунку (1 - готівка, 2 - дебетова картка, 3 - кредитна картка): ");

                    if (type < 1 || type > 3) {
                        throw "Невірний тип рахунку";
                    }

                    string card = "None";

                    if (type == 2 || type == 3) {
                        card = inputCard();
                    }

                    addAccount(accounts, accountSize, name, type, card);
                    saveAll(accounts, accountSize);

                    cout << "Рахунок успішно створено\n";
                    break;
                }

                case 2:
                    showAccounts(accounts, accountSize);
                    break;

                case 3: {
                    int id = inputInt("Введіть ID рахунку: ");
                    showAccount(accounts, accountSize, id);
                    break;
                }

                case 4: {
                    int id = inputInt("Введіть ID рахунку: ");

                    string name;
                    string card;

                    cout << "Нове ім'я (Enter - залишити старе): ";
                    getline(cin, name);

                    cout << "Новий номер картки (Enter - залишити старий): ";
                    getline(cin, card);

                    if (!name.empty()) {
                        for (int i = 0; i < name.length(); i++) {
                            if (isdigit(name[i])) {
                                throw "Ім'я не може містити цифри";
                            }
                        }
                    }

                    if (!card.empty()) {
                        if (card.length() != 16) {
                            throw "Номер картки повинен містити 16 цифр";
                        }

                        for (int i = 0; i < card.length(); i++) {
                            if (!isdigit(card[i])) {
                                throw "Номер картки повинен містити тільки цифри";
                            }
                        }
                    }

                    editAccount(accounts, accountSize, id, name, card);
                    saveAll(accounts, accountSize);

                    cout << "Рахунок відредаговано\n";
                    break;
                }

                case 5: {
                    int id = inputInt("Введіть ID рахунку: ");

                    deleteAccount(accounts, accountSize, id);
                    saveAll(accounts, accountSize);

                    cout << "Рахунок видалено\n";
                    break;
                }

                case 0:
                    break;

                default:
                    throw "Невірний пункт меню";
                }

                break;
            }


                  // ==================== ПОПОВНЕННЯ ====================
            case 2: {
                if (accountSize == 0) {
                    throw "Рахунків немає";
                }

                int id = inputInt("Введіть ID рахунку: ");
                int money = inputInt("Введіть суму поповнення: ");

                bool found = false;

                for (int i = 0; i < accountSize; i++) {
                    if (accounts[i].id == id) {
                        deposit(accounts[i], money);
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    throw "Акаунту з таким айді не існує";
                }

                saveAll(accounts, accountSize);

                cout << "Рахунок поповнено\n";
                break;
            }


                  // ==================== КАТЕГОРІЇ ====================
            case 3: {
                if (accountSize == 0) {
                    throw "Спочатку створіть рахунок";
                }

                int id = inputInt("Введіть ID рахунку: ");

                Account* account = nullptr;

                for (int i = 0; i < accountSize; i++) {
                    if (accounts[i].id == id) {
                        account = &accounts[i];
                        break;
                    }
                }

                if (account == nullptr) {
                    throw "Акаунту з таким айді не існує";
                }

                cout << "\n1. Додати категорію\n";
                cout << "2. Показати категорії\n";
                cout << "3. Видалити категорію\n";
                cout << "0. Назад\n";

                int categoryChoice = inputInt("Ваш вибір: ");

                switch (categoryChoice) {

                case 1: {
                    string category = inputName("Введіть назву категорії: ");

                    addCategory(*account, category);
                    saveAll(accounts, accountSize);

                    cout << "Категорію додано\n";
                    break;
                }

                case 2:
                    showCategories(*account);
                    break;

                case 3: {
                    int cId = inputInt("Введіть ID категорії: ");

                    deleteCategory(*account, cId);
                    saveAll(accounts, accountSize);

                    cout << "Категорію видалено\n";
                    break;
                }

                case 0:
                    break;

                default:
                    throw "Невірний пункт меню";
                }

                break;
            }


                  // ==================== ВИТРАТИ ====================
            case 4: {
                if (accountSize == 0) {
                    throw "Рахунків немає";
                }

                int id = inputInt("Введіть ID рахунку: ");

                Account* account = nullptr;

                for (int i = 0; i < accountSize; i++) {
                    if (accounts[i].id == id) {
                        account = &accounts[i];
                        break;
                    }
                }

                if (account == nullptr) {
                    throw "Акаунту з таким айді не існує";
                }

                cout << "\n1. Додати витрату\n";
                cout << "2. Редагувати витрату\n";
                cout << "3. Видалити витрату\n";
                cout << "0. Назад\n";

                int expenseChoice = inputInt("Ваш вибір: ");

                switch (expenseChoice) {

                case 1: {
                    double amount = inputDouble("Введіть суму витрати: ");

                    if (amount <= 0) {
                        throw "Сума повинна бути більше 0";
                    }

                    string category = inputName("Введіть назву категорії: ");

                    addExpense(*account, amount, category);
                    saveAll(accounts, accountSize);

                    cout << "Витрату додано\n";
                    break;
                }

                case 2: {
                    int expenseId = inputInt("Введіть ID витрати: ");
                    double amount = inputDouble("Введіть нову суму: ");
                    string category = inputName("Введіть нову категорію: ");
                    string description = inputName("Введіть опис: ");

                    editExpense(*account, expenseId, amount, category, description);
                    saveAll(accounts, accountSize);

                    cout << "Витрату відредаговано\n";
                    break;
                }

                case 3: {
                    int expenseId = inputInt("Введіть ID витрати: ");

                    deleteExpense(*account, expenseId);
                    saveAll(accounts, accountSize);

                    cout << "Витрату видалено\n";
                    break;
                }

                case 0:
                    break;

                default:
                    throw "Невірний пункт меню";
                }

                break;
            }


                  // ==================== ОПЕРАЦІЇ ====================
            case 5: {
                cout << "\n1. Операції одного рахунку\n";
                cout << "2. Всі операції\n";
                cout << "0. Назад\n";

                int operationChoice = inputInt("Ваш вибір: ");

                switch (operationChoice) {

                case 1: {
                    int id = inputInt("Введіть ID рахунку: ");

                    bool found = false;

                    for (int i = 0; i < accountSize; i++) {
                        if (accounts[i].id == id) {
                            showOperations(accounts[i]);
                            found = true;
                            break;
                        }
                    }

                    if (!found) {
                        throw "Акаунту з таким айді не існує";
                    }

                    break;
                }

                case 2:
                    showAllOperations(accounts, accountSize);
                    break;

                case 0:
                    break;

                default:
                    throw "Невірний пункт меню";
                }

                break;
            }


                  // ==================== ПОШУК ====================
            case 6: {
                cout << "\n1. По категорії\n";
                cout << "2. По даті\n";
                cout << "3. По опису\n";
                cout << "0. Назад\n";

                int searchChoice = inputInt("Ваш вибір: ");

                switch (searchChoice) {

                case 1: {
                    string category = inputName("Введіть категорію: ");
                    filterByCategory(accounts, accountSize, category);
                    break;
                }

                case 2: {
                    Date from;
                    Date to;

                    from.day = inputInt("Від, день: ");
                    from.month = inputInt("Від, місяць: ");
                    from.year = inputInt("Від, рік: ");

                    to.day = inputInt("До, день: ");
                    to.month = inputInt("До, місяць: ");
                    to.year = inputInt("До, рік: ");

                    if (from.month < 1 || from.month > 12 ||
                        to.month < 1 || to.month > 12) {
                        throw "Некоректний місяць";
                    }

                    if (from.day < 1 || from.day > 31 ||
                        to.day < 1 || to.day > 31) {
                        throw "Некоректний день";
                    }

                    filterByDate(accounts, accountSize, from, to);
                    break;
                }

                case 3: {
                    string description = inputName("Введіть опис: ");
                    searchByDescription(accounts, accountSize, description);
                    break;
                }

                case 0:
                    break;

                default:
                    throw "Невірний пункт меню";
                }

                break;
            }


                  // ==================== ЗВІТИ ====================
            case 7:
                showReport(accounts, accountSize);
                break;


                // ==================== ТОП ВИТРАТ ====================
            case 8:
                topExpenses(accounts, accountSize);
                break;


                // ==================== ТОП КАТЕГОРІЙ ====================
            case 9:
                topCategories(accounts, accountSize);
                break;


                // ==================== ЗБЕРЕГТИ ====================
            case 10:
                saveAll(accounts, accountSize);
                cout << "Дані збережено\n";
                break;


                // ==================== ВИХІД ====================
            case 0:
                saveAll(accounts, accountSize);
                cout << "Дані збережено. До побачення!\n";
                running = false;
                break;


            default:
                throw "Невірний пункт меню";
            }
        }
        catch (const char* error) {
            cout << "\nПомилка: " << error << endl;
        }
        catch (...) {
            cout << "\nСталася невідома помилка\n";
        }
    }


    for (int i = 0; i < accountSize; i++) {
        delete[] accounts[i].categories;
        delete[] accounts[i].expenses;
        delete[] accounts[i].operations;
    }

    delete[] accounts;

    return 0;
}