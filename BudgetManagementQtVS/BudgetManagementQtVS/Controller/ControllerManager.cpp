#include <Controller/ControllerManager.h>
ControllerManager::ControllerManager(UserController& userControllerRef, ProfileController& profileControllerRef, TransactionController& transactionControllerRef, CategoryController& categoryControllerRef, QObject* parent) : QObject(parent), userController(userControllerRef), profileController(profileControllerRef), transactionController(transactionControllerRef), categoryController(categoryControllerRef)
{
    connect(&userController, &UserController::userLoggedIn,
        &profileController, &ProfileController::showProfilesForCurrentUser);
    connect(&profileController, &ProfileController::profileSelected,
        &transactionController, &TransactionController::initializeMainWindow);
    connect(&transactionController, &TransactionController::categorySelectionRequest,
        &categoryController, &CategoryController::handleCategorySelectionWhileAddingTransactionRequest);
    connect(&categoryController, &CategoryController::categorySelected,
        &transactionController, &TransactionController::finalizeTransaction);
    connect(&transactionController, &TransactionController::showCategories,
        &categoryController, &CategoryController::handleCategorySelectionFromTransactionWindow);
    connect(&categoryController, &CategoryController::categoriesDataChanged,
        &transactionController, &TransactionController::handleCategoriesDataChangeRequest);
}