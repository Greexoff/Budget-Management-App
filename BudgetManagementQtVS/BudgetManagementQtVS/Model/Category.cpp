#include "model/Category.h"


Category::Category(int categoryId, const QString & categoryName, int profileId)
	: categoryId(categoryId), categoryName(categoryName), profileId(profileId) {
}

Category::~Category() {}

int Category::getCategoryId() const {
	return categoryId;
}

QString Category::getCategoryName() const {
	return categoryName;
}

int Category::getIdOfProfileConnectedToCategory() const
{
	return profileId;
}

void Category::setCategoryId(int id) {
	categoryId = id;
}

void Category::setCategoryName(const QString& name) {
	categoryName = name;
}


