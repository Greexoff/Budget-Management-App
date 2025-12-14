#include "DatabaseManager.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QCryptographicHash>
#include <QUuid>

/**
 * @brief Constructs the DatabaseManager and initializes the database schema
 *
 * This constructor performs the following initialization tasks:
 * 1. Establishes SQLite database connection
 * 2. Creates all required tables if they don't exist
 * 3. Sets up database triggers for automatic default category creation
 * 4. Creates a demo admin user for development purposes
 */
DatabaseManager::DatabaseManager() {
    datebaseInstance = QSqlDatabase::addDatabase("QSQLITE");
    datebaseInstance.setDatabaseName("BudgetDatabase.db");

    if (!datebaseInstance.open()) {
        qDebug() << "Database Error:" << datebaseInstance.lastError().text();
    }
    else {
        qDebug() << "Database opened correctly.";
    }

    QSqlQuery tableCreationQuery;

    // Enable foreign key support for referential integrity
    tableCreationQuery.exec("PRAGMA foreign_keys = ON;");

    // Create users table for authentication
    tableCreationQuery.exec("CREATE TABLE IF NOT EXISTS users"
        "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username TEXT UNIQUE NOT NULL CHECK (username != ''),"
        "password_hash TEXT NOT NULL CHECK (password_hash != ''), "
        "salt TEXT NOT NULL CHECK (salt != ''))");

    // Create profiles table with user association
    tableCreationQuery.exec("CREATE TABLE IF NOT EXISTS profiles"
        "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "profile_name TEXT NOT NULL CHECK (profile_name != ''), "
        "user_id INTEGER NOT NULL, "
        "budget_limit REAL DEFAULT 0, " 
        "FOREIGN KEY(user_id) REFERENCES users(id) ON DELETE CASCADE)");
    // Create categories table with profile scoping
   tableCreationQuery.exec("CREATE TABLE IF NOT EXISTS category"
        "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "category_name TEXT NOT NULL CHECK (category_name != ''), "
        "profile_id INTEGER, "
        "FOREIGN KEY (profile_id) REFERENCES profiles(id) ON DELETE CASCADE"
        ")");
   // Create financialAccounts table with profile scoping
   tableCreationQuery.exec("CREATE TABLE IF NOT EXISTS financialAccount"
       "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
       "financialAccount_name TEXT NOT NULL CHECK (financialAccount_name != ''), "
       "financialAccount_type TEXT NOT NULL, "
       "financialAccount_balance REAL DEFAULT 0, "
       "profile_id INTEGER, "
       "FOREIGN KEY (profile_id) REFERENCES profiles(id) ON DELETE CASCADE"
       ")");
    // Insert default category into database
   tableCreationQuery.exec("INSERT OR IGNORE INTO category (id, category_name, profile_id) VALUES (1, 'None', NULL)");
   // Insert default financial account into database
   tableCreationQuery.exec("INSERT OR IGNORE INTO financialAccount (id, financialAccount_name, financialAccount_type, profile_id) VALUES (1, 'None', 'Default', NULL)");

    // Create transactions table with relationships to profiles, categories and financial account
    tableCreationQuery.exec("CREATE TABLE IF NOT EXISTS transactions"
        "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT NOT NULL CHECK (name != ''), "
        "type TEXT NOT NULL CHECK (type != ''), "
        "date TEXT NOT NULL CHECK (date != ''), "
        "description TEXT, "
        "amount REAL NOT NULL, "
        "category_id INTEGER DEFAULT 1, "
        "financialAccount_id INTEGER DEFAULT 1, "
        "profile_id INTEGER, "
        "FOREIGN KEY (profile_id) REFERENCES profiles(id) ON DELETE CASCADE, "
        "FOREIGN KEY (category_id) REFERENCES category(id) ON DELETE SET DEFAULT, "
        "FOREIGN KEY (financialAccount_id) REFERENCES financialAccount(id) ON DELETE SET DEFAULT"
        ")");

    // DEVELOPMENT: Create demo admin user for testing
    QSqlQuery adminQuery(datebaseInstance);
    adminQuery.prepare("SELECT COUNT(*) FROM users WHERE username = 'admin'");

    if (adminQuery.exec() && adminQuery.next()) {
        if (adminQuery.value(0).toInt() == 0) {

            QString password = "admin"; 

            QString salt = QUuid::createUuid().toString();

            QByteArray dataToHash = (password + salt).toUtf8();
            QString hashedPassword = QString(QCryptographicHash::hash(dataToHash, QCryptographicHash::Sha256).toHex());

            QSqlQuery insertAdmin(datebaseInstance);
            insertAdmin.prepare(
                "INSERT INTO users (username, password_hash, salt) "
                "VALUES ('admin', :hash, :salt)"
            );
            insertAdmin.bindValue(":hash", hashedPassword);
            insertAdmin.bindValue(":salt", salt);

            if (insertAdmin.exec()) {
                qDebug() << "Demo admin user has been added (Pass: admin).";
            }
            else {
                qDebug() << "Error adding admin user:" << insertAdmin.lastError();
            }
        }
        else {
            qDebug() << "Admin already exists - skipping.";
        }
    }
}

/**
 * @brief Returns the singleton instance of DatabaseManager
 * @return DatabaseManager& Reference to the singleton instance
 */
DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;
    return instance;
}

/**
 * @brief Provides access to the application's database connection
 * @return QSqlDatabase& Reference to the QSqlDatabase instance
 */
QSqlDatabase& DatabaseManager::database() {
    return datebaseInstance;
}