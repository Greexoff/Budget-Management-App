#pragma once

#include <QVector.h>
#include <QString.h>

#include <qsqldatabase.h>
#include <Model/Transaction.h>
#include <Model/DatabaseManager.h>
#include <Model/User&Profiles.h>
#include <Model/Category.h>

/**
 * @class BaseRepository
 * @brief Abstract base class providing database connection to derived repositories
 *
 * This class ensures all repository classes have access to the shared
 * database connection managed by the DatabaseManager singleton.
 */
class BaseRepository {
public:
    BaseRepository();
    virtual ~BaseRepository() = default;

protected:
    QSqlDatabase& database;  ///< Reference to the shared database connection
};

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
    int authenticateUser(QString username, QString password) const;

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

/**
 * @class ProfilesRepository
 * @brief Handles database operations for budget profile management
 */
class ProfilesRepository : public BaseRepository
{
public:
    ProfilesRepository() = default;

    /**
     * @brief Retrieves all profiles belonging to a specific user
     * @param userId ID of the user
     * @return Vector of Profile objects for the user
     */
    QVector<Profile> getProfilesByUserId(int userId) const;

    /**
     * @brief Creates a new profile for a user
     * @param userId ID of the owning user
     * @param profileName Name for the new profile
     * @return True if profile created successfully, false otherwise
     */
    bool addProfile(int userId, QString profileName);

    /**
     * @brief Deletes a profile by ID
     * @param profileId ID of profile to delete
     * @return True if profile deleted successfully, false otherwise
     */
    bool removeProfileById(int profileId);
};

/**
 * @class TransactionRepository
 * @brief Handles database operations for financial transaction management
 */
class TransactionRepository : public BaseRepository
{
public:
    TransactionRepository() = default;

    /**
     * @brief Retrieves all transactions from the database
     * @return Vector of all Transaction objects
     */
    QVector<Transaction> getAll() const;

    /**
     * @brief Adds a new transaction to the database
     * @param transaction Transaction object to add
     * @return True if transaction added successfully, false otherwise
     */
    bool add(const Transaction& transaction);

    /**
     * @brief Deletes a transaction by ID
     * @param id ID of transaction to delete
     * @return True if transaction deleted successfully, false otherwise
     */
    bool removeById(int id);

    /**
     * @brief Retrieves all transactions for a specific profile
     * @param profileId ID of the profile
     * @return Vector of Transaction objects belonging to the profile
     */
    QVector<Transaction> getAllForProfile(int profileId) const;
};

/**
 * @class CategoryRepository
 * @brief Handles database operations for category management
 */
class CategoryRepository : public BaseRepository
{
public:
    CategoryRepository() = default;

    /**
     * @brief Retrieves all categories for a specific profile
     * @param profileId ID of the profile
     * @return Vector of Category objects belonging to the profile
     */
    QVector<Category> getAllCategories(int profileId) const;

    /**
     * @brief Creates a new category for a profile
     * @param category Name of the category to create
     * @param profileId ID of the profile that will own the category
     * @return True if category created successfully, false otherwise
     */
    bool addCategory(const QString& category, int profileId);

    /**
     * @brief Deletes a category by ID
     * @param categoryId ID of category to delete
     * @return True if category deleted successfully, false otherwise
     */
    bool removeCategoryUsingId(int categoryId);

    /**
     * @brief Retrieves category name by ID
     * @param categoryId ID of the category
     * @return Category name as QString, empty string if category not found
     */
    QString getNameOfCategoryBasedOnId(int categoryId);
};