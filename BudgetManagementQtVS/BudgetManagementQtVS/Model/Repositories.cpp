#include "Repositories.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

BaseRepository::BaseRepository() : database(DatabaseManager::instance().database()) {}

int UserRepository::authenticateUser(QString username, QString password) const
{
    QSqlQuery query(database);
    query.prepare("SELECT id, password FROM users WHERE username = :username");
    query.bindValue(":username", username);//nwm czy tego nie zamienic i dodac do tego id zeby po tym szukac//lepeij bedzie chyba po ID bo wiesz moze byc czysto teorteycznie dwa username takie smao albo mozemy zrobic ze nie mozna tworzyc 2 takich samow userow tez
    //^jest w sumie unique w username w sql wiec jak username jest unikalny

    if(!query.exec())
    {
       qDebug() << "UserRepo::authentication error:" << query.lastError().text();
       return -1;
    }
    if (query.next())
    {
        QString storedPassword = query.value(1).toString();
        if (storedPassword == password)//tu by sie w sumie hash przydal ale narazie wywalone
        {
            return query.value(0).toInt();
        }
    }
    return -1;
}
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

QVector<Category> CategoryRepository::getAllCategories(int profileId) const
{
    QVector<Category> categoriesForProfile;

    QSqlQuery query(database);

    query.prepare("SELECT id, category_name, profile_id FROM category WHERE profile_id = :profile_id");
    query.bindValue(":profile_id", profileId);
    if (!query.exec())
    {
        qDebug() << "CategoryRepository::error: Couldn't get categories" << query.lastError().text();
        return categoriesForProfile;
    }   

    while (query.next())
    {
        int id = query.value(0).toInt();
        QString category_name = query.value(1).toString();
        int profileId = query.value(2).toInt();
        Category category(id, category_name, profileId);
        categoriesForProfile.append(category);
    }

    return categoriesForProfile;
}

bool CategoryRepository::addCategory(const QString& categoryName, int profileId)//TO DO ZMIANY
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