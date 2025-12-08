#include "Controller/CategorySelectionController.h"

CategoryController::CategoryController(
    CategorySelectionView& categorySelectionViewRef,
    CategoryRepository& categoryRepositoryRef, 
    QObject* parent) : QObject(parent),
    categoryDialog(categorySelectionViewRef),   
    categoryRepository(categoryRepositoryRef)
{
    // Connect category dialog signals
    connect(&categoryDialog, &CategorySelectionView::selectRequestedCategory,
        this, &CategoryController::handleCategorySelection);
    connect(&categoryDialog, &CategorySelectionView::addRequestedCategory,
        this, &CategoryController::handleAddCategoryRequest);
    connect(&categoryDialog, &CategorySelectionView::deleteRequestedCategory,
        this, &CategoryController::handleDeleteCategoryRequest);
}
/**
 * @brief Displays the category dialog in specified mode
 * @param withSelectButton True for selection mode, false for management mode
 *
 * In selection mode, the dialog includes a "Select" button for
 * choosing a category for a transaction. In management mode,
 * only add/delete operations are available.
 */
void CategoryController::showCategoryDialog(bool withSelectButton)
{
    QVector<Category> categories = categoryRepository.getAllProfileCategories(currentProfileId);
    categoryDialog.setCategories(categories);
    categoryDialog.setSelectCategoryButtonVisible(withSelectButton);
    categoryDialog.exec();
}
/**
 * @brief Displays the category management dialog
 */
void CategoryController::handleShowCategorySelectionRequest(int currentProfileId)
{
    this->currentProfileId = currentProfileId;
    showCategoryDialog(false);
}

/**
 * @brief Handles category selection from the dialog
 * @param categoryId ID of the selected category
 */
void CategoryController::handleCategorySelection(int categoryId)
{
    selectedCategoryIdForTransaction = categoryId;
    categoryDialog.accept();
}

/**
 * @brief Prompts user to select a category for a transaction
 * @return ID of selected category, or -1 if cancelled
 */
int CategoryController::getCategoryIdFromInput()
{
    selectedCategoryIdForTransaction = 1;
    showCategoryDialog(true);
    return selectedCategoryIdForTransaction;
}

/**
 * @brief Handles creation of a new category
 * @param categoryName Name of the category to create
 */
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

/**
 * @brief Handles deletion of a category
 * @param categoryId ID of the category to delete
 */
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

/**
 * @brief Refreshes the category list in the dialog
 *
 * Updates the category dialog with the current list of categories
 * from the database for the active profile.
 */
void CategoryController::refreshCategoryDialogList()
{
    QVector<Category> categories = categoryRepository.getAllProfileCategories(currentProfileId);
    categoryDialog.setCategories(categories);
}
QString CategoryController::getCategoryNameById(int categoryId)
{
    return categoryRepository.getCategoryNameById(categoryId);
}