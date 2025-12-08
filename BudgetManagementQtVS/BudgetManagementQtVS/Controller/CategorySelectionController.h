#pragma once
#include "Model/Repositories.h"
#include "Model/Category.h"

#include "View/CategorySelectionView.h"

#include <QObject>
#include <QVector>
#include <QStringList>
#include <QDate>


class CategoryController : public QObject
{
	Q_OBJECT
    // Category management handlers
public:
    CategoryController(
        CategorySelectionView& categorySelectionViewRef,
        CategoryRepository& categoryRepositoryRef,
        QObject* parent = nullptr);

    void showCategoryDialog(bool withSelectButton);
    /**
    * @brief Prompts user to select a category for a transaction
     * @return ID of selected category, or -1 if cancelled
    */
    int getCategoryIdFromInput();

    /**
    * @brief Shows category management dialog
    */
    void handleShowCategorySelectionRequest(int currentProfileId);
    QString getCategoryNameById(int categoryId);
signals:
    void categoriesDataChanged();
private slots:
    /**
     * @brief Handles category selection from dialog
     * @param categoryId ID of the selected category
     */
    void handleCategorySelection(int categoryId);

    /**
     * @brief Handles request to add a new category
     * @param categoryName Name of the category to create
     */
    void handleAddCategoryRequest(const QString& categoryName);

    /**
     * @brief Handles request to delete a category
     * @param categoryId ID of the category to delete
     */
    void handleDeleteCategoryRequest(int categoryId);

private:
    CategorySelectionView& categoryDialog;      ///< Reference to the category selection dialog

    CategoryRepository& categoryRepository;         ///< Reference to the category repository

    int selectedCategoryIdForTransaction = 1;//REPLACE MAGIC NUMBER WITH SOME SORT OF NAMESPACE (ID OF 'NONE' CATEGORY)
    int currentProfileId = -1;
    /**
     * @brief Refreshes the category list in the dialog
     */
    void refreshCategoryDialogList();

};