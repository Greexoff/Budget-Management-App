#pragma once
#include <Model/Repositories/BaseRepository.h>

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
    QVector<Category> getAllProfileCategories(int profileId) const;

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
    bool removeCategoryById(int categoryId);

    /**
     * @brief Retrieves category name by ID
     * @param categoryId ID of the category
     * @return Category name as QString, empty string if category not found
     */
    QString getCategoryNameById(int categoryId);

    bool updateCategory(int categoryId, const QString& newName);
};