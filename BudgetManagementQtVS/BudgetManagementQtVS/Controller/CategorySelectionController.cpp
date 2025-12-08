#include "Controller/CategorySelectionController.h"

CategoryController::CategoryController(
    CategorySelectionView& categorySelectionViewRef,
    CategoryRepository& categoryRepositoryRef, 
    QObject* parent) : QObject(parent),
    categoryDialog(categorySelectionViewRef),   
    categoryRepository(categoryRepositoryRef)
{
    
    connect(&categoryDialog, &CategorySelectionView::selectRequestedCategory,
        this, &CategoryController::handleCategorySelection);
    connect(&categoryDialog, &CategorySelectionView::addRequestedCategory,
        this, &CategoryController::handleAddCategoryRequest);
    connect(&categoryDialog, &CategorySelectionView::deleteRequestedCategory,
        this, &CategoryController::handleDeleteCategoryRequest);
}

void CategoryController::showCategoryDialog(bool withSelectButton)
{
    QVector<Category> categories = categoryRepository.getAllProfileCategories(currentProfileId);
    categoryDialog.setCategories(categories);
    categoryDialog.setSelectCategoryButtonVisible(withSelectButton);
    categoryDialog.exec();
}

void CategoryController::handleShowCategorySelectionRequest(int currentProfileId)
{
    this->currentProfileId = currentProfileId;
    showCategoryDialog(false);
}

void CategoryController::handleCategorySelection(int categoryId)
{
    selectedCategoryIdForTransaction = categoryId;
    categoryDialog.accept();
}

int CategoryController::getCategoryIdFromInput()
{
    selectedCategoryIdForTransaction = 1;
    showCategoryDialog(true);
    return selectedCategoryIdForTransaction;
}


void CategoryController::handleAddCategoryRequest(const QString& categoryName)
{
    if (!categoryRepository.addCategory(categoryName, currentProfileId))
    {
        const QString header = tr("New category");
        const QString message = tr("Failed to add a category.");
        categoryDialog.showCategoryMessage(header, message, "error");
    }

    refreshCategoryDialogList();
}


void CategoryController::handleDeleteCategoryRequest(int categoryId)
{
    if (!categoryRepository.removeCategoryById(categoryId))
    {
        const QString header = tr("Delete category");
        const QString message = tr("Failed to delete a category.");
        categoryDialog.showCategoryMessage(header, message, "error");
    }

    refreshCategoryDialogList();

    emit categoriesDataChanged();
}


void CategoryController::refreshCategoryDialogList()
{
    QVector<Category> categories = categoryRepository.getAllProfileCategories(currentProfileId);
    categoryDialog.setCategories(categories);
}
QString CategoryController::getCategoryNameById(int categoryId)
{
    return categoryRepository.getCategoryNameById(categoryId);
}