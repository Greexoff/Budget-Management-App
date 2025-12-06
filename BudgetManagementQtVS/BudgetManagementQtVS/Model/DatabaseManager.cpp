#include "DatabaseManager.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

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
    tableCreationQuery.exec("PRAGMA foreign_keys = ON;");//Allowing to create foreign keys
    //Creating table for users
    tableCreationQuery.exec("CREATE TABLE IF NOT EXISTS users"
        "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username TEXT UNIQUE NOT NULL CHECK (username != ''),"
        "password TEXT NOT NULL CHECK (password != ''))");

    //Creating table for profiles
    tableCreationQuery.exec("CREATE TABLE IF NOT EXISTS profiles"
        "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "profile_name TEXT UNIQUE NOT NULL CHECK (profile_name != ''), "
        "user_id INTEGER, "
        "FOREIGN KEY(user_id) REFERENCES users(id) ON DELETE CASCADE)");

    //Creating table for categories
    tableCreationQuery.exec("CREATE TABLE IF NOT EXISTS category"
        "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "category_name TEXT UNIQUE NOT NULL CHECK (category_name != ''))");
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

    //DO usuniecia jak juz bedzie wszystko dzialalo
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
    // do tego momentu
}

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;
    return instance;
}

QSqlDatabase& DatabaseManager::database() {
    return datebaseInstance;
}