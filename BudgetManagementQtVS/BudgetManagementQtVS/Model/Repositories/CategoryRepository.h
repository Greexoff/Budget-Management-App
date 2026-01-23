#pragma once
#include <Model/Repositories/BaseRepository.h>

class CategoryRepository : public BaseRepository
{
public:
    CategoryRepository() = default;

    QVector<Category> getAllProfileCategories(int profileId) const;

    bool addCategory(const QString& category, int profileId) const;

    bool removeCategoryById(int categoryId) const;

    QString getCategoryNameById(int categoryId) const;

    bool updateCategory(int categoryId, const QString& newName) const;
};