#include "Controller/CategorySelectionController.h"

CategoryController::CategoryController(
    CategorySelectionView& categorySelectionViewRef,
    CategoryRepository& categoryRepositoryRef, 
    QObject* parent) : BaseController(parent),
    categoryDialog(categorySelectionViewRef),   
    categoryRepository(categoryRepositoryRef)
{    
    connect(&categoryDialog, &CategorySelectionView::selectRequestedCategory,
        this, &CategoryController::handleCategorySelection);
    connect(&categoryDialog, &CategorySelectionView::addRequestedCategory,
        this, &CategoryController::handleAddCategoryRequest);
    connect(&categoryDialog, &CategorySelectionView::deleteRequestedCategory,
        this, &CategoryController::handleDeleteCategoryRequest);
    connect(&categoryDialog, &CategorySelectionView::editRequestedCategory,
        this, &CategoryController::handleEditCategoryRequest);
}

void CategoryController::showCategoryDialog(bool withSelectButton)
{
    QVector<Category> categories = categoryRepository.getAllProfileCategories(getProfileId());
    categoryDialog.setCategories(categories);
    categoryDialog.setSelectCategoryButtonVisible(withSelectButton);
    categoryDialog.exec();
}

void CategoryController::handleCategorySelection(int categoryId)
{
    selectedCategoryIdForTransaction = categoryId;
    categoryDialog.accept();
}


void CategoryController::handleAddCategoryRequest(const QString& categoryName)
{
    if (!categoryRepository.addCategory(categoryName, getProfileId()))
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
    QVector<Category> categories = categoryRepository.getAllProfileCategories(getProfileId());
    categoryDialog.setCategories(categories);
}

void CategoryController::handleCategorySelectionWhileAddingTransactionRequest(TransactionBuilder& builder)
{
    showCategoryDialog(true);
    builder.withCategoryId(selectedCategoryIdForTransaction);
    emit categorySelected(builder);
}
void CategoryController::handleCategorySelectionFromTransactionWindow(bool selectButtonVisibility)
{
    showCategoryDialog(selectButtonVisibility);
}

void CategoryController::handleEditCategoryRequest(int categoryId, const QString& newName)
{
    if (!categoryRepository.updateCategory(categoryId, newName))
    {
        const QString header = tr("Edit category");
        const QString message = tr("Failed to update category.");
        categoryDialog.showCategoryMessage(header, message, "error");
        return;
    }

    refreshCategoryDialogList();

    emit categoriesDataChanged();
}