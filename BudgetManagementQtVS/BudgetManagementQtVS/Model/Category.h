#pragma once

#include <QString>

class Category {
private:
	int categoryId;
	QString categoryName;
public:

	// constructor
	Category(int categoryId, const QString& categoryName);

	// destructor
	~Category();

	// getters
	int getCategoryId() const;
	QString getCategoryName() const;

	// setters
	void setCategoryId(int id);
	void setCategoryName(const QString& name);

	void editCategory(const QString& name);
};