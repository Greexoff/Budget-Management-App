/**
 * @file UserRepository.h
 * @brief Header file for the User Repository.
 */
#pragma once
#include <QCryptographicHash>
#include <QUuid>
#include <Model/Repositories/BaseRepository.h>

 /**
  * @class UserRepository
  * @brief Manages database operations for user authentication and registration.
  */
class UserRepository : public BaseRepository
{
public:
    UserRepository() = default;
    /**
     * @brief Authenticates a user by checking credentials against the database.
     * @param username The username to check.
     * @param password The raw password to verify.
     * @return The user ID if successful, -1 otherwise.
     */
    int getUserIdBasedOnUsername(QString username, QString password) const;

    /**
     * @brief Registers a new user with a hashed password and salt.
     * @param username The desired username.
     * @param password The raw password.
     * @return True if successful, false otherwise.
     */
    bool addUser(QString username, QString password) const;

    /**
     * @brief Deletes a user account by ID.
     * @param userId The ID of the user to remove.
     * @return True if successful, false otherwise.
     */
    bool removeUserById(int userId) const;

    /**
     * @brief Checks if a username is already taken.
     * @param username The username to query.
     * @return True if exists, false otherwise.
     */
    bool checkIfUserExists(const QString& username) const;
};