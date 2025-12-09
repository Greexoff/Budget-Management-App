#include <Model/Repositories/UserRepository.h>

/**
 * @brief Creates a new user account
 * @param username Desired username
 * @param password Desired password
 * @return True if user created successfully, false otherwise
 */
bool UserRepository::addUser(QString username, QString password)
{
    QSqlQuery query(database);
    query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (!query.exec())
    {
        qDebug() << "UserRepo::adding user to database error:" << query.lastError().text();
        return false;
    }

    return true;
}

/**
 * @brief Deletes a user account by ID
 * @param userId ID of user to delete
 * @return True if user deleted successfully, false otherwise
 */
bool UserRepository::removeUserById(int userId)
{
    QSqlQuery query(database);
    query.prepare("DELETE FROM users WHERE id = :id");
    query.bindValue(":id", userId);

    if (!query.exec())
    {
        qDebug() << "UserRepo::removing user to database error:" << query.lastError().text();
        return false;
    }

    return true;
}

/**
 * @brief Authenticates user credentials against the database
 *
 * Performs a simple username/password comparison.
 *
 * @param username User's login name
 * @param password User's password
 * @return User ID if authentication successful, -1 otherwise
 */
int UserRepository::getUserIdBasedOnUsername(QString username, QString password) const
{
    QSqlQuery query(database);
    query.prepare("SELECT id, password FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec())
    {
        qDebug() << "UserRepo::authentication error:" << query.lastError().text();
        return -1;
    }

    if (query.next())
    {
        QString storedPassword = query.value(1).toString();
        if (storedPassword == password)
        {
            return query.value(0).toInt();
        }
    }

    return -1;
}
