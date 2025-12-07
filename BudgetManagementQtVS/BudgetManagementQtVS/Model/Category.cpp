#include "model/Category.h"

/**
* @brief Constructs a Category with the given identifier, name, and profile association
* @param categoryId Unique identifier for the category
* @param categoryName Display name for the category
* @param profileId ID of the profile that owns this category
*/
Category::Category(int categoryId, const QString & categoryName, int profileId)
	: categoryId(categoryId), categoryName(categoryName), profileId(profileId) {
}

/**
 * @brief Destructor
 */
Category::~Category() {}

/**
 * @brief Returns the category's unique identifier
 * @return int The category ID
 */
int Category::getCategoryId() const {
	return categoryId;
}

/**
 * @brief Returns the category's display name
 * @return QString The category name
 */
QString Category::getCategoryName() const {
	return categoryName;
}

/**
 * @brief Returns the ID of the profile that owns this category
 * @return int The profile ID
 *
 * This method enables category isolation between different profiles,
 * ensuring that each user's categories remain private to their profiles.
 */
int Category::getIdOfProfileConnectedToCategory() const
{
	return profileId;
}

/**
 * @brief Updates the category's identifier
 * @param Id New identifier to assign
 */
void Category::setCategoryId(int id) {
	categoryId = id;
}

/**
 * @brief Updates the category's display name
 * @param new New name to assign
 */
void Category::setCategoryName(const QString& name) {
	categoryName = name;
}

/**
 * @brief Updates the category name
 * @param name New name for the category
 */
void Category::editCategory(const QString& name) {
	categoryName = name;
}