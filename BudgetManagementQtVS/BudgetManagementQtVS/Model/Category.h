#pragma once

#include <QString>

class Category {
private:
	int categoryId;
	QString categoryName;
	int profileId;
public:

	// constructor
	Category(int categoryId, const QString& categoryName, int profileId);

	// destructor
	~Category();

	// getters
	int getCategoryId() const;
	QString getCategoryName() const;
	int getIdOfProfileConnectedToCategory() const;

	// setters
	void setCategoryId(int id);
	void setCategoryName(const QString& name);

	void editCategory(const QString& name);
};