#include "DatabaseManager.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

DatabaseManager::DatabaseManager() {
    datebaseInstance = QSqlDatabase::addDatabase("QSQLITE");
    datebaseInstance.setDatabaseName("BudgetDatabase.db");

    if (!datebaseInstance.open()) {
        qDebug() << "DB Error:" << datebaseInstance.lastError().text();
    }
    else {
        qDebug() << "Database opened correctly!";
    }

    QSqlQuery tableCreationQuery;
    tableCreationQuery.exec("CREATE TABLE IF NOT EXISTS BudgetDatabase "
        "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT)");
}

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;
    return instance;
}

QSqlDatabase& DatabaseManager::database() {
    return datebaseInstance;
}