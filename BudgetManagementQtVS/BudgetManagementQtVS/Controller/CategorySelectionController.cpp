#include "Controller/CategorySelectionController.h"

CategoryController::CategoryController(
    CategorySelectionView& categorySelectionViewRef,
    CategoryRepository& categoryRepositoryRef, 
    QObject* parent) : BaseController(parent),
    categoryDialog(categorySelectionViewRef),   
    categoryRepository(categoryRepositoryRef)
{    
    connect(&categoryDialog, &CategorySelectionView::addRequestedCategory,
        this, &CategoryController::handleAddCategoryRequest);
    connect(&categoryDialog, &CategorySelectionView::deleteRequestedCategory,
        this, &CategoryController::handleDeleteCategoryRequest);
    connect(&categoryDialog, &CategorySelectionView::editRequestedCategory,
        this, &CategoryController::handleEditCategoryRequest);
    connect(&categoryDialog, &CategorySelectionView::searchTextRequest,
        this, &CategoryController::handleFilteringCategoryRequest);
    connect(&categoryDialog, &CategorySelectionView::columnSortRequest,
        this, &CategoryController::handleSortRequest);
}


//----------------Setting up view-------------------------------------------------


//Method responsible for setting up all categories on screen while opening window
void CategoryController::setupCategoryView()
{
    QVector<Category> categories = categoryRepository.getAllProfileCategories(getProfileId());
    categoryDialog.setCategories(categories);
    setFilteringText("");
    categoryDialog.clearSearchLineEdit();
    categoryDialog.exec();
}

//Method responsible for refreshing categories in window, used every time a change in list occurs
void CategoryController::refreshCategoryDialogList()
{
    QVector<Category> categories = categoryRepository.getAllProfileCategories(getProfileId());
    categories = executeFilteringCategory(categories);
    executeSortingCategory(categories);
    categoryDialog.setCategories(categories);
}


//----------------Handling actions performed on categories-----------------------


//Method responsible for adding category based on entered name
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

//Method responsible for handling editing of category
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

//Method responsible for handling deletion of category
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

//Method that sets up filtering text and calls refresh view method where filtering occurs
void CategoryController::handleFilteringCategoryRequest(const QString& searchText)
{
    setFilteringText(searchText);
    refreshCategoryDialogList();
}

//An actual method for handling filtering specific categories
QVector<Category> CategoryController::executeFilteringCategory(const QVector<Category> allCategories)
{
    auto matchFound = [&](const Category& category) -> bool
        {
            bool categoryMatches = category.getCategoryName().contains(getFilteringText(), Qt::CaseInsensitive);
            return categoryMatches;
        };
    return executeFiltering(allCategories, matchFound);
}

//Method that sets up selected column id on which sorting will occur and calls refresh view method where an actual sorting method is called
void CategoryController::handleSortRequest(int columnId)
{
    setSelectedColumnId(columnId);
    refreshCategoryDialogList();
}

//An actual method for handling sorting categories
void CategoryController::executeSortingCategory(QVector<Category>& allCategories)
{
    executeSorting(allCategories, getSelectedColumnId(), getLastSortingOrder());
}