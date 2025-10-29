#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <map>

template<typename T>
class IRepository {
public:
    virtual ~IRepository() = default;
    virtual bool save(const T& entity) = 0;
    virtual bool remove(int id) = 0;
    virtual std::vector<T> findAll() = 0;
    virtual T* findById(int id) = 0;
    virtual std::vector<T> findByField(const std::string& fieldName, const std::string& value) = 0;
};

class Transaction {
public:
    enum class Type { INCOME, EXPENSE };

private:
    int id;
    double amount;
    std::string category;
    std::string description;
    std::string date;
    Type type;

public:
    Transaction() : id(0), amount(0), type(Type::EXPENSE) {}
    Transaction(int id, double amount, const std::string& category,
        const std::string& description, const std::string& date, Type type)
        : id(id), amount(amount), category(category), description(description), date(date), type(type) {
    }

    int getId() const { return id; }
    double getAmount() const { return amount; }
    std::string getCategory() const { return category; }
    std::string getDescription() const { return description; }
    std::string getDate() const { return date; }
    Type getType() const { return type; }

    void setId(int newId) { id = newId; }
    void setAmount(double newAmount) { amount = newAmount; }
    void setCategory(const std::string& newCategory) { category = newCategory; }
    void setDescription(const std::string& newDescription) { description = newDescription; }
    void setDate(const std::string& newDate) { date = newDate; }
    void setType(Type newType) { type = newType; }

    std::string getTypeString() const {
        return (type == Type::INCOME) ? "INCOME" : "EXPENSE";
    }

    static Type stringToType(const std::string& typeStr) {
        return (typeStr == "INCOME") ? Type::INCOME : Type::EXPENSE;
    }

    void display() const {
        std::cout << "ID: " << id << " | " << date << " | " << getTypeString()
            << " | " << category << " | " << amount << " PLN | " << description << std::endl;
    }
};

class Budget {
private:
    int id;
    std::string category;
    double limit;
    double spent;

public:
    Budget() : id(0), limit(0), spent(0) {}
    Budget(int id, const std::string& category, double limit, double spent = 0)
        : id(id), category(category), limit(limit), spent(spent) {
    }

    int getId() const { return id; }
    std::string getCategory() const { return category; }
    double getLimit() const { return limit; }
    double getSpent() const { return spent; }
    double getRemaining() const { return limit - spent; }

    void setId(int newId) { id = newId; }
    void setCategory(const std::string& newCategory) { category = newCategory; }
    void setLimit(double newLimit) { limit = newLimit; }
    void setSpent(double newSpent) { spent = newSpent; }

    void addSpending(double amount) { spent += amount; }

    void display() const {
        double remaining = getRemaining();
        std::cout << "Kategoria: " << category << " | Limit: " << limit
            << " PLN | Wydano: " << spent << " PLN | Pozostalo: "
            << remaining << " PLN" << std::endl;

        if (remaining < 0) {
            std::cout << "! Przekroczono budzet o " << (-remaining) << " PLN!" << std::endl;
        }
    }
};

class User {
private:
    int id;
    std::string username;
    std::string pinHash;

public:
    User() : id(0) {}
    User(int id, const std::string& username, const std::string& pinHash)
        : id(id), username(username), pinHash(pinHash) {
    }

    int getId() const { return id; }
    std::string getUsername() const { return username; }
    std::string getPinHash() const { return pinHash; }

    void setPinHash(const std::string& newPinHash) { pinHash = newPinHash; }

    bool verifyPin(const std::string& pin) const {
        return pinHash == std::to_string(std::hash<std::string>{}(pin));
    }
};

class FileTransactionRepository : public IRepository<Transaction> {
private:
    std::string filename;
    int nextId;

    int getNextId() {
        return nextId++;
    }

    void loadNextId() {
        std::ifstream file(filename);
        std::string line;
        int maxId = 0;

        if (std::getline(file, line)) { 
            while (std::getline(file, line)) {
                std::stringstream ss(line);
                std::string idStr;
                if (std::getline(ss, idStr, ',')) {
                    int id = std::stoi(idStr);
                    if (id > maxId) maxId = id;
                }
            }
        }
        nextId = maxId + 1;
    }

public:
    FileTransactionRepository(const std::string& filename) : filename(filename) {
        loadNextId();
    }

    bool save(const Transaction& entity) override {
        std::ofstream file(filename, std::ios::app);
        if (!file.is_open()) return false;

        Transaction toSave = entity;
        if (toSave.getId() == 0) {
            toSave.setId(getNextId());
        }

        file << toSave.getId() << ","
            << toSave.getAmount() << ","
            << toSave.getCategory() << ","
            << toSave.getDescription() << ","
            << toSave.getDate() << ","
            << toSave.getTypeString() << "\n";

        return true;
    }

    bool remove(int id) override {
        auto all = findAll();
        std::ofstream file(filename);
        if (!file.is_open()) return false;

        file << "id,amount,category,description,date,type\n";
        bool found = false;
        for (const auto& transaction : all) {
            if (transaction.getId() != id) {
                file << transaction.getId() << ","
                    << transaction.getAmount() << ","
                    << transaction.getCategory() << ","
                    << transaction.getDescription() << ","
                    << transaction.getDate() << ","
                    << transaction.getTypeString() << "\n";
            }
            else {
                found = true;
            }
        }
        return found;
    }

    std::vector<Transaction> findAll() override {
        std::vector<Transaction> transactions;
        std::ifstream file(filename);
        std::string line;

        if (!file.is_open()) return transactions;

        if (std::getline(file, line)) { // Skip header
            while (std::getline(file, line)) {
                std::stringstream ss(line);
                std::string idStr, amountStr, category, description, date, typeStr;

                if (std::getline(ss, idStr, ',') &&
                    std::getline(ss, amountStr, ',') &&
                    std::getline(ss, category, ',') &&
                    std::getline(ss, description, ',') &&
                    std::getline(ss, date, ',') &&
                    std::getline(ss, typeStr, ',')) {

                    Transaction transaction(
                        std::stoi(idStr),
                        std::stod(amountStr),
                        category,
                        description,
                        date,
                        Transaction::stringToType(typeStr)
                    );
                    transactions.push_back(transaction);
                }
            }
        }
        return transactions;
    }

    Transaction* findById(int id) override {
        auto all = findAll();
        for (auto& transaction : all) {
            if (transaction.getId() == id) {
                return new Transaction(transaction);
            }
        }
        return nullptr;
    }

    std::vector<Transaction> findByField(const std::string& fieldName, const std::string& value) override {
        auto all = findAll();
        std::vector<Transaction> result;

        for (const auto& transaction : all) {
            if (fieldName == "category" && transaction.getCategory() == value) {
                result.push_back(transaction);
            }
            else if (fieldName == "type" && transaction.getTypeString() == value) {
                result.push_back(transaction);
            }
        }
        return result;
    }
};

class FileBudgetRepository : public IRepository<Budget> {
private:
    std::string filename;
    int nextId;

    int getNextId() {
        return nextId++;
    }

    void loadNextId() {
        std::ifstream file(filename);
        std::string line;
        int maxId = 0;

        if (std::getline(file, line)) {
            while (std::getline(file, line)) {
                std::stringstream ss(line);
                std::string idStr;
                if (std::getline(ss, idStr, ',')) {
                    int id = std::stoi(idStr);
                    if (id > maxId) maxId = id;
                }
            }
        }
        nextId = maxId + 1;
    }

public:
    FileBudgetRepository(const std::string& filename) : filename(filename) {
        loadNextId();
    }

    bool save(const Budget& entity) override {
        auto all = findAll();
        std::ofstream file(filename);
        if (!file.is_open()) return false;

        file << "id,category,limit,spent\n";

        bool updated = false;
        Budget toSave = entity;
        if (toSave.getId() == 0) {
            toSave.setId(getNextId());
        }

        for (auto& budget : all) {
            if (budget.getId() == toSave.getId()) {
                file << toSave.getId() << ","
                    << toSave.getCategory() << ","
                    << toSave.getLimit() << ","
                    << toSave.getSpent() << "\n";
                updated = true;
            }
            else {
                file << budget.getId() << ","
                    << budget.getCategory() << ","
                    << budget.getLimit() << ","
                    << budget.getSpent() << "\n";
            }
        }

        if (!updated) {
            file << toSave.getId() << ","
                << toSave.getCategory() << ","
                << toSave.getLimit() << ","
                << toSave.getSpent() << "\n";
        }

        return true;
    }

    bool remove(int id) override {
        auto all = findAll();
        std::ofstream file(filename);
        if (!file.is_open()) return false;

        file << "id,category,limit,spent\n";
        bool found = false;
        for (const auto& budget : all) {
            if (budget.getId() != id) {
                file << budget.getId() << ","
                    << budget.getCategory() << ","
                    << budget.getLimit() << ","
                    << budget.getSpent() << "\n";
            }
            else {
                found = true;
            }
        }
        return found;
    }

    std::vector<Budget> findAll() override {
        std::vector<Budget> budgets;
        std::ifstream file(filename);
        std::string line;

        if (!file.is_open()) return budgets;

        if (std::getline(file, line)) {
            while (std::getline(file, line)) {
                std::stringstream ss(line);
                std::string idStr, category, limitStr, spentStr;

                if (std::getline(ss, idStr, ',') &&
                    std::getline(ss, category, ',') &&
                    std::getline(ss, limitStr, ',') &&
                    std::getline(ss, spentStr, ',')) {

                    Budget budget(
                        std::stoi(idStr),
                        category,
                        std::stod(limitStr),
                        std::stod(spentStr)
                    );
                    budgets.push_back(budget);
                }
            }
        }
        return budgets;
    }

    Budget* findById(int id) override {
        auto all = findAll();
        for (auto& budget : all) {
            if (budget.getId() == id) {
                return new Budget(budget);
            }
        }
        return nullptr;
    }

    std::vector<Budget> findByField(const std::string& fieldName, const std::string& value) override {
        auto all = findAll();
        std::vector<Budget> result;

        for (const auto& budget : all) {
            if (fieldName == "category" && budget.getCategory() == value) {
                result.push_back(budget);
            }
        }
        return result;
    }
};

class DataStorage {
private:
    std::unique_ptr<IRepository<Transaction>> transactionRepo;
    std::unique_ptr<IRepository<Budget>> budgetRepo;

public:
    DataStorage() {
        transactionRepo = std::make_unique<FileTransactionRepository>("transactions.csv");
        budgetRepo = std::make_unique<FileBudgetRepository>("budgets.csv");
        initializeSampleData();
    }

    std::string getCurrentDate() {
        std::time_t t = std::time(nullptr);
        std::tm now;
#ifdef _WIN32
        localtime_s(&now, &t);
#else
        localtime_r(&t, &now);
#endif
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(2) << now.tm_mday << "."
            << std::setw(2) << (now.tm_mon + 1) << "."
            << (now.tm_year + 1900);
        return oss.str();
    }

    void initializeSampleData() {
        auto transactions = transactionRepo->findAll();
        auto budgets = budgetRepo->findAll();

        if (transactions.empty() && budgets.empty()) {
            transactionRepo->save(Transaction(0, 500.0, "Wynagrodzenie", "Pensja pazdziernik", getCurrentDate(), Transaction::Type::INCOME));
            transactionRepo->save(Transaction(0, 150.0, "Zakupy", "Biedronka", getCurrentDate(), Transaction::Type::EXPENSE));
            budgetRepo->save(Budget(0, "Zakupy", 400.0, 150.0));
            budgetRepo->save(Budget(0, "Rozrywka", 200.0, 0.0));
        }
    }

    void addTransaction(double amount, const std::string& category,
        const std::string& description, const std::string& date,
        Transaction::Type type) {
        Transaction transaction(0, amount, category, description, date, type);
        transactionRepo->save(transaction);

        if (type == Transaction::Type::EXPENSE) {
            auto budgets = budgetRepo->findByField("category", category);
            if (!budgets.empty()) {
                Budget budget = budgets[0];
                budget.addSpending(amount);
                budgetRepo->save(budget);
            }
        }
    }

    std::vector<Transaction> getAllTransactions() const {
        return transactionRepo->findAll();
    }

    std::vector<Transaction> getTransactionsByCategory(const std::string& category) const {
        return transactionRepo->findByField("category", category);
    }

    void addBudget(const std::string& category, double limit) {
        Budget budget(0, category, limit, 0.0);
        budgetRepo->save(budget);
    }

    std::vector<Budget> getAllBudgets() const {
        return budgetRepo->findAll();
    }

    Budget* getBudgetByCategory(const std::string& category) {
        auto budgets = budgetRepo->findByField("category", category);
        if (!budgets.empty()) {
            return new Budget(budgets[0]);
        }
        return nullptr;
    }

    void updateBudgetLimit(const std::string& category, double newLimit) {
        auto budgets = budgetRepo->findByField("category", category);
        if (!budgets.empty()) {
            Budget budget = budgets[0];
            budget.setLimit(newLimit);
            budgetRepo->save(budget);
        }
    }

    double getTotalIncome() const {
        double total = 0;
        auto transactions = transactionRepo->findAll();
        for (const auto& transaction : transactions) {
            if (transaction.getType() == Transaction::Type::INCOME) {
                total += transaction.getAmount();
            }
        }
        return total;
    }

    double getTotalExpenses() const {
        double total = 0;
        auto transactions = transactionRepo->findAll();
        for (const auto& transaction : transactions) {
            if (transaction.getType() == Transaction::Type::EXPENSE) {
                total += transaction.getAmount();
            }
        }
        return total;
    }

    double getBalance() const {
        return getTotalIncome() - getTotalExpenses();
    }
};

class AuthService {
private:
    std::string pinHash;
    const std::string authFile = "auth.dat";
    bool isAuthenticated;

    std::string hashPin(const std::string& pin) {
        return std::to_string(std::hash<std::string>{}(pin));
    }

    void loadAuthData() {
        std::ifstream file(authFile);
        if (file.is_open()) {
            std::getline(file, pinHash);
            file.close();
        }
    }

    void saveAuthData() {
        std::ofstream file(authFile);
        if (file.is_open()) {
            file << pinHash;
            file.close();
        }
    }

public:
    AuthService() : isAuthenticated(false) {
        loadAuthData();
    }

    bool isFirstRun() const {
        return pinHash.empty();
    }

    bool setPin(const std::string& newPin) {
        if (newPin.length() != 4) {
            std::cout << "PIN musi miec 4 cyfry!" << std::endl;
            return false;
        }

        for (char c : newPin) {
            if (!isdigit(c)) {
                std::cout << "PIN moze zawierac tylko cyfry!" << std::endl;
                return false;
            }
        }

        pinHash = hashPin(newPin);
        saveAuthData();
        std::cout << "PIN ustawiony pomyslnie!" << std::endl;
        return true;
    }

    bool authenticate(const std::string& pin) {
        if (pinHash == hashPin(pin)) {
            isAuthenticated = true;
            std::cout << "Autoryzacja pomyslna!" << std::endl;
            return true;
        }
        else {
            std::cout << "Nieprawidlowy PIN!" << std::endl;
            return false;
        }
    }

    bool checkAuth() const {
        return isAuthenticated;
    }

    void logout() {
        isAuthenticated = false;
        std::cout << "Wylogowano pomyslnie." << std::endl;
    }
};

class BudgetManager {
private:
    DataStorage dataStorage;

public:
    void addIncome(double amount, const std::string& category, const std::string& description) {
        dataStorage.addTransaction(amount, category, description,
            dataStorage.getCurrentDate(), Transaction::Type::INCOME);
        std::cout << "Dodano przychod: " << amount << " PLN" << std::endl;
    }

    void addExpense(double amount, const std::string& category, const std::string& description) {
        Budget* budget = dataStorage.getBudgetByCategory(category);
        if (budget && (budget->getRemaining() - amount) < 0) {
            std::cout << "Ostrzezenie: Ten wydatek przekroczy budzet dla kategorii '"
                << category << "'!" << std::endl;
        }

        dataStorage.addTransaction(amount, category, description,
            dataStorage.getCurrentDate(), Transaction::Type::EXPENSE);
        std::cout << "Dodano wydatek: " << amount << " PLN" << std::endl;

        delete budget;
    }

    void addBudget(const std::string& category, double limit) {
        dataStorage.addBudget(category, limit);
        std::cout << "Dodano budzet: " << category << " - " << limit << " PLN" << std::endl;
    }

    void displayAllTransactions() {
        auto transactions = dataStorage.getAllTransactions();
        if (transactions.empty()) {
            std::cout << "Brak transakcji." << std::endl;
            return;
        }

        std::cout << "\nWSZYSTKIE TRANSAKCJE:" << std::endl;
        std::cout << "==========================================" << std::endl;
        for (const auto& transaction : transactions) {
            transaction.display();
        }
    }

    void displayBudgets() {
        auto budgets = dataStorage.getAllBudgets();
        if (budgets.empty()) {
            std::cout << "Brak zdefiniowanych budzetow." << std::endl;
            return;
        }

        std::cout << "\nBUDZETY:" << std::endl;
        std::cout << "==========================================" << std::endl;
        for (const auto& budget : budgets) {
            budget.display();
            std::cout << "------------------------------------------" << std::endl;
        }
    }

    void displayFinancialSummary() {
        double income = dataStorage.getTotalIncome();
        double expenses = dataStorage.getTotalExpenses();
        double balance = dataStorage.getBalance();

        std::cout << "\nPODSUMOWANIE FINANSOWE:" << std::endl;
        std::cout << "==========================================" << std::endl;
        std::cout << "Laczne przychody: " << income << " PLN" << std::endl;
        std::cout << "Laczne wydatki: " << expenses << " PLN" << std::endl;
        std::cout << "Saldo: " << balance << " PLN" << std::endl;

        if (balance > 0) {
            std::cout << "Stan konta: dodatni" << std::endl;
        }
        else if (balance < 0) {
            std::cout << "Stan konta: ujemny" << std::endl;
        }
        else {
            std::cout << "Stan konta: zerowy" << std::endl;
        }
    }

    void displayCategoryReport() {
        auto transactions = dataStorage.getAllTransactions();
        auto budgets = dataStorage.getAllBudgets();

        std::cout << "\nRAPORT KATEGORII:" << std::endl;
        std::cout << "==========================================" << std::endl;

        for (const auto& budget : budgets) {
            std::string category = budget.getCategory();
            auto catTransactions = dataStorage.getTransactionsByCategory(category);

            double catIncome = 0;
            double catExpenses = 0;

            for (const auto& trans : catTransactions) {
                if (trans.getType() == Transaction::Type::INCOME) {
                    catIncome += trans.getAmount();
                }
                else {
                    catExpenses += trans.getAmount();
                }
            }

            std::cout << "Kategoria: " << category << std::endl;
            std::cout << "  Przychody: " << catIncome << " PLN" << std::endl;
            std::cout << "  Wydatki: " << catExpenses << " PLN" << std::endl;
            std::cout << "  Budzet: " << budget.getLimit() << " PLN" << std::endl;
            std::cout << "  Wydano: " << budget.getSpent() << " PLN" << std::endl;
            std::cout << "  Pozostalo: " << budget.getRemaining() << " PLN" << std::endl;
            std::cout << "------------------------------------------" << std::endl;
        }
    }
};

class ConsoleUI {
private:
    BudgetManager manager;
    AuthService authService;

    void displayMenu() {
        std::cout << "\nBUDGET MANAGER - PROTO TYP #1" << std::endl;
        std::cout << "==========================================" << std::endl;
        std::cout << "1. Dodaj przychod" << std::endl;
        std::cout << "2. Dodaj wydatek" << std::endl;
        std::cout << "3. Przegladaj transakcje" << std::endl;
        std::cout << "4. Zarzadzaj budzetami" << std::endl;
        std::cout << "5. Podsumowanie finansowe" << std::endl;
        std::cout << "6. Raport kategorii" << std::endl;
        std::cout << "7. Wyloguj" << std::endl;
        std::cout << "0. Wyjscie" << std::endl;
        std::cout << "==========================================" << std::endl;
        std::cout << "Wybierz opcje: ";
    }

    void handleAddIncome() {
        double amount;
        std::string category, description;

        std::cout << "Kwota: ";
        std::cin >> amount;
        std::cin.ignore();

        std::cout << "Kategoria: ";
        std::getline(std::cin, category);

        std::cout << "Opis: ";
        std::getline(std::cin, description);

        manager.addIncome(amount, category, description);
    }

    void handleAddExpense() {
        double amount;
        std::string category, description;

        std::cout << "Kwota: ";
        std::cin >> amount;
        std::cin.ignore();

        std::cout << "Kategoria: ";
        std::getline(std::cin, category);

        std::cout << "Opis: ";
        std::getline(std::cin, description);

        manager.addExpense(amount, category, description);
    }

    void handleManageBudgets() {
        int choice;
        std::cout << "\nZARZADZANIE BUDZETAMI:" << std::endl;
        std::cout << "1. Dodaj nowy budzet" << std::endl;
        std::cout << "2. Przegladaj budzety" << std::endl;
        std::cout << "Wybierz opcje: ";
        std::cin >> choice;

        if (choice == 1) {
            std::string category;
            double limit;

            std::cin.ignore();
            std::cout << "Kategoria: ";
            std::getline(std::cin, category);

            std::cout << "Limit miesieczny: ";
            std::cin >> limit;

            manager.addBudget(category, limit);
        }
        else if (choice == 2) {
            manager.displayBudgets();
        }
    }

    bool setupPin() {
        std::string pin;
        std::cout << "PIERWSZE URUCHOMIENIE - KONFIGURACJA" << std::endl;
        std::cout << "Ustaw 4-cyfrowy PIN do aplikacji: ";
        std::cin >> pin;

        return authService.setPin(pin);
    }

    bool login() {
        std::string pin;
        std::cout << "Podaj PIN: ";
        std::cin >> pin;

        return authService.authenticate(pin);
    }

public:
    void run() {
        std::cout << "URUCHAMIANIE BUDGET MANAGER - PROTO TYP #1" << std::endl;
        std::cout << "Architektura 3-warstwowa: Prezentacja -> Logika -> Dane" << std::endl;

        // Konfiguracja i autoryzacja
        if (authService.isFirstRun()) {
            if (!setupPin()) {
                return;
            }
        }

        if (!login()) {
            std::cout << "Bledny PIN. Aplikacja zostanie zamknieta." << std::endl;
            return;
        }

        // Główna pętla aplikacji
        int choice;
        do {
            displayMenu();
            std::cin >> choice;
            std::cin.ignore();

            switch (choice) {
            case 1:
                handleAddIncome();
                break;
            case 2:
                handleAddExpense();
                break;
            case 3:
                manager.displayAllTransactions();
                break;
            case 4:
                handleManageBudgets();
                break;
            case 5:
                manager.displayFinancialSummary();
                break;
            case 6:
                manager.displayCategoryReport();
                break;
            case 7:
                authService.logout();
                if (!login()) {
                    choice = 0; 
                }
                break;
            case 0:
                std::cout << "Zamykanie aplikacji..." << std::endl;
                break;
            default:
                std::cout << "Nieprawidlowy wybor!" << std::endl;
            }

        } while (choice != 0);
    }
};

int main() {
    ConsoleUI app;
    app.run();
    return 0;
}