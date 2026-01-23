/**
 * @file DatabaseManager.h
 * @brief Header file for the Database Manager singleton.
 */
#pragma once
#include <QSqlDatabase>

 /**
  * @class DatabaseManager
  * @brief Singleton class responsible for initializing and managing the SQLite database connection.
  */
class DatabaseManager {
public:
    /**
     * @brief Accesses the single instance of the DatabaseManager.
     * @return Reference to the singleton instance.
     */
    static DatabaseManager& instance();

    /**
     * @brief Provides access to the QSqlDatabase object.
     * @return Reference to the active database connection.
     */
    QSqlDatabase& database();
private:
    /** @brief Private constructor to enforce Singleton pattern. */
    DatabaseManager();
    QSqlDatabase datebaseInstance;   ///< The internal Qt SQL database object.
};