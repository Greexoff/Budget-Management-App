#include <Model/Repositories/UserRepository.h>

/**
 * @brief Creates a new user account
 * @param username Desired username
 * @param password Desired password
 * @return True if user created successfully, false otherwise
 */
bool UserRepository::addUser(QString username, QString password)
{
    QString salt = QUuid::createUuid().toString();

    QByteArray dataToHash = (password + salt).toUtf8();
    QString hashedPassword = QString(QCryptographicHash::hash(dataToHash, QCryptographicHash::Sha256).toHex());

    QSqlQuery query(database);
    query.prepare("INSERT INTO users (username, password_hash, salt) VALUES (:username, :password_hash, :salt)");

    query.bindValue(":username", username);
    query.bindValue(":password_hash", hashedPassword);
    query.bindValue(":salt", salt);

    if (!query.exec()) {
        qDebug() << "Registration error:" << query.lastError().text();
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
    query.prepare("SELECT id, password_hash, salt FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (query.exec() && query.next()) {
        int userId = query.value(0).toInt();
        QString storedHash = query.value(1).toString();
        QString storedSalt = query.value(2).toString();

        QByteArray dataToHash = (password + storedSalt).toUtf8();
        QString calculatedHash = QString(QCryptographicHash::hash(dataToHash, QCryptographicHash::Sha256).toHex());

        if (storedHash == calculatedHash) {
            return userId; 
        }
    }

    return -1;
}
