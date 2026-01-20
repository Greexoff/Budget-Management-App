#include "Controller/CategorySelectionController.h"
#include <QInputDialog>
#include <QMessageBox>
#include <algorithm>


CategoryController::CategoryController(CategorySelectionView& viewRef, CategoryRepository& categoryRepositoryRef, QObject* parent)
    : BaseController(parent),
    view(viewRef),
    categoryRepository(categoryRepositoryRef)
{
    setupCategoryView();
    refreshTable();
}

void CategoryController::setupCategoryView()
{
    // 1. ADD CATEGORY
    connect(&view, &CategorySelectionView::addCategoryRequest, this, [this]() {
        bool ok;
        QString name = QInputDialog::getText(&view, "Add Category", "Category Name:", QLineEdit::Normal, "", &ok);
        if (ok && !name.isEmpty()) {
            handleAddCategoryRequest(name);
        }
        });

    // 2. EDIT CATEGORY
    connect(&view, &CategorySelectionView::editCategoryRequest, this, [this]() {
        int id = view.getSelectedCategoryId();
        if (id == -1) {
            view.showMessage("Warning", "Select a category to edit.", "error");
            return;
        }

        bool ok;
        QString name = QInputDialog::getText(&view, "Edit Category", "New Name:", QLineEdit::Normal, "", &ok);
        if (ok && !name.isEmpty()) {
            handleEditCategoryRequest(id, name);
        }
        });

    // 3. DELETE CATEGORY
    connect(&view, &CategorySelectionView::deleteCategoryRequest, this, [this]() {
        int id = view.getSelectedCategoryId();
        if (id == -1) {
            view.showMessage("Warning", "Select a category to delete.", "error");
            return;
        }

        auto reply = QMessageBox::question(&view, "Confirm",
            "Are you sure you want to delete this category?",
            QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            handleDeleteCategoryRequest(id);
        }
        });

    // 4. SEARCH & SORT
    connect(&view, &CategorySelectionView::searchCategoryRequest, this, &CategoryController::handleFilteringCategoryRequest);
    connect(&view, &CategorySelectionView::columnSortRequest, this, &CategoryController::handleSortRequest);

    // 5. REFRESH
    connect(&view, &CategorySelectionView::refreshRequest, this, &CategoryController::refreshTable);
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


    view.setCategoryTabHeaders(viewData);
    emit categoriesDataChanged();
}


void CategoryController::handleAddCategoryRequest(const QString& categoryName)
{
    if (categoryRepository.addCategory(categoryName, getUserId())) {
        view.showMessage("Success", "Category added successfully.", "info");
        refreshTable();
    }
    else {
        view.showMessage("Error", "Failed to add category.", "error");
    }
}

void CategoryController::handleEditCategoryRequest(int categoryId, const QString& newName)
{
    if (categoryRepository.updateCategory(categoryId, newName)) {
        view.showMessage("Success", "Category updated.", "info");
        refreshTable();
    }
    else {
        view.showMessage("Error", "Failed to update category.", "error");
    }
}

void CategoryController::handleDeleteCategoryRequest(int categoryId)
{
    if (categoryId == selectedCategoryIdForTransaction) {
        view.showMessage("Error", "Cannot delete default category.", "error");
        return;
    }

    if (categoryRepository.removeCategoryById(categoryId)) {
        view.showMessage("Success", "Category deleted.", "info");
        refreshTable();
    }
    else {
        view.showMessage("Error", "Failed to delete category (might be in use).", "error");
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
    QVector<Category> filtered;
    for (const auto& cat : allCategories) {
        if (cat.getCategoryName().contains(getFilteringText(), Qt::CaseInsensitive)) {
            filtered.append(cat);
        }
    }
    return filtered;
}

void CategoryController::executeSortingCategory(QVector<Category>& allCategories)
{
    std::sort(allCategories.begin(), allCategories.end(), [this](const Category& a, const Category& b) {
        if (getLastSortingOrder() == Qt::AscendingOrder) {
            return a.getCategoryName() < b.getCategoryName();
        }
        else {
            return a.getCategoryName() > b.getCategoryName();
        }
        });
}