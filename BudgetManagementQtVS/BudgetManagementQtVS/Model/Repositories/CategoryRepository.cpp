#include <Model/Repositories/CategoryRepository.h>

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

bool CategoryRepository::updateCategory(int categoryId, const QString& newName)
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