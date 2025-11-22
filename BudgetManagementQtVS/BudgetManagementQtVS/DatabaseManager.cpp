#include "DatabaseManager.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

DatabaseManager::DatabaseManager() {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("budget.db");

    if (!m_db.open()) {
        qDebug() << "DB Error:" << m_db.lastError().text();
    }
    else {
        qDebug() << "Database opened correctly!";
    }

    // Utworzymy przyk³adow¹ tabelê (do testów)
    QSqlQuery q;
    q.exec("CREATE TABLE IF NOT EXISTS test "
        "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT)");
}

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;
    return instance;
}

QSqlDatabase& DatabaseManager::db() {
    return m_db;
}