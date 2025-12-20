#pragma once

#include <QString>

class Category {
private:
	int categoryId;	
	QString categoryName;	
	int profileId;         
public:

	Category(int categoryId, const QString& categoryName, int profileId);

	~Category();

	int getCategoryId() const;

	QString getCategoryName() const;

	int getIdOfProfileConnectedToCategory() const;

	void setCategoryId(int id);

	void setCategoryName(const QString& name);

};