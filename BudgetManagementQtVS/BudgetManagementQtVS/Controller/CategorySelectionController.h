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
#include <QPointer>


class CategoryController : public BaseController
{
    Q_OBJECT

public:
    CategoryController(CategoryRepository& categoryRepositoryRef, QObject* parent = nullptr);
    void run();

    void showCategories();

    QWidget* getView();

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
    QPointer<CategorySelectionView> categoryView;
    CategoryRepository& categoryRepository;

    int selectedCategoryIdForTransaction = 1; 

    void refreshTable();
    QVector<Category> executeFilteringCategory(const QVector<Category> allCategories);
    void executeSortingCategory(QVector<Category>& allCategories) const;
};