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
 * @brief Base class providing database access to derived repositories
 *
 * This abstract base class ensures all repository classes have
 * access to the application's database connection.
 */
class BaseRepository {
public: 
	BaseRepository();
	virtual ~BaseRepository() = default;
protected:
	QSqlDatabase& database;		///< Reference to shared database connection
};

/**
 * @class UserRepository
 * @brief Handles database operations for user accounts
 *
 * Provides methods for user authentication, registration,
 * and account management.
 */
class UserRepository : public BaseRepository
{
public:
	UserRepository() = default;

	/**
	 * @brief Authenticates user credentials
	 * @param username User's login name
	 * @param password User's password
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
	 * @brief Deletes a user account
	 * @param userId ID of user to delete
	 * @return True if user deleted successfully, false otherwise
	 */
	bool removeUserById(int userId);
};

/**
 * @class ProfilesRepository
 * @brief Handles database operations for budget profiles
 *
 * Budget profiles allow users to separate transactions into
 * different contexts (e.g., personal, business).
 */
class ProfilesRepository : public BaseRepository
{
public:
	ProfilesRepository() = default;

	/**
	 * @brief Retrieves all profiles belonging to a user
	 * @param userId ID of the user
	 * @return Vector of Profile objects
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
	 * @brief Deletes a profile
	 * @param profileId ID of profile to delete
	 * @return True if profile deleted successfully, false otherwise
	 */
	bool removeProfileById(int profileId);
};

/**
 * @class TransactionRepository
 * @brief Handles database operations for financial transactions
 */
class TransactionRepository : public BaseRepository
{
public:
	TransactionRepository() = default;

	/**
	 * @brief Retrieves all transactions from database
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
 * @brief Handles database operations for transaction categories
 */
class CategoryRepository : public BaseRepository
{
public:
	CategoryRepository() = default;

	/**
	 * @brief Retrieves all categories from database
	 * @return Vector of all Category objects
	 */
	QVector<Category> getAllCategories() const;

	/**
	 * @brief Creates a new category
	 * @param category Name of the category to create
	 * @return True if category created successfully, false otherwise
	 */
	bool addCategory(const QString& category);

	/**
	 * @brief Deletes a category by ID
	 * @param categoryId ID of category to delete
	 * @return True if category deleted successfully, false otherwise
	 */
	bool removeCategoryUsingId(int categoryId);

	/**
	 * @brief Retrieves category name by ID
	 * @param categoryId ID of the category
	 * @return Category name, or empty string if not found
	 */
	QString getNameOfCategoryBasedOnId(int categoryId);
};

