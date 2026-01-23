/**
 * @file Category.h
 * @brief Header file for the Category model class.
 */
#pragma once

#include <QString>

 /**
  * @class Category
  * @brief Represents a transaction category (e.g., Food, Rent) linked to a profile.
  */
class Category {
private:
	int categoryId;			///< Unique identifier for the category.
	QString categoryName;	///< Display name of the category.
	int profileId;          ///< ID of the profile this category belongs to.
public:
    /**
     * @brief Constructs a Category object.
     * @param categoryId Unique ID.
     * @param categoryName Name of the category.
     * @param profileId Associated profile ID.
     */
    Category(int categoryId, const QString& categoryName, int profileId);

    /** @brief Destructor. */
    ~Category();

    /** @brief Returns the category ID. */
    int getCategoryId() const;

    /** @brief Returns the category name. */
    QString getCategoryName() const;

    /** @brief Returns the ID of the connected profile. */
    int getIdOfProfileConnectedToCategory() const;

    /** @brief Sets the category ID. @param id New ID. */
    void setCategoryId(int id);

    /** @brief Sets the category name. @param name New name. */
    void setCategoryName(const QString& name);
};