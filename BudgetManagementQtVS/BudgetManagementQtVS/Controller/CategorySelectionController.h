#pragma once
#include "Model/Repositories.h"
#include "Model/Category.h"

#include "View/CategorySelectionView.h"

#include <QObject>
#include <QVector>
#include <QStringList>
#include <QDate>


class CategoryController : public QObject
{
	Q_OBJECT

public:
    CategoryController(CategorySelectionView& categorySelectionViewRef, CategoryRepository& categoryRepositoryRef, QObject* parent = nullptr);

    int getCategoryIdFromInput();
    void handleShowCategorySelectionRequest(int currentProfileId);
    QString getCategoryNameById(int categoryId);

signals:
    void categoriesDataChanged();

private slots:
    void handleCategorySelection(int categoryId);
    void handleAddCategoryRequest(const QString& categoryName);
    void handleDeleteCategoryRequest(int categoryId);

private:
    CategorySelectionView& categoryDialog;      ///< Reference to the category selection dialog
    CategoryRepository& categoryRepository;         ///< Reference to the category repository

    int selectedCategoryIdForTransaction = 1;//REPLACE MAGIC NUMBER WITH SOME SORT OF NAMESPACE (ID OF 'NONE' CATEGORY)
    int currentProfileId = -1;//REPLACE MAGIC NUMBER WITH SOME SORT OF NAMESPACE
 
    void refreshCategoryDialogList();
    void showCategoryDialog(bool withSelectButton);

};