/**
 * @file CategoryRepository.cpp
 * @brief Implementation of the Category Repository.
 */
#include <Model/Repositories/CategoryRepository.h>

 /**
  * @brief Fetches all categories for a profile, including the default category (ID 1).
  */
QVector<Category> CategoryRepository::getAllProfileCategories(int profileId) const
{
    QVector<Category> categoriesForProfile;

    QSqlQuery query(database);

    query.prepare("SELECT id, category_name, profile_id FROM category WHERE profile_id = :profile_id OR id = 1");
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
 * @brief Inserts a new category record into the category table.
 */
bool CategoryRepository::addCategory(const QString& categoryName, int profileId) const
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
 * @brief Deletes a category. Reassigns associated transactions to the default category (ID 1) before deletion.
 * Uses a transaction to ensure data integrity.
 */
bool CategoryRepository::removeCategoryById(int categoryId) const
{
    if (categoryId == 1)
    {
        return false;
    }
    QSqlQuery query(database);

    database.transaction();

    query.prepare("UPDATE transactions SET category_id = :defaultId WHERE category_id = :catId");
    query.bindValue(":defaultId", 1);
    query.bindValue(":catId", categoryId);

    if (!query.exec()) {
        qDebug() << "CategoryRepo::removeCategory update transactions error:" << query.lastError().text();
        database.rollback();
        return false;
    }

    query.prepare("DELETE FROM category WHERE id = :id");
    query.bindValue(":id", categoryId);

    if (!query.exec())
    {
        qDebug() << "CategoryRepository:: error: Couldn't remove category from database" << query.lastError().text();
        database.rollback();
        return false;
    }

    if (!database.commit()) {
        qDebug() << "CategoryRepo::removeCategory commit failed:" << database.lastError().text();
        database.rollback();
        return false;
    }

    return true;
}
/**
 * @brief Queries the category table for the name corresponding to the given ID.
 */
QString CategoryRepository::getCategoryNameById(int categoryId) const
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
/**
 * @brief Updates the category_name field for the specified ID. Prevents updating the default category.
 */
bool CategoryRepository::updateCategory(int categoryId, const QString& newName) const
{
    if (categoryId == 1) return false;

    QSqlQuery query(database);
    query.prepare("UPDATE category SET category_name = :name WHERE id = :id");
    query.bindValue(":name", newName);
    query.bindValue(":id", categoryId);

    if (!query.exec()) {
        qDebug() << "CategoryRepo::updateCategory error:" << query.lastError().text();
        return false;
    }
    return true;
}
