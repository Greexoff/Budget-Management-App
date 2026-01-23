/**
 * @file User.h
 * @brief Header file for the User model class.
 */
#pragma once
#include<QString>

 /**
  * @class User
  * @brief Represents a registered system user.
  */
class User
{
public:
    /**
     * @brief Constructs a User object.
     * @param userId Unique User ID.
     * @param username Login username.
     * @param password Hashed password string.
     */
    User(int userId, QString username, QString password);

    /** @brief Returns User ID. */
    int getUserId() const;

    /** @brief Returns stored password hash. */
    QString getPassword() const;

    /** @brief Returns username. */
    QString getUsername() const;
private:
    int userId;       ///< Unique identifier.
    QString username; ///< Login name.
    QString password; ///< Password hash.
};
