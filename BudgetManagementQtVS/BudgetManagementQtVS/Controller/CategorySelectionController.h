#pragma once
#include <Model/Repositories/CategoryRepository.h>
#include "Model/Category.h"

#include <Controller/BaseController.h>

#include "View/CategorySelectionView.h"

#include <Model/TransactionBuilder.h>

#include <QObject>
#include <QVector>
#include <QStringList>
#include <QDate>


class CategoryController : public BaseController
{
	Q_OBJECT

public:
    CategoryController(CategorySelectionView& categorySelectionViewRef, CategoryRepository& categoryRepositoryRef, QObject* parent = nullptr);
signals:
    void categoriesDataChanged();
public slots:
    void setupCategoryView();
    void handleAddCategoryRequest(const QString& categoryName);
    void handleDeleteCategoryRequest(int categoryId);
    void handleEditCategoryRequest(int categoryId, const QString& newName);
    void handleFilteringCategoryRequest(const QString& searchText);
    void handleSortRequest(int columnId);

private:
    CategorySelectionView& categoryDialog;      
    CategoryRepository& categoryRepository; 

    int selectedCategoryIdForTransaction = 1;//REPLACE MAGIC NUMBER WITH SOME SORT OF NAMESPACE (ID OF 'NONE' CATEGORY)
 
    void refreshCategoryDialogList();
    QVector<Category> executeFilteringCategory(const QVector<Category> allCategories);
    void executeSortingCategory(QVector<Category>& allCategories);

};