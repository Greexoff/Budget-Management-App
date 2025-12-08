#include "Repositories.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

/**
 * @brief Constructs BaseRepository with database connection
 */
BaseRepository::BaseRepository() : database(DatabaseManager::instance().database()) {}

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
 * @brief Retrieves all profiles belonging to a specific user
 * @param userId ID of the user
 * @return Vector of Profile objects for the user
 */
QVector<Profile> ProfilesRepository::getProfilesByUserId(int userId) const
{
    QVector<Profile> foundProfiles;

    QSqlQuery query(database);
    query.prepare("SELECT id, profile_name, user_id FROM profiles WHERE user_id = :userId");
    query.bindValue(":userId", userId);

    if (!query.exec())
    {
        qDebug() << "ProfilesRepo::getting profiles that belong to user error:" << query.lastError().text();
        return foundProfiles;
    }

    while (query.next())
    {
        int profileId = query.value(0).toInt();
        QString profileName = query.value(1).toString();
        int profileUserId = query.value(2).toInt();

        Profile profile(profileId, profileUserId, profileName);
        foundProfiles.append(profile);
    }

    return foundProfiles;
}

/**
 * @brief Creates a new profile for a user
 *
 * Profile names must be unique across all users. The database trigger
 * `insertDefaultCategory` will automatically create a default "None" category
 * for the new profile.
 *
 * @param userId ID of the owning user
 * @param profileName Name for the new profile
 * @return True if profile created successfully, false otherwise
 */
bool ProfilesRepository::addProfile(int userId, QString profileName)
{
    QSqlQuery query(database);
    query.prepare("INSERT INTO profiles (profile_name, user_id) VALUES (:profileName, :userId)");
    query.bindValue(":profileName", profileName);
    query.bindValue(":userId", userId);

    if (!query.exec())
    {
        qDebug() << "ProfilesRepo::adding profile to database error:" << query.lastError().text();
        return false;
    }

    return true;
}

/**
 * @brief Deletes a profile by ID
 * @param profileId ID of profile to delete
 * @return True if profile deleted successfully, false otherwise
 */
bool ProfilesRepository::removeProfileById(int profileId)
{
    QSqlQuery query(database);
    query.prepare("DELETE FROM profiles WHERE id = :id");
    query.bindValue(":id", profileId);

    if (!query.exec())
    {
        qDebug() << "ProfilesRepo::removing profile to database error:" << query.lastError().text();
        return false;
    }
    return true;
}

/**
 * @brief Retrieves all transactions for a specific profile
 * @param profileId ID of the profile
 * @return Vector of Transaction objects belonging to the profile
 */
QVector<Transaction> TransactionRepository::getAllProfileTransaction(int profileId) const
{
    QVector<Transaction> result;
    QSqlQuery query(database);

    query.prepare(
        "SELECT id, name, date, description, amount, type, category_id, profile_id "
        "FROM transactions WHERE profile_id = :profileId"
    );
    query.bindValue(":profileId", profileId);

    if (!query.exec())
    {
        qDebug() << "Transaction retrieval for profile failed:" << query.lastError().text();
        return result;
    }

    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        QString dateStr = query.value(2).toString();
        QString description = query.value(3).toString();
        double amount = query.value(4).toDouble();
        QString typeStr = query.value(5).toString();
        int categoryId = query.value(6).toInt();
        int profileId = query.value(7).toInt();

        QDate date = QDate::fromString(dateStr, "yyyy-MM-dd");
        TransactionType type = (typeStr == "INCOME") ? INCOME : EXPENSE;

        Transaction transaction(id, name, date, description, amount, type, categoryId, profileId);
        result.append(transaction);
    }

    return result;
}

/**
 * @brief Retrieves all transactions from the database
 * @return Vector of all Transaction objects
 */
QVector<Transaction> TransactionRepository::getAll() const
{
    QVector<Transaction> result;

    QSqlQuery query(database);

    if (!query.exec("SELECT id, name, date, description, amount, type, profile_id, category_id FROM transactions"))
    {
        qDebug() << "TransactionRepository::getAll error:" << query.lastError().text();
        return result;
    }

    while (query.next())
    {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        QString dateStr = query.value(2).toString();
        QString description = query.value(3).toString();
        double amount = query.value(4).toDouble();
        QString typeStr = query.value(5).toString();
        int associatedProfileId = query.value(6).toInt();
        int categoryId = query.value(7).toInt();
        QDate date = QDate::fromString(dateStr, "yyyy-MM-dd");

        TransactionType type = (typeStr == "INCOME") ? INCOME : EXPENSE;

        Transaction transaction(id, name, date, description, amount, type, categoryId, associatedProfileId);
        result.append(transaction);
    }

    return result;
}

/**
 * @brief Adds a new transaction to the database
 *
 * Determines transaction type based on amount sign:
 * Positive amount: EXPENSE 
 * Negative amount: INCOME 
 *
 * @param transaction Transaction object to add
 * @return True if transaction added successfully, false otherwise
 */
bool TransactionRepository::addTransaction(const Transaction& transaction)
{
    QSqlQuery query(database);

    query.prepare(
        "INSERT INTO transactions (name, type, date, description, amount, category_id, profile_id) "
        "VALUES (:name, :type, :date, :description, :amount, :category_id, :profile_id)"
    );

    query.bindValue(":name", transaction.getTransactionName());

    // Determine transaction type based on amount sign
    QString typeStr = (transaction.getTransactionAmount() >= 0.0) ? "INCOME" : "EXPENSE";
    query.bindValue(":type", typeStr);

    query.bindValue(":date", transaction.getTransactionDate().toString("yyyy-MM-dd"));
    query.bindValue(":description", transaction.getTransactionDescription());
    query.bindValue(":amount", transaction.getTransactionAmount());
    query.bindValue(":profile_id", transaction.getAssociatedProfileId());
    query.bindValue(":category_id", transaction.getCategoryId());

    if (!query.exec())
    {
        qDebug() << "TransactionRepository::add error:" << query.lastError().text();
        return false;
    }

    return true;
}

/**
 * @brief Deletes a transaction by ID
 * @param id ID of transaction to delete
 * @return True if transaction deleted successfully, false otherwise
 */
bool TransactionRepository::removeTransactionById(int id)
{
    QSqlQuery query(database);
    query.prepare("DELETE FROM transactions WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "TransactionRepository::removeById error:" << query.lastError().text();
        return false;
    }

    return true;
}

/**
 * @brief Retrieves all categories for a specific profile
 * @param profileId ID of the profile
 * @return Vector of Category objects belonging to the profile
 */
QVector<Category> CategoryRepository::getAllProfileCategories(int profileId) const
{
    QVector<Category> categoriesForProfile;

    QSqlQuery query(database);

    query.prepare("SELECT id, category_name, profile_id FROM category WHERE profile_id = :profile_id OR id = 1");//'OR id = 1' to fetch 'None' category that does not have profile_id assinged to them
    query.bindValue(":profile_id", profileId);

    if (!query.exec())
    {
        qDebug() << "CategoryRepository::error: Couldn't get categories" << query.lastError().text();
        return categoriesForProfile;
    }

    while (query.next())
    {
        int id = query.value(0).toInt();
        QString categoryName = query.value(1).toString();
        int categoryProfileId = query.value(2).toInt();

        Category category(id, categoryName, categoryProfileId);
        categoriesForProfile.append(category);
    }

    return categoriesForProfile;
}

/**
 * @brief Creates a new category for a profile
 * @param categoryName Name of the category to create
 * @param profileId ID of the profile that will own the category
 * @return True if category created successfully, false otherwise
 */
bool CategoryRepository::addCategory(const QString& categoryName, int profileId)
{
    QSqlQuery query(database);

    query.prepare("INSERT INTO category (category_name, profile_id) VALUES (:name, :profile_id)");
    query.bindValue(":name", categoryName);
    query.bindValue(":profile_id", profileId);

    if (!query.exec())
    {
        qDebug() << "CategoryRepository:: error: Couldn't add category to database" << query.lastError().text();
        return false;
    }

    return true;
}

/**
 * @brief Deletes a category by ID using a database transaction
 *
 * This method ensures data integrity by:
 * 1. Starting a database transaction
 * 2. Updating all transactions using the category to use the default category (ID 1)
 * 3. Deleting the category
 * 4. Committing the transaction if both operations succeed
 *
 * The default category (ID 1, "None") cannot be deleted.
 *
 * @param categoryId ID of category to delete
 * @return True if category deleted successfully, false otherwise
 */
bool CategoryRepository::removeCategoryById(int categoryId)
{
    //If user tries to remove 'None' category, not allow them
    if (categoryId == 1)//Can adjust magic numbers in some type of namespace
    {
        return false;
    }
    QSqlQuery query(database);

    // Begin transaction to ensure data consistency
    database.transaction();

    // Update all transactions using this category to use default category 
    query.prepare("UPDATE transactions SET category_id = :defaultId WHERE category_id = :catId");
    query.bindValue(":defaultId", 1);
    query.bindValue(":catId", categoryId);

    if (!query.exec()) {
        qDebug() << "CategoryRepo::removeCategory update transactions error:" << query.lastError().text();
        database.rollback();
        return false;
    }

    // Delete the category
    query.prepare("DELETE FROM category WHERE id = :id");
    query.bindValue(":id", categoryId);

    if (!query.exec())
    {
        qDebug() << "CategoryRepository:: error: Couldn't remove category from database" << query.lastError().text();
        database.rollback();
        return false;
    }

    // Commit the transaction
    if (!database.commit()) {
        qDebug() << "CategoryRepo::removeCategory commit failed:" << database.lastError().text();
        database.rollback();
        return false;
    }

    return true;
}

/**
 * @brief Retrieves category name by ID
 * @param categoryId ID of the category
 * @return Category name as QString, empty string if category not found
 */
QString CategoryRepository::getCategoryNameById(int categoryId)
{
    QSqlQuery query(database);
    QString categoryName = "";

    query.prepare("SELECT category_name FROM category WHERE id = :id");
    query.bindValue(":id", categoryId);

    if (!query.exec())
    {
        qDebug() << "CategoryRepository:: error: Couldn't find category in database" << query.lastError().text();
        return categoryName;
    }

    if (query.next())
    {
        categoryName = query.value(0).toString();
    }

    return categoryName;
}