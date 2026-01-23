/**
 * @file CategorySelectionController.h
 * @brief Header file for the Category Controller.
 */
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

 /**
  * @class CategoryController
  * @brief Manages category interactions, including CRUD operations, filtering, and sorting.
  */
class CategoryController : public BaseController
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the controller.
     * @param categoryRepositoryRef Reference to the category repository.
     * @param parent Parent QObject.
     */
    CategoryController(CategoryRepository& categoryRepositoryRef, QObject* parent = nullptr);

    /** @brief Initializes the controller logic. */
    void run();

    /** @brief Refreshes the category list view. */
    void showCategories();

    /** @brief Returns the associated view widget. */
    QWidget* getView();
signals:
    /** @brief Emitted when category data changes. */
    void categoriesDataChanged();

public slots:
    /** @brief Configures view connections. */
    void setupCategoryView();

    /** @brief Handles request to add a new category. */
    void handleAddCategoryRequest(const QString& categoryName);
    /** @brief Handles request to delete a category. */
    void handleDeleteCategoryRequest(int categoryId);
    /** @brief Handles request to edit a category. */
    void handleEditCategoryRequest(int categoryId, const QString& newName);
    /** @brief Handles filtering requests. */
    void handleFilteringCategoryRequest(const QString& searchText);
    /** @brief Handles sorting requests. */
    void handleSortRequest(int columnId);
private:
    QPointer<CategorySelectionView> categoryView;
    CategoryRepository& categoryRepository;

    int selectedCategoryIdForTransaction = 1; // Default/Protected ID

    void refreshTable();
    QVector<Category> executeFilteringCategory(const QVector<Category> allCategories);
    void executeSortingCategory(QVector<Category>& allCategories);
    void executeSortingCategory(QVector<Category>& allCategories) const;
};