#include <Controller/ControllerManager.h>
ControllerManager::ControllerManager(UserController& userControllerRef, ProfileController& profileControllerRef, TransactionController& transactionControllerRef, CategoryController& categoryControllerRef, FinancialAccountController& financialAccountControllerRef, QObject* parent) : QObject(parent), userController(userControllerRef), profileController(profileControllerRef), transactionController(transactionControllerRef), categoryController(categoryControllerRef), financialAccountController(financialAccountControllerRef)
{
    connect(&userController, &UserController::userLoggedIn,
        &profileController, &ProfileController::showProfilesForCurrentUser);
    connect(&profileController, &ProfileController::profileSelected,
        &transactionController, &TransactionController::initializeMainWindow);
    connect(&transactionController, &TransactionController::categorySelectionRequest,
        &categoryController, &CategoryController::handleCategorySelectionWhileAddingTransactionRequest);
    connect(&transactionController, &TransactionController::financialAccountSelectionRequest,
        &financialAccountController, &FinancialAccountController::handleFinancialAccountSelectionWhileAddingTransactionRequest);
    connect(&categoryController, &CategoryController::categorySelected,
        &financialAccountController, &FinancialAccountController::handleFinancialAccountSelectionWhileAddingTransactionRequest);
    connect(&financialAccountController, &FinancialAccountController::financialAccountSelected,
        &transactionController, &TransactionController::finalizeAddingTransaction);
    connect(&transactionController, &TransactionController::showCategories,
        &categoryController, &CategoryController::handleCategorySelectionFromTransactionWindow);
    connect(&transactionController, &TransactionController::showFinancialAccounts,
        &financialAccountController, &FinancialAccountController::handleFinancialAccountSelectionFromTransactionWindow);
    connect(&categoryController, &CategoryController::categoriesDataChanged,
        &transactionController, &TransactionController::handleCategoriesDataChangeRequest);
    connect(&financialAccountController, &FinancialAccountController::financialAccountDataChanged,
        &transactionController, &TransactionController::handleFinancialAccountsDataChangeRequest);
}