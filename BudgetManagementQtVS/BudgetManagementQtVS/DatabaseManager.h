#pragma once
#include <QSqlDatabase>

class DatabaseManager {
public:
    static DatabaseManager& instance();
    QSqlDatabase& db();

private:
    DatabaseManager();
    QSqlDatabase m_db;
};