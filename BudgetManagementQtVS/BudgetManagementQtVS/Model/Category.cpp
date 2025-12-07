#include "model/Category.h"

/**
 * @brief Constructs a Category with the given identifier and name
 * @param categoryId Unique identifier for the category
 * @param categoryName Display name for the category
 */
Category::Category(int categoryId, const QString& categoryName)
	: categoryId(categoryId), categoryName(categoryName) {}

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