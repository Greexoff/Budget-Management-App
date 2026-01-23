/**
 * @file Category.cpp
 * @brief Implementation of the Category model.
 */
#include "model/Category.h"

 /** @brief Constructor initializing member variables. */
Category::Category(int categoryId, const QString & categoryName, int profileId)
	: categoryId(categoryId), categoryName(categoryName), profileId(profileId) {
}
/** @brief Default destructor. */
Category::~Category() {}
/** @brief Getter for category ID. */
int Category::getCategoryId() const {
	return categoryId;
}
/** @brief Getter for category name. */
QString Category::getCategoryName() const {
	return categoryName;
}
/** @brief Getter for profile ID. */
int Category::getIdOfProfileConnectedToCategory() const
{
	return profileId;
}
/** @brief Setter for category ID. */
void Category::setCategoryId(int id) {
	categoryId = id;
}
/** @brief Setter for category name. */
void Category::setCategoryName(const QString& name) {
	categoryName = name;
}


