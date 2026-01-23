/**
 * @file CategoryRepository.h
 * @brief Header file for the Category Repository.
 */
#pragma once
#include <Model/Repositories/BaseRepository.h>

 /**
  * @class CategoryRepository
  * @brief Manages database operations for transaction categories.
  */
class CategoryRepository : public BaseRepository
{
public:
    CategoryRepository() = default;
    /**
     * @brief Retrieves all categories associated with a specific profile.
     * @param profileId The ID of the profile.
     * @return A list of Category objects.
     */
    QVector<Category> getAllProfileCategories(int profileId) const;

    /**
     * @brief Adds a new category to the database.
     * @param category The name of the category.
     * @param profileId The profile ID to associate the category with.
     * @return True if successful, false otherwise.
     */
    bool addCategory(const QString& category, int profileId) const;

    /**
     * @brief Removes a category by its ID.
     * @param categoryId The ID of the category to remove.
     * @return True if successful, false otherwise.
     */
    bool removeCategoryById(int categoryId) const;

    /**
     * @brief Retrieves the name of a category by its ID.
     * @param categoryId The ID of the category.
     * @return The name of the category.
     */
    QString getCategoryNameById(int categoryId) const;

    /**
     * @brief Updates the name of an existing category.
     * @param categoryId The ID of the category to update.
     * @param newName The new name for the category.
     * @return True if successful, false otherwise.
     */
    bool updateCategory(int categoryId, const QString& newName) const;
};