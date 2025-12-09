#pragma once
#include <Model/Repositories/BaseRepository.h>
/**
 * @class UserRepository
 * @brief Handles database operations for user account management
 *
 * Provides methods for user authentication, registration, and account deletion.
 */
class UserRepository : public BaseRepository
{
public:
    UserRepository() = default;

    /**
     * @brief Authenticates user credentials against the database
     * @param username User's login name
     * @param password User's password (plaintext - for development only)
     * @return User ID if authentication successful, -1 otherwise
     */
    int getUserIdBasedOnUsername(QString username, QString password) const;

    /**
     * @brief Creates a new user account
     * @param username Desired username (must be unique)
     * @param password Desired password
     * @return True if user created successfully, false otherwise
     */
    bool addUser(QString username, QString password);

    /**
     * @brief Deletes a user account by ID
     * @param userId ID of user to delete
     * @return True if user deleted successfully, false otherwise
     */
    bool removeUserById(int userId);
};