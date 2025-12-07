#pragma once

#include <QString>

/**
 * @class Category
 * @brief Represents a transaction category associated with a specific profile
 *
 * The Category class encapsulates information about transaction categories
 * that are scoped to individual user profiles. 
 */
class Category {
private:
	int categoryId;			///< Unique identifier for the category (database primary key)
	QString categoryName;	///< Display name of the category
	int profileId;         ///< ID of the profile this category belongs to
public:
	/**
	 * @brief Constructs a Category object with specified properties
	 * @param categoryId Unique numerical identifier for the category
	 * @param categoryName Descriptive name for the category
	 * @param profileId ID of the profile that owns this category
	 */
	Category(int categoryId, const QString& categoryName, int profileId);

	/**
	 * @brief Destructor
	 */
	~Category();

	/**
	 * @brief Gets the category identifier
	 * @return int Category ID
	 */
	int getCategoryId() const;

	/**
	* @brief Gets the category name
	* @return QString Category display name
	*/
	QString getCategoryName() const;

	/**
	 * @brief Retrieves the ID of the profile that owns this category
	 * @return int The profile ID
	 */
	int getIdOfProfileConnectedToCategory() const;

	/**
	 * @brief Sets the category identifier
	 * @param id New category ID
	 */
	void setCategoryId(int id);

	/**
	 * @brief Sets the category name
	 * @param name New category name
	 */
	void setCategoryName(const QString& name);

	/**
	 * @brief Edits the category with new name
	 * @param name New name for the category
	 */
	void editCategory(const QString& name);
};