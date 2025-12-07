#pragma once

#include <QString>

/**
 * @brief Represents a transaction category in the budgeting application
 *
 * This class encapsulates category data including unique identifier and name.
 * It provides methods for accessing and modifying category properties.
 */
class Category {
private:
	int categoryId;			///< Unique identifier for the category (database primary key)
	QString categoryName;	///< Display name of the category
public:
	/**
	 * @brief Constructs a new Category object
	 * @param categoryId Unique identifier for the category
	 * @param categoryName Display name of the category
	 */
	Category(int categoryId, const QString& categoryName);

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