#include "Controller/CategorySelectionController.h"
#include <QInputDialog>
#include <QMessageBox>
#include <algorithm>


CategoryController::CategoryController(CategoryRepository& categoryRepositoryRef, QObject* parent)
    : BaseController(parent),
    categoryRepository(categoryRepositoryRef)
{
    categoryView = new CategorySelectionView();
    setupCategoryView();
    refreshTable();
}

void CategoryController::run()
{
    refreshTable();
}

QWidget* CategoryController::getView()
{
    return categoryView;
}

void CategoryController::setupCategoryView()
{
    // 1. ADD CATEGORY
    connect(categoryView, &CategorySelectionView::addCategoryRequest, this, [this]() {
        bool ok;
        QString name = QInputDialog::getText(categoryView, "Add Category", "Category Name:", QLineEdit::Normal, "", &ok);
        if (ok && !name.isEmpty()) {
            handleAddCategoryRequest(name);
        }
        });

    // 2. EDIT CATEGORY
    connect(categoryView, &CategorySelectionView::editCategoryRequest, this, [this]() {
        int id = categoryView->getSelectedCategoryId();
        if (id == -1) {
            categoryView->showMessage("Warning", "Select a category to edit.", "error");
            return;
        }

        bool ok;
        QString name = QInputDialog::getText(categoryView, "Edit Category", "New Name:", QLineEdit::Normal, "", &ok);
        if (ok && !name.isEmpty()) {
            handleEditCategoryRequest(id, name);
        }
        });

    // 3. DELETE CATEGORY
    connect(categoryView, &CategorySelectionView::deleteCategoryRequest, this, [this]() {
        int id = categoryView->getSelectedCategoryId();
        if (id == -1) {
            categoryView->showMessage("Warning", "Select a category to delete.", "error");
            return;
        }

        auto reply = QMessageBox::question(categoryView, "Confirm",
            "Are you sure you want to delete this category?",
            QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            handleDeleteCategoryRequest(id);
        }
        });

    // 4. SEARCH & SORT
    connect(categoryView, &CategorySelectionView::searchCategoryRequest, this, &CategoryController::handleFilteringCategoryRequest);
    connect(categoryView, &CategorySelectionView::columnSortRequest, this, &CategoryController::handleSortRequest);

    // 5. REFRESH
    connect(categoryView, &CategorySelectionView::refreshRequest, this, &CategoryController::refreshTable);
}

void CategoryController::showCategories()
{
    refreshTable();
}

void CategoryController::refreshTable()
{
    QVector<Category> categories = categoryRepository.getAllProfileCategories(getUserId());


    if (!getFilteringText().isEmpty()) {
        categories = executeFilteringCategory(categories);
    }


    executeSortingCategory(categories);


    QVector<QStringList> viewData;
    for (const auto& cat : categories) {
        QStringList row;
        row << QString::number(cat.getCategoryId());
        row << cat.getCategoryName();
        viewData.append(row);
    }


    categoryView->setCategoryTabHeaders(viewData);
    emit categoriesDataChanged();
}


void CategoryController::handleAddCategoryRequest(const QString& categoryName)
{
    if (categoryRepository.addCategory(categoryName, getUserId())) {
        categoryView->showMessage("Success", "Category added successfully.", "info");
        refreshTable();
    }
    else {
        categoryView->showMessage("Error", "Failed to add category.", "error");
    }
}

void CategoryController::handleEditCategoryRequest(int categoryId, const QString& newName)
{
    if (categoryRepository.updateCategory(categoryId, newName)) {
        categoryView->showMessage("Success", "Category updated.", "info");
        refreshTable();
    }
    else {
        categoryView->showMessage("Error", "Failed to update category.", "error");
    }
}

void CategoryController::handleDeleteCategoryRequest(int categoryId)
{
    if (categoryId == selectedCategoryIdForTransaction) {
        categoryView->showMessage("Error", "Cannot delete default category.", "error");
        return;
    }

    if (categoryRepository.removeCategoryById(categoryId)) {
        categoryView->showMessage("Success", "Category deleted.", "info");
        refreshTable();
    }
    else {
        categoryView->showMessage("Error", "Failed to delete category (might be in use).", "error");
    }
}

void CategoryController::handleFilteringCategoryRequest(const QString& searchText)
{
    setFilteringText(searchText);
    refreshTable();
}

void CategoryController::handleSortRequest(int columnId)
{
    setSelectedColumnId(columnId);
    refreshTable();
}



QVector<Category> CategoryController::executeFilteringCategory(const QVector<Category> allCategories)
{
    return executeFiltering(allCategories, [this](const Category& cat)
        {
        return cat.getCategoryName().contains(getFilteringText(), Qt::CaseInsensitive);
        });
}

void CategoryController::executeSortingCategory(QVector<Category>& allCategories) 
{
    executeSorting(allCategories, [this](const Category& a, const Category& b) {
    	if (getSelectedColumnId() == 1) { 
            return a.getCategoryName().toLower() < b.getCategoryName().toLower();
        }
        return a.getCategoryId() < b.getCategoryId();});
}
