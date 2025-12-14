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
    void categorySelected(TransactionBuilder& builder);
public slots:
    void handleCategorySelectionWhileAddingTransactionRequest(TransactionBuilder& builder);
    void handleCategorySelection(int categoryId);
    void handleCategorySelectionFromTransactionWindow(bool selectButtonVisibility);
    void handleAddCategoryRequest(const QString& categoryName);
    void handleDeleteCategoryRequest(int categoryId);
    void handleEditCategoryRequest(int categoryId, const QString& newName);
    void handleFilteringCategoryRequest(const QString& searchText);

private:
    CategorySelectionView& categoryDialog;      ///< Reference to the category selection dialog
    CategoryRepository& categoryRepository;         ///< Reference to the category repository

    int selectedCategoryIdForTransaction = 1;//REPLACE MAGIC NUMBER WITH SOME SORT OF NAMESPACE (ID OF 'NONE' CATEGORY)
 
    void refreshCategoryDialogList();
    void showCategoryDialog(bool withSelectButton);
    QVector<Category> executeFilteringCategory(const QVector<Category> allCategories);

};