#include "DatabaseManager.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

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
        "password TEXT NOT NULL CHECK (password != ''))");

    // Create profiles table with user association
    tableCreationQuery.exec("CREATE TABLE IF NOT EXISTS profiles"
        "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "profile_name TEXT NOT NULL CHECK (profile_name != ''), "
        "user_id INTEGER NOT NULL, "
        "FOREIGN KEY(user_id) REFERENCES users(id) ON DELETE CASCADE)");

    // Create categories table with profile scoping
    tableCreationQuery.exec("CREATE TABLE IF NOT EXISTS category"
        "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "category_name TEXT NOT NULL CHECK (category_name != ''), "
        "profile_id INTEGER NOT NULL, "
        "FOREIGN KEY (profile_id) REFERENCES profiles(id) ON DELETE CASCADE"
        ")");
    // Create trigger to automatically add default "None" category for new profiles
    tableCreationQuery.exec("CREATE TRIGGER IF NOT EXISTS insertDefaultCategory "
        "AFTER INSERT ON profiles "
        "FOR EACH ROW BEGIN INSERT INTO category (category_name, profile_id) "
        "VALUES ('None', NEW.id); "
        "END;");
    //Create trigger to prevent deletion of default category
    //Need to create workaround to make 1:N connection to 'None' category
    /*tableCreationQuery.exec("CREATE TRIGGER IF NOT EXISTS onDeletionOfDefaultCategory "
        "BEFORE DELETE ON category "
        "FOR EACH ROW WHEN OLD.category_name = 'None' "
        "BEGIN SELECT RAISE(ABORT, 'Cannot delete default category'); "
        "END;");*/

    // Create transactions table with relationships to profiles and categories
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
                qDebug() << "Demo admin user has been added.";
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