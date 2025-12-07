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
 * Performs a secure comparison of provided credentials with stored values.
 *
 * @param username User's login name
 * @param password User's password
 * @return User ID if authentication successful, -1 otherwise
 */
int UserRepository::authenticateUser(QString username, QString password) const
{
    QSqlQuery query(database);
    query.prepare("SELECT id, password FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if(!query.exec())
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
 *
 * Inserts a new user record into the database. Username must be unique.
 *
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
    query.prepare("SELECT id, profile_name, user_id FROM profiles WHERE user_id=:userId");
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
        int userId = query.value(2).toInt();

        Profile profile(profileId, userId, profileName);
        foundProfiles.append(profile);
    }
    return foundProfiles;
}

/**
 * @brief Creates a new profile for a user
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
QVector<Transaction> TransactionRepository::getAllForProfile(int profileId) const
{
    QVector<Transaction> result;
    QSqlQuery query(database);
    query.prepare("SELECT id, name, date, description, amount, type, category_id, profile_id "
        "FROM transactions WHERE profile_id = :profileId");
    query.bindValue(":profileId", profileId);

    if (!query.exec())
        return result;

    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        QString dateStr = query.value(2).toString();
        QString description = query.value(3).toString();
        double amount = query.value(4).toDouble();
        QString typeStr = query.value(5).toString();
        int categoryId = query.value(6).toInt();
        int profId = query.value(7).toInt();

        QDate date = QDate::fromString(dateStr, "yyyy-MM-dd");
        TransactionType type = (typeStr == "INCOME") ? INCOME : EXPENSE;

        Transaction t(id, name, date, description, amount, type, categoryId, profId);
        result.append(t);
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
        int associatedProfile_id = query.value(6).toInt();
        int categoryId = query.value(7).toInt();
        QDate date = QDate::fromString(dateStr, "yyyy-MM-dd");


        TransactionType type = (typeStr == "INCOME") ? INCOME : EXPENSE;
 
        Transaction transaction(id, name, date, description, amount, type, categoryId, associatedProfile_id);
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
bool TransactionRepository::add(const Transaction& transaction)
{
    QSqlQuery query(database);

    query.prepare(
        "INSERT INTO transactions (name, type, date, description, amount, category_id, profile_id) "
        "VALUES (:name, :type, :date, :description, :amount, :category_id, :profile_id)");

    query.bindValue(":name", transaction.getTransactionName());


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
bool TransactionRepository::removeById(int id)
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
 * @brief Retrieves all categories from the database
 * @return Vector of all Category objects
 */
QVector<Category> CategoryRepository::getAllCategories() const
{
    QVector<Category> allCategories;

    QSqlQuery query(database);

    if (!query.exec("SELECT id, category_name FROM category"))
    {
        qDebug() << "CategoryRepository::error: Couldn't get categories" << query.lastError().text();
        return allCategories;
    }

    while (query.next())
    {
        int id = query.value(0).toInt();
        QString category_name = query.value(1).toString();

        Category category(id, category_name);
        allCategories.append(category);
    }

    return allCategories;
}

/**
 * @brief Creates a new category
 *
 * Category names must be unique.
 *
 * @param categoryName Name of the category to create
 * @return True if category created successfully, false otherwise
 */
bool CategoryRepository::addCategory(const QString& categoryName)
{
    QSqlQuery query(database);

    query.prepare("INSERT INTO category (category_name) VALUES (:name)");
    query.bindValue(":name", categoryName);

    if (!query.exec())
    {
        qDebug() << "CategoryRepository:: error: Couldn't add category to database" << query.lastError().text();
        return false;
    }
    return true;
}

/**
 * @brief Deletes a category by ID
 * @param categoryId ID of category to delete
 * @return True if category deleted successfully, false otherwise
 */
bool CategoryRepository::removeCategoryUsingId(int categoryId)
{
    QSqlQuery query(database);
    query.prepare("DELETE FROM category WHERE id = :id");
    query.bindValue(":id", categoryId);

    if (!query.exec())
    {
        qDebug() << "CategoryRepository:: error: Couldn't remove category from database" << query.lastError().text();
        return false;
    }

    return true;
}

/**
 * @brief Retrieves category name by ID
 * @param categoryId ID of the category
 * @return Category name as QString, empty string if category not found
 */
QString CategoryRepository::getNameOfCategoryBasedOnId(int categoryId)
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