#pragma once
#include <QSqlDatabase>

class DatabaseManager {
public:
    static DatabaseManager& instance();

    QSqlDatabase& database();

private:

    DatabaseManager();
    QSqlDatabase datebaseInstance;   
};