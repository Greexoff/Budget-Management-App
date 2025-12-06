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
        "username TEXT UNIQUE NOT NULL,"
        "password TEXT NOT NULL)");
    //Creating table for profiles
    tableCreationQuery.exec("CREATE TABLE IF NOT EXISTS profiles"
        "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "profile_name TEXT UNIQUE NOT NULL, "
        "user_id INTEGER, "
        "FOREIGN KEY(user_id) REFERENCES users(id) ON DELETE CASCADE)");
    //Creating table for transactions
    tableCreationQuery.exec("CREATE TABLE IF NOT EXISTS transactions"
        "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT NOT NULL, "
        "type TEXT NOT NULL, "
        "date TEXT NOT NULL, "
        "description TEXT, "
        "amount REAL NOT NULL, "
        "category_id INTEGER DEFAULT 0, "//tutaj trzeba bedzie pewnie dorobic kolejny foreign key w ktoras strone   
        "profile_id INTEGER NOT NULL, "
        "FOREIGN KEY (profile_id) REFERENCES profiles(id) ON DELETE CASCADE"
        ")");
    //IN PROGRESS Creating table for category
    //IN PROGRESS Here make sql query for creating table for category with id, name, 
}

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;
    return instance;
}

QSqlDatabase& DatabaseManager::database() {
    return datebaseInstance;
}