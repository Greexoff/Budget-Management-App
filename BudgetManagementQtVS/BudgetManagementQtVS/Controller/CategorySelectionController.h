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
#include <QInputDialog>
#include <QMessageBox>
#include <algorithm>


class CategoryController : public BaseController
{
    Q_OBJECT

public:
    CategoryController(CategorySelectionView& viewRef, CategoryRepository& categoryRepositoryRef, QObject* parent = nullptr);

    void showCategories();

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
    CategorySelectionView& view;
    CategoryRepository& categoryRepository;

    int selectedCategoryIdForTransaction = 1; 

    void refreshTable();
    QVector<Category> executeFilteringCategory(const QVector<Category> allCategories);
    void executeSortingCategory(QVector<Category>& allCategories);
};