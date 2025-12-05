#include "model/Category.h"

// Constructor
Category::Category(int categoryId, const QString& categoryName)
	: categoryId(categoryId), categoryName(categoryName) {}

// Destructor
Category::~Category() {}

// Getters
int Category::getCategoryId() const {
	return categoryId;
}
QString Category::getCategoryName() const {
	return categoryName;
}

// Setters
void Category::setCategoryId(int id) {
	categoryId = id;
	}
void Category::setCategoryName(const QString& name) {
	categoryName = name;
}

void Category::editCategory(const QString& name) {
	categoryName = name;
}