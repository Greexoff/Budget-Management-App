#pragma once
#include <QSqlDatabase>

/**
 * @class DatabaseManager
 * @brief Singleton class for centralized database connection management
 *
 * This class ensures a single database connection is used throughout
 * the application. It handles database initialization, schema creation,
 * and provides access to the QSqlDatabase instance.
 */
class DatabaseManager {
public:
    static DatabaseManager& instance();

    /**
     * @brief Returns the singleton instance of DatabaseManager
     * @return Reference to the singleton instance
     */
    QSqlDatabase& database();

private:
    /**
     * @brief Private constructor for singleton pattern
     *
     * Initializes the SQLite database connection, creates necessary
     * tables if they don't exist, and sets up default data.
     */
    DatabaseManager();
    QSqlDatabase datebaseInstance;      ///< The SQLite database connection
};