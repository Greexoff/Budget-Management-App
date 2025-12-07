#include "DatabaseManager.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

/**
 * @brief Private constructor - initializes database and schema
 *
 * Performs the following tasks:
 * 1. Establishes SQLite database connection
 * 2. Creates required tables if they don't exist
 * 3. Inserts default data (categories, demo user)
 * 4. Enables foreign key constraints
 */
DatabaseManager::DatabaseManager() {
    datebaseInstance = QSqlDatabase::addDatabase("QSQLITE");
    datebaseInstance.setDatabaseName("BudgetDatabase.db");

    if (!datebaseInstance.open()) {
        qDebug() << "Database Error:" << datebaseInstance.lastError().text();
    }
    else {
        qDebug() << "Database opened correctly";
    }


    QSqlQuery tableCreationQuery;

    // Enable foreign key support
    tableCreationQuery.exec("PRAGMA foreign_keys = ON;");

    // Create users table for authentication
    tableCreationQuery.exec("CREATE TABLE IF NOT EXISTS users"
        "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username TEXT UNIQUE NOT NULL CHECK (username != ''),"
        "password TEXT NOT NULL CHECK (password != ''))");

    // Create profiles table
    tableCreationQuery.exec("CREATE TABLE IF NOT EXISTS profiles"
        "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "profile_name TEXT UNIQUE NOT NULL CHECK (profile_name != ''), "
        "user_id INTEGER, "
        "FOREIGN KEY(user_id) REFERENCES users(id) ON DELETE CASCADE)");

    // Create categories table for transaction classification
    tableCreationQuery.exec("CREATE TABLE IF NOT EXISTS category"
        "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "category_name TEXT UNIQUE NOT NULL CHECK (category_name != ''))");

    // Insert default "No Category" entry
    tableCreationQuery.exec("INSERT OR IGNORE INTO category (id, category_name) VALUES (1, 'Brak Kategorii')");

    //Creating table for transactions
    tableCreationQuery.exec("CREATE TABLE IF NOT EXISTS transactions"
        "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT NOT NULL CHECK (name != ''), "
        "type TEXT NOT NULL CHECK (type != ''), "
        "date TEXT NOT NULL CHECK (date != ''), "
        "description TEXT, "
        "amount REAL NOT NULL, "
        "category_id INTEGER DEFAULT 1, "
        "profile_id INTEGER, "
        "FOREIGN KEY (profile_id) REFERENCES profiles(id) ON DELETE CASCADE, "
        "FOREIGN KEY (category_id) REFERENCES category(id) ON DELETE SET DEFAULT"
        ")");

    // DEVELOPMENT: Create demo admin user for testing
    QSqlQuery adminQuery(datebaseInstance);
    adminQuery.prepare("SELECT COUNT(*) FROM users WHERE username = 'admin'");

    if (adminQuery.exec() && adminQuery.next()) {
        if (adminQuery.value(0).toInt() == 0) {
            QSqlQuery insertAdmin(datebaseInstance);
            insertAdmin.prepare(
                "INSERT INTO users (username, password) "
                "VALUES ('admin', 'admin')"
            );

            if (insertAdmin.exec()) {
                qDebug() << "Użytkownik admin został dodany.";
            }
            else {
                qDebug() << "Błąd przy dodawaniu admina:" << insertAdmin.lastError();
            }
        }
        else {
            qDebug() << "Admin już istnieje – pomijam.";
        }
    }
}

/**
 * @brief Returns the singleton instance
 * @return DatabaseManager& Reference to the singleton instance
 */
DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;
    return instance;
}

/**
 * @brief Provides access to the database connection
 * @return QSqlDatabase& Reference to the database connection
 */
QSqlDatabase& DatabaseManager::database() {
    return datebaseInstance;
}