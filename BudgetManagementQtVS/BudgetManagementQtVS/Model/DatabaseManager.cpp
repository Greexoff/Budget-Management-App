/**
 * @file DatabaseManager.cpp
 * @brief Implementation of the Database Manager.
 */
#include "DatabaseManager.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QCryptographicHash>
#include <QUuid>

 /**
  * @brief Constructor.
  * - Opens the SQLite database connection.
  * - Enables foreign keys.
  * - Creates necessary tables (users, profiles, categories, financialAccount, transactions) if they don't exist.
  * - Inserts default 'None' records.
  * - Creates a default admin user for development purposes.
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

    tableCreationQuery.exec("PRAGMA foreign_keys = ON;");

    tableCreationQuery.exec("CREATE TABLE IF NOT EXISTS users"
        "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username TEXT UNIQUE NOT NULL CHECK (username != ''),"
        "password_hash TEXT NOT NULL CHECK (password_hash != ''), "
        "salt TEXT NOT NULL CHECK (salt != ''))");

    tableCreationQuery.exec("CREATE TABLE IF NOT EXISTS profiles"
        "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "profile_name TEXT NOT NULL CHECK (profile_name != ''), "
        "user_id INTEGER NOT NULL, "
        "budget_limit REAL DEFAULT 0, " 
        "FOREIGN KEY(user_id) REFERENCES users(id) ON DELETE CASCADE)");

   tableCreationQuery.exec("CREATE TABLE IF NOT EXISTS category"
        "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "category_name TEXT NOT NULL CHECK (category_name != ''), "
        "profile_id INTEGER, "
        "FOREIGN KEY (profile_id) REFERENCES profiles(id) ON DELETE CASCADE"
        ")");

   tableCreationQuery.exec("CREATE TABLE IF NOT EXISTS financialAccount"
       "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
       "financialAccount_name TEXT NOT NULL CHECK (financialAccount_name != ''), "
       "financialAccount_type TEXT NOT NULL, "
       "financialAccount_balance REAL DEFAULT 0, "
       "profile_id INTEGER, "
       "FOREIGN KEY (profile_id) REFERENCES profiles(id) ON DELETE CASCADE"
       ")");

   tableCreationQuery.exec("INSERT OR IGNORE INTO category (id, category_name, profile_id) VALUES (1, 'None', NULL)");

   tableCreationQuery.exec("INSERT OR IGNORE INTO financialAccount (id, financialAccount_name, financialAccount_type, profile_id) VALUES (1, 'None', 'Default', NULL)");


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
/** @brief Returns the static singleton instance. */
DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;
    return instance;
}
/** @brief Returns the internal QSqlDatabase instance. */
QSqlDatabase& DatabaseManager::database() {
    return datebaseInstance;
}